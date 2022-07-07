#include "partition.h"
#include <boost/log/trivial.hpp>
#include <mutex>

using namespace nmq;

Partition::Partition(const std::string &filename)
    : _partition_log(filename), _partition_mutex(){};

nmq::Partition::~Partition(){};

uint64_t nmq::Partition::add(const proto::Message &message) {
  std::lock_guard<std::mutex> guard(_partition_mutex);
  return _partition_log.add(message);
};
