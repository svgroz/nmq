#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace nmq {
class Partition {
private:
  Partition(const Partition &) = delete;

public:
  Partition(std::string filename);
  virtual ~Partition();
  const std::string _filename;
  std::ifstream _file;
  std::mutex _mutex;
};

class Topic {
private:
  Topic(const Topic &) = delete;
  std::vector<std::unique_ptr<Partition>> partitions;

public:
  Topic(std::string name, uint64_t partitions);
  virtual ~Topic();
};

}; // namespace nmq