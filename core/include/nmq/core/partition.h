#pragma once

#include <cstdio>
#include <filesystem>
#include <nmq/core/message.h>
#include <nmq/core/partition_log.h>
#include <string>

namespace nmq::partition {
class Partition {
 private:
  partition_log::PartitionLog _partition_log;
  std::mutex _partition_mutex;

 public:
  explicit Partition(const std::filesystem::path &path);
  Partition(Partition &&) = delete;
  virtual ~Partition() = default;
  auto push_back(message::Message &message) -> index_chunk::position_t;
};
} // namespace nmq::partition