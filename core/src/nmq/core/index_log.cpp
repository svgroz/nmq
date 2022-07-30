#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <tuple>
#include <vector>

#include <spdlog/spdlog.h>

#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <nmq/core/types.h>

namespace nmq {

inline constexpr std::int_fast64_t PAGE_SIZE =
    // static_cast<std::int_fast64_t>(64 * 1024); // 64kb
    static_cast<std::int_fast64_t>((sizeof(IndexChunk) * 3) +
                                   1); // for test purposes only
inline constexpr std::int_fast64_t CHUNKS_ON_PAGE =
    PAGE_SIZE / sizeof(IndexChunk);
inline constexpr std::int_fast64_t CHUNKS_ON_PAGE_RAW_SIZE =
    CHUNKS_ON_PAGE * sizeof(IndexChunk);
inline constexpr std::int_fast64_t PAGE_TILE_SIZE =
    PAGE_SIZE - CHUNKS_ON_PAGE_RAW_SIZE;
inline constexpr std::int_fast64_t NOT_FOUND = -1;

IndexLog::IndexLog(std::string &filename)
    : _filename(filename),
      _file(_filename, std::fstream::in | std::fstream::out |
                           std::fstream::binary | std::fstream::app) {

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

  auto current_position_on_page = end % PAGE_SIZE;
  if (current_position_on_page >= CHUNKS_ON_PAGE_RAW_SIZE) {
    auto tail_buffer = std::array<char, PAGE_TILE_SIZE + sizeof(IndexChunk)>();
    std::memcpy(tail_buffer.data() + PAGE_TILE_SIZE, &index_chunk,
                sizeof(IndexChunk));
    _file.write(tail_buffer.data(), tail_buffer.size());
  } else {
    _file.write(raw_chunk_ptr, sizeof(IndexChunk));
  }
};

auto search_on_page(const IndexChunk *page, const std::size_t page_size,
                    const message_offset_t offset)
    -> std::tuple<std::int_fast32_t, std::int_fast64_t> {
  if (offset < page[0]._offset) {
    return std::make_tuple(-1, NOT_FOUND);
  }

  if (offset > page[page_size - 1]._offset) {
    return std::make_tuple(1, NOT_FOUND);
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
      return std::make_pair(0, m);
    }
  }

  return std::make_pair(0, m);
};

auto load_page(std::int_fast64_t page_index, char *buffer, std::fstream &file,
               std::int_fast64_t file_size) -> std::int_fast64_t {
  std::int_fast64_t current_page_start = page_index * PAGE_SIZE;
  std::int_fast64_t to_read = file_size - current_page_start > 0
                                  ? PAGE_SIZE - 1
                                  : file_size - current_page_start;
  file.seekg(current_page_start);
  file.read(buffer, to_read);
  return to_read / static_cast<std::int_fast64_t>(sizeof(IndexChunk));
}

auto find_page_with_offset(message_offset_t offset, std::fstream &file,
                           const std::int_fast64_t file_size, char *buffer,
                           IndexChunk *index_chunk_ptr)
    -> std::tuple<std::int_fast64_t, std::int_fast64_t, std::int_fast64_t> {

  std::int_fast64_t left_index = 0;
  std::int_fast64_t right_index = file_size / PAGE_SIZE;

  do {
    std::int_fast64_t current_page_index = (left_index + right_index) / 2;
    std::int_fast64_t chunks_in_ptr =
        load_page(current_page_index, buffer, file, file_size);
    auto [found, index] =
        search_on_page(index_chunk_ptr, chunks_in_ptr, offset);
    if (found > 0) {
      left_index = current_page_index + 1;
    } else if (found < 0) {
      right_index = current_page_index - 1;
    } else {
      return std::make_tuple(current_page_index, index, chunks_in_ptr);
    }
  } while (left_index <= right_index);
  return std::make_tuple(NOT_FOUND, NOT_FOUND, NOT_FOUND);
};

auto IndexLog::load(message_offset_t offset, std::size_t count)
    -> std::unique_ptr<std::vector<IndexChunk>> {

  _file.seekg(0, std::fstream::end);
  const std::int_fast64_t end = _file.tellg();

  const std::int_fast64_t pages_available = (end / PAGE_SIZE) + 1;

  std::unique_ptr<std::array<char, PAGE_SIZE>> page_buffer =
      std::make_unique<std::array<char, PAGE_SIZE>>();
  auto *index_chunk_ptr = reinterpret_cast<IndexChunk *>(page_buffer->data());

  auto [page_numer, chunk_index, chunks_on_page] = find_page_with_offset(
      offset, _file, end, page_buffer->data(), index_chunk_ptr);

  if (page_numer <= NOT_FOUND) {
    return std::make_unique<std::vector<IndexChunk>>();
  }

  auto result = std::make_unique<std::vector<IndexChunk>>();

  auto reserve_size =
      page_numer == pages_available - 1 ? chunks_on_page - chunk_index : count;

  result->reserve(reserve_size);
  std::size_t result_size = 0;

  do {
    for (auto i = chunk_index; i < chunks_on_page; i++) {
      result->push_back(index_chunk_ptr[i]);
      result_size++;
      if (result_size == count) {
        return result;
      }
    }

    if (page_numer == pages_available - 1) {
      return result;
    }

    page_numer = page_numer + 1;
    chunk_index = 0;
    chunks_on_page = load_page(page_numer, page_buffer->data(), _file, end);
  } while (page_numer < pages_available);

  return result;
};

} // namespace nmq