#pragma once

#include <fstream>

#include <nmq/core/index_chunk.h>
#include <nmq/core/message.h>

namespace nmq::partition_log {
class PartitionLog final {
 private:
  std::string _filename;
  std::fstream _file;

 public:
  explicit PartitionLog(const std::string &filename);
  PartitionLog(PartitionLog &&) = delete;
  virtual ~PartitionLog();
  auto push_back(Message &message) -> index_chunk::position_t;
  auto read(index_chunk::position_t position, index_chunk::size_t size)
      -> Message;
};
} // namespace nmq::partition_log