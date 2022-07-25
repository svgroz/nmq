#include <algorithm>
#include <array>
#include <cerrno>
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
static constexpr std::int_fast64_t CHUNKS_IN_PAGE =
    PAGE_SIZE / IndexChunk::MIN_BUFFER_SIZE;
static constexpr std::int_fast64_t PAGE_TAIL_SIZE =
    PAGE_SIZE - (CHUNKS_IN_PAGE * IndexChunk::MIN_BUFFER_SIZE);

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

auto IndexLog::push_back(IndexChunk &index_chunk) -> void {
  auto buffer = std::array<char, IndexChunk::MIN_BUFFER_SIZE>();
  index_chunk.write(buffer.data(), IndexChunk::MIN_BUFFER_SIZE);
  _file.write(buffer.data(), buffer.size());
};

auto IndexLog::load(message_offset_t offset, std::size_t count)
    -> std::unique_ptr<std::vector<IndexChunk>> {

  std::int_fast64_t start = 0;
  _file.seekg(0, std::fstream::end);
  std::int_fast64_t end = _file.tellg();

  std::unique_ptr<std::vector<char>> page_buffer =
      std::make_unique<std::vector<char>>();
  std::unique_ptr<std::vector<IndexChunk>> chunks_buffer =
      std::make_unique<std::vector<IndexChunk>>();

  for (std::int_fast64_t i = 0; i < end; i = i + PAGE_SIZE) {
    auto buffer_size = end - i - PAGE_SIZE > 0 ? PAGE_SIZE : end - i;
    if (page_buffer->capacity() != buffer_size) {
      page_buffer->resize(buffer_size);
      chunks_buffer->resize(page_buffer->capacity() /
                            IndexChunk::MIN_BUFFER_SIZE);
    }

    _file.seekg(i);
    _file.read(page_buffer->data(), page_buffer->capacity());

    for (std::size_t j = 0; j < page_buffer->capacity();
         j = j + IndexChunk::MIN_BUFFER_SIZE) {
      chunks_buffer->push_back(IndexChunk::read(page_buffer->data() + j,
                                                IndexChunk::MIN_BUFFER_SIZE));
    }
  }

  return std::make_unique<std::vector<IndexChunk>>();
};

} // namespace nmq