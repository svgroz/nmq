#include "nmq/core/types.h"
#include <array>
#include <asm-generic/errno.h>
#include <boost/numeric/conversion/cast.hpp>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <spdlog/spdlog.h>
#include <type_traits>
#include <utility>
#include <vector>

namespace nmq {

constexpr std::int_fast64_t PAGE_SIZE =
    // static_cast<std::int_fast64_t>(64 * 1024); // 64kb
    static_cast<std::int_fast64_t>((sizeof(IndexChunk) * 3) +
                                   1); // for test purposes only
constexpr std::int_fast64_t CHUNKS_ON_PAGE = PAGE_SIZE / sizeof(IndexChunk);
constexpr std::int_fast64_t CHUNKS_ON_PAGE_RAW_SIZE =
    CHUNKS_ON_PAGE * sizeof(IndexChunk);
constexpr std::int_fast64_t NOT_FOUND_ON_PAGE = CHUNKS_ON_PAGE + 1;

IndexLog::IndexLog(std::string &filename)
    : _filename(filename),
      _file(_filename, std::fstream::in | std::fstream::out |
                           std::fstream::binary | std::fstream::app |
                           std::fstream::ate) {

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
    auto tail_buffer = std::vector<char>(PAGE_SIZE - current_position_on_page);
    _file.write(tail_buffer.data(), tail_buffer.capacity());
  }
  _file.write(raw_chunk_ptr, sizeof(IndexChunk));
};

auto search_on_page(const IndexChunk *page, const std::size_t page_size,
                    const message_offset_t offset)
    -> std::pair<int, std::int_fast64_t> {
  if (offset < page[0]._offset) {
    return std::make_pair(-1, NOT_FOUND_ON_PAGE);
  }

  if (offset > page[page_size - 1]._offset) {
    return std::make_pair(1, NOT_FOUND_ON_PAGE);
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

auto IndexLog::load(message_offset_t offset, std::size_t count)
    -> std::unique_ptr<std::vector<IndexChunk>> {

  std::int_fast64_t start = 0;
  _file.seekg(0, std::fstream::end);
  std::int_fast64_t end = _file.tellg();
  std::size_t pages_available = end / PAGE_SIZE;

  std::unique_ptr<std::array<char, PAGE_SIZE>> page_buffer =
      std::make_unique<std::array<char, PAGE_SIZE>>();
  auto *index_chunk_ptr = reinterpret_cast<IndexChunk *>(page_buffer->data());
  
  std::size_t L = 0;
  std::size_t R = pages_available;
  std::size_t m = 0;

  do {
    m = (L + R) / 2;
    std::int_fast64_t i = m * PAGE_SIZE;
    std::int_fast64_t to_read = end - i > 0 ? PAGE_SIZE : end - i;
    std::int_fast64_t chunk_in_ptr = to_read / sizeof(IndexChunk);

    _file.seekg(i);
    _file.read(page_buffer->data(), to_read);

    auto search_result = search_on_page(index_chunk_ptr, chunk_in_ptr, offset);
    if (search_result.first > 0) {
      L = m + 1;
    } else if (search_result.first < 0) {
      R = m - 1;
    } else {
      break;
    }
  } while (L <= R);

  return std::make_unique<std::vector<IndexChunk>>();
};

} // namespace nmq