#include "topic.h"
#include <boost/log/trivial.hpp>
#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

nmq::Partition::Partition(std::string filename) : _filename(filename) {
  _file =
      std::ifstream(_filename, std::ifstream::in | std::ifstream::out |
                                   std::ifstream::app | std::ifstream::binary);
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

nmq::Topic::Topic(const std::string name, const uint64_t partitions) {
  for (uint64_t i = 0; i < partitions; i++) {
    auto partition_filename = name + "_" + std::to_string(partitions);
    this->partitions.push_back(
        std::make_unique<nmq::Partition>(partition_filename));
  }
};

nmq::Topic::~Topic(){};