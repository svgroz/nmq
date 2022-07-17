#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <nmq/core/message.h>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include <nmq/core/partition.h>

namespace nmq {

using PartitionOffset = struct {
  uint64_t partiton;
  uint64_t offset;
};

class Topic {
 private:
  std::hash<std::string_view> _key_hasher;
  std::default_random_engine _random_engine;
  std::uniform_int_distribution<uint64_t> _distribution;
  std::vector<std::unique_ptr<partition::Partition>> _partitions;

 public:
  Topic(const std::filesystem::path &path, const std::string &name,
        const std::size_t partitions);
  Topic(const Topic &) = delete;
  virtual ~Topic() = default;
  auto add(message::Message &message) -> PartitionOffset;
};

}; // namespace nmq