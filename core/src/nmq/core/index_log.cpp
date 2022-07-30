#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <tuple>
#include <vector>

#include <spdlog/spdlog.h>

#include <nmq/core/buffer.h>
#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <nmq/core/types.h>

namespace nmq {

using SearchResult = enum search_result : std::int_fast8_t {
  LEFT,
  RIGHT,
  FOUND
};

inline constexpr std::int_fast64_t NOT_FOUND = -1;

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
    auto tail_buffer = Buffer(buffer_size);

    std::memcpy(tail_buffer.data() + _page_tile_size, &index_chunk,
                sizeof(IndexChunk));
    _file.write(tail_buffer.data(), tail_buffer.size());
  } else {
    _file.write(raw_chunk_ptr, sizeof(IndexChunk));
  }
};

auto load_page(std::int_fast64_t page_index, Buffer &buffer, std::fstream &file,
               std::int_fast64_t file_size, std::int_fast64_t page_size)
    -> std::int_fast64_t {

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
  return to_read / static_cast<std::int_fast64_t>(sizeof(IndexChunk));
}

auto search_on_page(const IndexChunk *page, const std::size_t page_size,
                    const message_offset_t offset)
    -> std::tuple<SearchResult, std::int_fast64_t> {
  if (offset < page[0]._offset) {
    return std::make_pair(SearchResult::LEFT, NOT_FOUND);
  }

  if (offset > page[page_size - 1]._offset) {
    return std::make_pair(SearchResult::RIGHT, NOT_FOUND);
  }

  std::size_t L = 0;
  std::size_t R = page_size - 1;
  std::size_t m = 0;

  while (L <= R) {
    m = (L + R) / 2;
    if (page[m]._offset < offset) {
      L = m + 1;
    } else if (page[m]._offset > offset) {
      R = m - 1;
    } else {
      return std::make_pair(SearchResult::FOUND, m);
    }
  }

  return std::make_pair(SearchResult::FOUND, m);
};

auto find_page_with_offset(message_offset_t offset, std::fstream &file,
                           const std::int_fast64_t file_size, Buffer &buffer,
                           IndexChunk *index_chunk_ptr,
                           std::int_fast64_t page_size)
    -> std::tuple<std::int_fast64_t, std::int_fast64_t, std::int_fast64_t> {

  std::int_fast64_t left_index = 0;
  std::int_fast64_t right_index = file_size / page_size;

  do {
    std::int_fast64_t current_page_index = (left_index + right_index) / 2;
    std::int_fast64_t chunks_in_ptr =
        load_page(current_page_index, buffer, file, file_size, page_size);
    auto [found, index] =
        search_on_page(index_chunk_ptr, chunks_in_ptr, offset);

    switch (found) {
    case RIGHT: {
      left_index = current_page_index + 1;
      break;
    };
    case LEFT: {
      right_index = current_page_index - 1;
      break;
    };
    default: {
      return std::make_tuple(current_page_index, index, chunks_in_ptr);
    };
    }
  } while (left_index <= right_index);
  return std::make_tuple(NOT_FOUND, NOT_FOUND, NOT_FOUND);
};

auto IndexLog::load(message_offset_t offset, std::size_t count)
    -> std::unique_ptr<std::vector<IndexChunk>> {

  _file.seekg(0, std::fstream::end);
  const std::int_fast64_t end = _file.tellg();

  const std::int_fast64_t pages_available = (end / _page_size) + 1;

  auto page_buffer = Buffer(_page_size);
  auto *index_chunk_ptr = reinterpret_cast<IndexChunk *>(page_buffer.data());

  auto [page_index, chunk_index, chunks_on_page] = find_page_with_offset(
      offset, _file, end, page_buffer, index_chunk_ptr, _page_size);

  if (page_index <= NOT_FOUND) {
    return std::make_unique<std::vector<IndexChunk>>();
  }

  auto result = std::make_unique<std::vector<IndexChunk>>();

  auto reserve_size =
      page_index == pages_available - 1 ? chunks_on_page - chunk_index : count;

  result->reserve(reserve_size);
  std::size_t result_size = 0;

  do {
    for (auto i = chunk_index; i < chunks_on_page; i++) {
      if (result_size == count) {
        return result;
      }
      result->push_back(index_chunk_ptr[i]);
      result_size++;
    }

    if (page_index == pages_available - 1) {
      return result;
    }

    page_index = page_index + 1;
    chunk_index = 0;
    chunks_on_page = load_page(page_index, page_buffer, _file, end, _page_size);
  } while (page_index < pages_available);

  return result;
};

} // namespace nmq