#pragma once

#include "partition_log.h"
#include <fstream>
#include <proto/nmq_server.pb.h>
#include <string>

namespace nmq {
class Partition {
private:
  Partition(const Partition &) = delete;
  PartitionLog _partition_log;
  std::mutex _partition_mutex;
public:
  Partition(const std::string &filename);
  virtual ~Partition();
  uint64_t add(const proto::Message &message);
};
} // namespace nmq