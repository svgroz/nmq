#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <sys/errno.h>
#include <tuple>
#include <vector>

#include <spdlog/spdlog.h>

#include <nmq/core/buffer.h>
#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <nmq/core/types.h>

namespace nmq {

inline constexpr std::int_fast64_t CHUNK_SIZE =
    static_cast<std::int_fast64_t>(sizeof(IndexChunk));

using BufferContext = struct {
  std::int_fast64_t _page;
  std::int_fast64_t _chunks_on_page;
};

IndexLog::IndexLog(std::string &filename, std::int_fast64_t page_size)
    : _filename(filename),
      _file(_filename, std::fstream::in | std::fstream::out |
                           std::fstream::binary | std::fstream::app),
      _page_size(page_size), _chunks_on_page(static_cast<std::int_fast64_t>(
                                 _page_size / sizeof(IndexChunk))),
      _chunks_on_page_raw_size(
          static_cast<std::int_fast64_t>(_chunks_on_page * sizeof(IndexChunk))),
      _page_tile_size(_page_size - _chunks_on_page_raw_size) {

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
    auto buffer_size = _page_tile_size + sizeof(IndexChunk);
    auto tail_buffer = Buffer<int>(buffer_size, 0);

    std::memcpy(tail_buffer.data() + _page_tile_size, &index_chunk,
                sizeof(IndexChunk));
    _file.write(tail_buffer.data(), tail_buffer.size());
  } else {
    _file.write(raw_chunk_ptr, sizeof(IndexChunk));
  }
};

auto load_page(std::int_fast64_t page_index, std::int_fast64_t page_size,
               Buffer<BufferContext> &buffer, std::fstream &file,
               std::int_fast64_t file_size) -> std::int_fast64_t {

  std::int_fast64_t current_page_start = page_index * page_size;
  std::int_fast64_t to_read = file_size - current_page_start >= page_size
                                  ? page_size
                                  : file_size - current_page_start;

  spdlog::info(
      "start load_page page_index: {} current_page_start: {} to_read: {} "
      "current_page_start+to_read: {} file_size: {}, state: {}",
      page_index, current_page_start, to_read, current_page_start + to_read,
      file_size, file.rdstate());

  file.seekg(current_page_start);
  file.read(buffer.data(), to_read);
  return to_read / CHUNK_SIZE;
}

auto refresh_buffer_if_necessery(Buffer<BufferContext> &page_buffer,
                                 std::int_fast64_t page_index,
                                 std::int_fast64_t page_size,
                                 std::fstream &file,
                                 std::int_fast64_t file_size) {
  if (page_buffer.context()._page != page_index) {
    auto chunks_on_page =
        load_page(page_index, page_size, page_buffer, file, file_size);
    page_buffer.set_context(
        {._page = page_index, ._chunks_on_page = chunks_on_page});
  }
}

auto IndexLog::load(message_offset_t offset, std::size_t count)
    -> std::unique_ptr<std::vector<IndexChunk>> {
  if (offset < 0) {
    return std::make_unique<std::vector<IndexChunk>>();
  }

  _file.seekg(0, std::fstream::end);
  const std::int_fast64_t file_size = _file.tellg();

  const std::int_fast64_t pages_available = (file_size / _page_size) + 1;
  const std::int_fast64_t last_page_size =
      file_size - ((pages_available - 1) * _page_size);
  const std::int_fast64_t chunks_available =
      ((pages_available - 1) * _chunks_on_page) + (last_page_size / CHUNK_SIZE);

  auto page_buffer = Buffer<BufferContext>(
      _page_size, BufferContext{._page = 0, ._chunks_on_page = 0});
  auto *index_chunk_ptr = reinterpret_cast<IndexChunk *>(page_buffer.data());

  std::int_fast64_t L = 0;
  std::int_fast64_t R = chunks_available - 1;
  std::int_fast64_t m = 0;
  while (L <= R) {
    m = (L + R) / 2;
    const std::int_fast64_t page_index = m / _chunks_on_page;
    const std::int_fast64_t chunk_index_on_the_page =
        m - (page_index * _chunks_on_page);

    refresh_buffer_if_necessery(page_buffer, page_index, _page_size, _file,
                                file_size);

    auto chunk = index_chunk_ptr[chunk_index_on_the_page];

    if (chunk._offset < offset) {
      L = m + 1;
    } else if (chunk._offset > offset) {
      R = m - 1;
    } else {
      break;
    }
  }

  auto result = std::make_unique<std::vector<IndexChunk>>();
  std::int_fast64_t result_size = 0;
  for (; result_size < count && m < chunks_available; m++) {
    const std::int_fast64_t page_index = m / _chunks_on_page;
    const std::int_fast64_t chunk_index_on_the_page =
        m - (page_index * _chunks_on_page);

    refresh_buffer_if_necessery(page_buffer, page_index, _page_size, _file,
                                file_size);

    result->push_back(index_chunk_ptr[chunk_index_on_the_page]);
    result_size++;
  }

  return result;
};

} // namespace nmq