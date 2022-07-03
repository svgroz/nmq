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
#include <string>

nmq::Topic::Topic(const std::string name, const uint64_t partitions) {
  for (uint64_t i = 0; i < partitions; i++) {
    auto partition_filename = name + "_" + std::to_string(partitions);

    auto _topic_file = std::make_unique<std::ifstream>(
        partition_filename, std::ifstream::in | std::ifstream::out |
                                std::ifstream::app | std::ifstream::binary);

    if (_topic_file->is_open()) {
      BOOST_LOG_TRIVIAL(debug) << "file " << partition_filename << " is opened";
    } else {
      BOOST_LOG_TRIVIAL(error) << "could not open file: " << partition_filename;
      throw "TODO"; // TODO
    }

    this->_topic_files.push_back(std::move(_topic_file));
  }
};

nmq::Topic::~Topic(){};