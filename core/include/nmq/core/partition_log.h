#pragma once

#include <fstream>

#include <nmq/core/message.h>

namespace nmq {
class PartitionLog final {
 private:
  std::string _filename;
  std::fstream _file;

 public:
  explicit PartitionLog(const std::string &filename);
  PartitionLog(PartitionLog &&) = default;
  virtual ~PartitionLog();
  auto push_back(Message &message) -> std::uint64_t;
  auto read(std::size_t position, std::size_t size) -> Message;
};
} // namespace nmq