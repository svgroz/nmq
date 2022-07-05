#include "partition.h"
#include <boost/log/trivial.hpp>

#include "utils.h"

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
  nmq::close_file_gratefully(_message_log, _message_log_mutex,
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