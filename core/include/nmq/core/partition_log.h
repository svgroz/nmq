#pragma once

#include <cstdint>
#include <fstream>
#include <mutex>
#include <nmq_server.pb.h>

namespace nmq {
class PartitionLog {
private:
  PartitionLog(const PartitionLog &) = delete;
  std::fstream _log_file;

public:
  PartitionLog(const std::string &filename);
  virtual ~PartitionLog();
  std::uint64_t add(const proto::Message &message);
};
} // namespace nmq