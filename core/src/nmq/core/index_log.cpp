#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <spdlog/spdlog.h>

#include <boost/numeric/conversion/cast.hpp>
#include <filesystem>

namespace nmq {

static constexpr std::size_t MAX_RAW_SIZE =
    static_cast<std::size_t>(32 * 1024 * 1024);
static constexpr std::size_t INDEX_LOG_BUFFER_SIZE =
    MAX_RAW_SIZE - (MAX_RAW_SIZE % sizeof(IndexChunk));
static constexpr std::size_t INDEX_LOG_CHUNKS_COUNT =
    INDEX_LOG_BUFFER_SIZE / sizeof(IndexChunk);

auto check_offset(message_offset_t begin, message_offset_t offset,
                  message_offset_t end) {
  return begin <= offset <= end;
}

IndexLog::IndexLog(std::string &filename, message_offset_t begin)
    : _filename(filename),
      _file(_filename, std::fstream::in | std::fstream::out |
                           std::fstream::binary | std::fstream::app |
                           std::fstream::ate) {
  _begin = begin;
  _end = boost::numeric_cast<message_offset_t>(_begin + INDEX_LOG_CHUNKS_COUNT);
  if (_file.good()) {
    spdlog::info("file is opened: {}", _filename);
  } else {
    auto error_code = errno;
    spdlog::error("could not open file: {} {}:{}", _filename,
                  std::strerror(error_code), error_code);
    throw SystemError(error_code);
  }
};

auto IndexLog::push_back(IndexChunk &index_chunk) -> bool {
  if (!check_offset(_begin, index_chunk._offset, _end)) {
    return false;
  }

  auto buffer = std::array<char, sizeof(IndexChunk)>();
  std::memcpy(buffer.data(), &index_chunk, sizeof(index_chunk));
  _file.write(buffer.data(), buffer.size());
  auto error_code = errno;
  return true;
};

auto IndexLog::find(message_offset_t offset) -> IndexChunk {
  return {._offset = -1};
};

} // namespace nmq