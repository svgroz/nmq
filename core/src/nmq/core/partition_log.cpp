#include <cstdlib>
#include <fstream>
#include <nmq/core/exceptions.h>
#include <nmq/core/partition_log.h>

#include <spdlog/spdlog.h>

namespace nmq {
PartitionLog::PartitionLog(const std::string &filename) : _filename(filename) {
  spdlog::info("trying to open the file: {}", filename);
  _file = std::fstream(_filename, std::fstream::in | std::fstream::out |
                                      std::fstream::binary | std::fstream::app);
  if (_file.is_open()) {
    spdlog::info("file successfully opened: {}", _filename);
  } else {
    spdlog::error("could not open the file: {}", _filename);
    throw CouldNotOpenFile(_filename);
  }
};

PartitionLog::~PartitionLog() {
  spdlog::info("trying to close file: {}", _filename);
  _file.close();
  if (_file.is_open()) {
    spdlog::error("could not close file: {}", _filename);
  } else {
    spdlog::info("file successfully closed: {}", _filename);
  }
};

auto PartitionLog::push_back(Message &message) -> position_t {
  std::int64_t message_size = message.size();
  auto buffer = std::make_unique<std::vector<char>>(message_size);
  message.write(buffer->data(), message_size);
  _file.seekp(0, _file.end);
  std::streampos current_position = _file.tellp();
  _file.write(buffer->data(), message_size);
  return current_position + message_size;
}

auto PartitionLog::read(position_t position, size_t size) -> Message {
  auto buffer = std::make_unique<std::vector<char>>(size);
  _file.seekg(position);
  _file.read(buffer->data(), size);
  return read(buffer->data(), size);
}

} // namespace nmq
