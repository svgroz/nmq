#include "topic.h"
#include <_types/_uint64_t.h>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>

#include <boost/log/trivial.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

nmq::FileLock::FileLock(std::string filename) : _filename(filename) {
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

nmq::FileLock::~FileLock() {
  _mutex.lock();
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
  _mutex.unlock();
}

nmq::Topic::Topic(const std::string name, const uint64_t partitions)
    : _topics_lock_file(
          std::vector<std::unique_ptr<nmq::FileLock>>(partitions)) {
  for (uint64_t i = 0; i < partitions; i++) {
    auto partition_filename = name + "_" + std::to_string(partitions);
    _topics_lock_file.push_back(
        std::make_unique<nmq::FileLock>(partition_filename));
  }
};

nmq::Topic::~Topic(){};