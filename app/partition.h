#pragma once

#include <fstream>
#include <nmq_server.pb.h>
#include <string>

namespace nmq {
class Partition {
private:
  Partition(const Partition &) = delete;
  const std::string _message_log_filename;
  std::fstream _message_log;
  std::mutex _message_log_mutex;

public:
  Partition(std::string filename);
  virtual ~Partition();
  uint64_t add(const proto::Message &message);
};
} // namespace nmq