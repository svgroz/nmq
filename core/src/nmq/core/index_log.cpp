#include <nmq/core/exceptions.h>
#include <nmq/core/index_log.h>
#include <spdlog/spdlog.h>

namespace nmq {

IndexLog::IndexLog(std::string &filename) : _filename(filename) {
  _file = std::fstream(_filename, std::fstream::in | std::fstream::out |
                                      std::fstream::binary | std::fstream::app);
  if (_file.is_open()) {
    spdlog::info("file successfully opened: {}", _filename);
  } else {
    spdlog::error("could not open the file: {}", _filename);
    throw CouldNotOpenFile(_filename);
  }
};

IndexLog::~IndexLog() {
  spdlog::info("trying to close file: {}", _filename);
  _file.close();
  if (_file.is_open()) {
    spdlog::error("could not close file: {}", _filename);
  } else {
    spdlog::info("file successfully closed: {}", _filename);
  }
};

auto IndexLog::push_back(IndexChunk &index_chunk) -> void {
  auto buffer = std::array<char, IndexChunk::INDEX_CHUNK_RAW_SIZE>();
  index_chunk.write(buffer);
  _file.seekp(0, _file.end);
  _file.write(buffer.data(), IndexChunk::INDEX_CHUNK_RAW_SIZE);
};

auto IndexLog::find(message_offset_t offset) -> std::optional<IndexChunk> {
  auto offset_position_in_file = offset * IndexChunk::INDEX_CHUNK_RAW_SIZE;

  if (offset_position_in_file >
      (_file.tellp() - IndexChunk::INDEX_CHUNK_RAW_SIZE)) {
    return std::nullopt;
  }

  _file.seekp(offset_position_in_file, _file.beg);
  auto buffer = std::array<char, IndexChunk::INDEX_CHUNK_RAW_SIZE>();
  _file.read(buffer.data(), IndexChunk::INDEX_CHUNK_RAW_SIZE);

  return std::make_optional(IndexChunk::read(buffer));
};

} // namespace nmq