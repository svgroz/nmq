#include "topic.h"

#include <boost/log/trivial.hpp>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

void close_file_gratefully(std::fstream &file, std::mutex &file_mutex,
                           const std::string &filename) {
  try {
    std::lock_guard<std::mutex> guard(file_mutex);

    if (file.is_open()) {
      BOOST_LOG_TRIVIAL(debug) << "trying to close file: " << filename;
      file.close();
      if (file.is_open()) {
        BOOST_LOG_TRIVIAL(debug) << "could not close file: " << filename;
      } else {
        BOOST_LOG_TRIVIAL(debug) << "file successfully closed: " << filename;
      }
    } else {
      BOOST_LOG_TRIVIAL(warning) << "file is already closed: " << filename;
    }
    file.close();
  } catch (void *ex) {
    throw ex;
  }
}

nmq::Partition::Partition(std::string filename)
    : _message_log_filename(filename) {
  _message_log = std::fstream(_message_log_filename,
                              std::fstream::in | std::fstream::out |
                                  std::fstream::app | std::fstream::binary);
  if (_message_log.is_open()) {
    BOOST_LOG_TRIVIAL(debug) << "file is open: " << _message_log_filename;
  } else {
    BOOST_LOG_TRIVIAL(error)
        << "could not open file: " << _message_log_filename;
    throw "TODO"; // TODO
  }
};

nmq::Partition::~Partition() {
  close_file_gratefully(_message_log, _message_log_mutex,
                        _message_log_filename);
}

uint64_t nmq::Partition::add(const proto::Message &message) {
  try {
    std::lock_guard<std::mutex> guard(_message_log_mutex);

    _message_log.seekg(0, _message_log.end);
    uint64_t offset = _message_log.tellg();
    message.SerializeToOstream(&_message_log);
    _message_log.sync();
    return offset;
  } catch (void *ex) {
    throw ex;
  }
};

nmq::Topic::Topic(const std::string name, const uint64_t partitions)
    : _random_engine(std::default_random_engine(std::time(nullptr))),
      _distribution(0, partitions - 1),
      _partition_offset_log_filename(name + "_" + "partition_offset_log") {
  for (uint64_t i = 0; i < partitions; i++) {
    auto partition_filename = name + "_" + std::to_string(i);
    this->_partitions.push_back(
        std::make_unique<nmq::Partition>(partition_filename));
  }

  _partition_offset_log.open(_partition_offset_log_filename,
                             std::fstream::in | std::fstream::out |
                                 std::fstream::app | std::fstream::binary);
  if (_partition_offset_log.is_open()) {
    BOOST_LOG_TRIVIAL(debug)
        << "file is open: " << _partition_offset_log_filename;
  } else {
    BOOST_LOG_TRIVIAL(error)
        << "could not open file: " << _partition_offset_log_filename;
    throw "TODO"; // TODO
  }

  _partition_offset_log.seekg(0, _partition_offset_log.end);
};

nmq::Topic::~Topic() {
  close_file_gratefully(_partition_offset_log, _partition_offset_log_mutex,
                        _partition_offset_log_filename);
};

nmq::PartitionOffset nmq::Topic::add(const proto::Message &message) {
  const uint64_t partition_by_key =
      message.has_key()
          ? std::hash<std::string>{}(message.key()) % _partitions.size()
          : _distribution(_random_engine);

  uint64_t offset = _partitions.at(partition_by_key)->add(message);

  return nmq::PartitionOffset{partition_by_key, offset};
};