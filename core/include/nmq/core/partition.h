#pragma once

#include <cstdio>
#include <filesystem>
#include <nmq/core/message.h>
#include <nmq/core/partition_log.h>
#include <string>

namespace nmq {
class Partition {
private:
  PartitionLog _partition_log;
  std::mutex _partition_mutex;

public:
  explicit Partition(const std::filesystem::path &path);
  Partition(Partition &&) = delete;
  virtual ~Partition();
  auto add(Message &message) -> std::uint64_t;
};
} // namespace nmq