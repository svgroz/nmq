#pragma once

#include <fstream>
#include <mutex>

namespace nmq {
class PartitionLog {
private:
  std::fstream _log_file;

public:
  explicit PartitionLog(const std::string &filename);
  PartitionLog(const PartitionLog &) = delete;
  virtual ~PartitionLog();
  auto add(const proto::Message &message) -> std::uint64_t;
};
} // namespace nmq