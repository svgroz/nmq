#include <cstdlib>
#include <fstream>
#include <nmq/core/partition_log.h>

#include <boost/log/trivial.hpp>
#include <mutex>
#include <stdexcept>

using namespace nmq;

PartitionLog::PartitionLog(const std::string &filename)
    : _log_file(filename, std::fstream::in | std::fstream::out |
                              std::fstream::app | std::fstream::binary) {

  if (_log_file.good()) {
    BOOST_LOG_TRIVIAL(debug) << "file is opened: " << filename;
  } else {
    BOOST_LOG_TRIVIAL(error) << "could not open file: " << filename;
    throw std::invalid_argument(filename);
  }
}

PartitionLog::~PartitionLog(){};

uint64_t PartitionLog::add(const proto::Message &message) {
  _log_file.seekg(0, _log_file.end);
  uint64_t offset = _log_file.tellg();
  message.SerializeToOstream(&_log_file);
  _log_file.sync();
  return offset;
}
