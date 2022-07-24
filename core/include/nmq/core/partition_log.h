#pragma once

#include <fstream>

#include <nmq/core/message.h>
#include <nmq/core/types.h>

namespace nmq {
class PartitionLog final {
private:
  std::string _filename;
  std::fstream _file;

public:
  explicit PartitionLog(const std::string &filename);
  PartitionLog(PartitionLog &&) = delete;
  virtual ~PartitionLog();
  auto push_back(Message &message) -> message_position_t;
  auto read(message_position_t position, size_t size) -> Message;
};
} // namespace nmq