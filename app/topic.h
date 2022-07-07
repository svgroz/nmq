#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <sys/_types/_ssize_t.h>
#include <vector>

#include "partition.h"
#include <nmq_server.pb.h>

namespace nmq {

typedef struct {
  uint64_t partiton;
  uint64_t offset;
} PartitionOffset;

class Topic {
private:
  Topic(const Topic &) = delete;
  std::default_random_engine _random_engine;
  std::uniform_int_distribution<uint64_t> _distribution;
  std::vector<std::unique_ptr<Partition>> _partitions;

public:
  Topic(const std::string &name, const uint64_t partitions);
  virtual ~Topic();
  PartitionOffset add(const proto::Message &message);
};

}; // namespace nmq