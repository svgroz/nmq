#include <algorithm>
#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <ios>
#include <memory>
#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <spdlog/spdlog.h>

#include <boost/numeric/conversion/cast.hpp>
#include <filesystem>
#include <vector>

namespace nmq {

static constexpr std::int_fast64_t PAGE_SIZE =
    static_cast<std::int_fast64_t>(64 * 1024); // 64kb
static constexpr std::int_fast64_t CHUNKS_ON_PAGE =
    PAGE_SIZE / sizeof(IndexChunk);
static constexpr std::int_fast64_t PAGE_TAIL_SIZE =
    PAGE_SIZE - (CHUNKS_ON_PAGE * sizeof(IndexChunk));

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
  _file.write(raw_chunk_ptr, sizeof(index_chunk));
};

auto IndexLog::load(message_offset_t offset, std::size_t count)
    -> std::unique_ptr<std::vector<IndexChunk>> {

  std::int_fast64_t start = 0;
  _file.seekg(0, std::fstream::end);
  std::int_fast64_t end = _file.tellg();
  std::size_t pages_available = end / PAGE_SIZE;

  std::unique_ptr<std::array<char, PAGE_SIZE>> page_buffer =
      std::make_unique<std::array<char, PAGE_SIZE>>();

  for (std::int_fast64_t i = 0; i < end; i = i + PAGE_SIZE) {
    auto to_read = end - i - PAGE_SIZE > 0 ? PAGE_SIZE : end - i;

    _file.seekg(i);
    _file.read(page_buffer->data(), to_read);

    auto *index_chunk_ptr = reinterpret_cast<IndexChunk *>(page_buffer->data());
    std::size_t chunk_in_ptr = to_read / sizeof(IndexChunk);

    for (std::size_t j = 0; j < chunk_in_ptr; j++) {
      auto chunk = index_chunk_ptr[j];
      spdlog::info("loaded {} {} {}", chunk._offset, chunk._position, chunk._size);
    }
  }

  return std::make_unique<std::vector<IndexChunk>>();
};

} // namespace nmq