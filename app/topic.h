#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#include <nmq_server.pb.h>

namespace nmq {

typedef struct {
  uint64_t partiton;
  uint64_t offset;
} PartitionOffset;

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

class Topic {
private:
  Topic(const Topic &) = delete;
  std::default_random_engine _random_engine;
  std::uniform_int_distribution<uint64_t> _distribution;
  std::vector<std::unique_ptr<Partition>> _partitions;
  const std::string _partition_offset_log_filename;
  std::fstream _partition_offset_log;
  std::mutex _partition_offset_log_mutex;

public:
  Topic(std::string name, uint64_t partitions);
  virtual ~Topic();
  PartitionOffset add(const proto::Message &message);
};

}; // namespace nmq