#include "topic.h"
#include <_types/_uint64_t.h>
#include <boost/log/trivial.hpp>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <ios>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

nmq::Partition::Partition(std::string filename) : _filename(filename) {
  _file = std::fstream(_filename, std::fstream::in | std::fstream::out |
                                      std::fstream::app | std::fstream::binary);
  if (_file.is_open()) {
    BOOST_LOG_TRIVIAL(debug) << "file is open: " << _filename;
  } else {
    BOOST_LOG_TRIVIAL(error) << "could not open file: " << _filename;
    throw "TODO"; // TODO
  }
};

nmq::Partition::~Partition() {
  try {
    std::lock_guard<std::mutex> guard(_mutex);

    if (_file.is_open()) {
      BOOST_LOG_TRIVIAL(debug) << "trying to close file: " << _filename;
      _file.close();
      if (_file.is_open()) {
        BOOST_LOG_TRIVIAL(debug) << "could not close file: " << _filename;
      } else {
        BOOST_LOG_TRIVIAL(debug) << "file successfully closed: " << _filename;
      }
    } else {
      BOOST_LOG_TRIVIAL(warning) << "file is already closed: " << _filename;
    }
    _file.close();
  } catch (...) {
  }
}

uint64_t nmq::Partition::add(const proto::Message &message) {
  try {
    std::lock_guard<std::mutex> guard(_mutex);

    _file.seekg(0, _file.end);
    uint64_t offset = _file.tellg();
    message.SerializeToOstream(&_file);
    _file.sync();
    return offset;
  } catch (void *ex) {
    throw ex;
  }
};

nmq::Topic::Topic(const std::string name, const uint64_t partitions)
    : _random_engine(std::default_random_engine(std::time(nullptr))),
      _distribution(0, partitions - 1) {
  for (uint64_t i = 0; i < partitions; i++) {
    auto partition_filename = name + "_" + std::to_string(partitions);
    this->_partitions.push_back(
        std::make_unique<nmq::Partition>(partition_filename));
  }
};

nmq::Topic::~Topic(){};

nmq::PartitionOffset nmq::Topic::add(const proto::Message &message) {
  const uint64_t partition_by_key =
      message.has_key() ? std::hash<std::string>{}(message.key())
                        : _distribution(_random_engine);

  uint64_t offset = _partitions.at(partition_by_key)->add(message);
  return nmq::PartitionOffset{partition_by_key, offset};
};