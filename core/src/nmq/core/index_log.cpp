#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <spdlog/spdlog.h>

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

IndexLog::IndexLog(std::string &filename, message_offset_t begin) {
  _begin = begin;
  _end = begin + INDEX_LOG_CHUNKS_COUNT;
  _filename = std::string(filename);
  bool file_exists = std::filesystem::exists(_filename);
  spdlog::debug("file exists: {} {}", _filename, file_exists);
  if (!file_exists) {
    spdlog::debug("trying to create file: {}", _filename);
    std::ofstream index_log_file(_filename,
                                 std::ofstream::app | std::ofstream::binary);
    index_log_file.close();
    std::filesystem::resize_file(_filename, INDEX_LOG_BUFFER_SIZE);
    spdlog::debug("file created with size: {} {}", _filename,
                  INDEX_LOG_BUFFER_SIZE);
  }

  _file_mapping = boost::interprocess::file_mapping(
      _filename.c_str(), boost::interprocess::read_write);
  _mapped_region = boost::interprocess::mapped_region(
      _file_mapping, boost::interprocess::read_write, 0, 0);
  _mapped_address = static_cast<IndexChunk *>(_mapped_region.get_address());
  if (!file_exists) {
    spdlog::debug("file initializing: {}", _filename);
    IndexChunk initial_chunk{._offset = -1, ._position = 0, ._message_size = 0};
    for (std::size_t i = 0; i < INDEX_LOG_CHUNKS_COUNT; i++) {
      _mapped_address[i] = initial_chunk;
    }
    spdlog::debug("file initialized: {}", _filename);
  }
};

auto IndexLog::set(IndexChunk &index_chunk) -> void {
  if (check_offset(_begin, index_chunk._offset, _end)) {
    _mapped_address[index_chunk._offset - _begin] = index_chunk;
  }
};

auto IndexLog::find(message_offset_t offset) -> std::optional<IndexChunk> {
  if (check_offset(_begin, offset, _end)) {
    auto chunk = _mapped_address[offset];

    if (chunk._offset == offset) {
      return std::optional(chunk);
    }
  }
  return std::nullopt;
};

} // namespace nmq