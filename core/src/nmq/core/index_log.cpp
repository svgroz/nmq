#include <cstddef>
#include <memory>

#include <boost/compute/detail/lru_cache.hpp>
#include <spdlog/spdlog.h>

#include <nmq/core/buffer.h>
#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <nmq/core/types.h>

namespace nmq {

inline constexpr std::int_fast64_t CHUNK_SIZE =
    static_cast<std::int_fast64_t>(sizeof(IndexChunk));

IndexLog::IndexLog(std::string &filename, std::int_fast64_t page_size)
    : _filename(filename),
      _file(_filename, std::fstream::in | std::fstream::out |
                           std::fstream::binary | std::fstream::app),
      _page_size(page_size), _chunks_on_page(_page_size / CHUNK_SIZE),
      _chunks_on_page_raw_size(_chunks_on_page * CHUNK_SIZE),
      _page_tile_size(_page_size - _chunks_on_page_raw_size), _page_cache(20) {

  if (_file.good()) {
    spdlog::info("file is opened: {}", _filename);
  } else {
    auto error_code = errno;
    spdlog::error("could not open file: {} {}:{}", _filename,
                  std::strerror(error_code), error_code);
    throw SystemError(error_code);
  }
};

auto IndexLog::push_back(IndexChunk index_chunk) -> void {
  char *raw_chunk_ptr = reinterpret_cast<char *>(&index_chunk);

  _file.seekg(0, std::fstream::end);
  std::int_fast64_t end = _file.tellg();

  auto current_position_on_page = end % _page_size;
  if (current_position_on_page >= _chunks_on_page_raw_size) {
    auto buffer_size = _page_tile_size + CHUNK_SIZE;
    auto tail_buffer = Buffer<int>(buffer_size, 0);

    std::memcpy(tail_buffer.data() + _page_tile_size, &index_chunk, CHUNK_SIZE);
    _file.write(tail_buffer.data(), tail_buffer.size());
  } else {
    _file.write(raw_chunk_ptr, CHUNK_SIZE);
  }
};

auto IndexLog::chunks_available(std::int_fast64_t file_size) noexcept
    -> std::int_fast64_t {
  const std::int_fast64_t pages_available = (file_size / _page_size) + 1;
  const std::int_fast64_t last_page_size =
      file_size - ((pages_available - 1) * _page_size);
  return ((pages_available - 1) * _chunks_on_page) +
         (last_page_size / CHUNK_SIZE);
};

auto IndexLog::load_page_buffer(std::int_fast64_t page_index,
                                std::int_fast64_t file_size) -> PageBuffer {

  const std::int_fast64_t current_page_start = page_index * _page_size;
  const std::int_fast64_t to_read = file_size - current_page_start >= _page_size
                                        ? _page_size
                                        : file_size - current_page_start;
  const std::int_fast64_t chunks_to_read = to_read / CHUNK_SIZE;

  bool refresh_is_necessery = false;
  boost::optional<PageBuffer> cached_page_buffer = _page_cache.get(page_index);
  if (cached_page_buffer.has_value() &&
      cached_page_buffer.value()->context()._chunks_on_page == chunks_to_read) {
    return cached_page_buffer.value();
  }

  spdlog::info(
      "start load_page page_index: {} current_page_start: {} to_read: {} "
      "current_page_start+to_read: {} file_size: {}, state: {}",
      page_index, current_page_start, to_read, current_page_start + to_read,
      file_size, _file.rdstate());

  PageBuffer buffer = std::make_shared<Buffer<BufferContext>>(
      _page_size,
      BufferContext{._page = page_index, ._chunks_on_page = chunks_to_read});

  _file.seekg(current_page_start);
  _file.read(buffer->data(), to_read);

  _page_cache.insert(page_index, buffer);

  return buffer;
}

auto IndexLog::upper_bound(message_offset_t offset, std::int_fast64_t file_size)
    -> std::int_fast64_t {
  const std::int_fast64_t pages_available = (file_size / _page_size) + 1;
  const std::int_fast64_t chunks_available = this->chunks_available(file_size);

  std::int_fast64_t L = 0;
  std::int_fast64_t R = chunks_available - 1;

  while (L < R) {
    const std::int_fast64_t m = (L + R) / 2;
    const std::int_fast64_t page_index = m / _chunks_on_page;
    const std::int_fast64_t chunk_index_on_the_page =
        m - (page_index * _chunks_on_page);

    auto page_buffer = load_page_buffer(page_index, file_size);
    auto *index_chunk_ptr = reinterpret_cast<IndexChunk *>(page_buffer->data());

    auto chunk = index_chunk_ptr[chunk_index_on_the_page];

    if (offset >= chunk._offset) {
      L = m + 1;
    } else {
      R = m;
    }
  }

  std::int_fast64_t page_index = L / _chunks_on_page;
  std::int_fast64_t chunk_index_on_the_page =
      L - ((page_index)*_chunks_on_page);

  auto page_buffer = load_page_buffer(page_index, file_size);
  auto *index_chunk_ptr = reinterpret_cast<IndexChunk *>(page_buffer->data());

  if (L >= chunks_available) {
    return -1;
  }

  auto chunk_by_L = index_chunk_ptr[chunk_index_on_the_page];
  if (chunk_by_L._offset < offset) {
    return L + 1;
  }

  return L;
};

auto IndexLog::load(message_offset_t offset, std::size_t count)
    -> std::unique_ptr<std::vector<IndexChunk>> {
  if (offset < 0) {
    return std::make_unique<std::vector<IndexChunk>>();
  }

  _file.seekg(0, std::fstream::end);
  const std::int_fast64_t file_size = _file.tellg();

  const std::int_fast64_t pages_available = (file_size / _page_size) + 1;
  const std::int_fast64_t chunks_available = this->chunks_available(file_size);

  std::int_fast64_t chunk_index = upper_bound(offset, file_size);

  if (chunk_index < 0) {
    return std::make_unique<std::vector<IndexChunk>>();
  }

  auto result = std::make_unique<std::vector<IndexChunk>>();
  std::int_fast64_t result_size = 0;

  do {
    const std::int_fast64_t page_index = chunk_index / _chunks_on_page;
    const auto page_buffer = load_page_buffer(page_index, file_size);
    const auto page_buffer_context = page_buffer->context();
    const std::int_fast64_t chunk_index_on_the_page =
        chunk_index - (page_index * _chunks_on_page);
    const IndexChunk *chunks_ptr =
        reinterpret_cast<IndexChunk *>(page_buffer->data());

    for (std::int_fast64_t i = chunk_index_on_the_page;
         i < page_buffer_context._chunks_on_page && result_size < count; i++) {
      auto chunk = chunks_ptr[i];
      result->push_back(chunk);
      result_size++;
    }

    chunk_index = chunk_index + result_size;

  } while (result_size < count && chunk_index < chunks_available);

  return result;
};

} // namespace nmq