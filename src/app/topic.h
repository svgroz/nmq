#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace nmq {
class Topic {
private:
  std::vector<std::unique_ptr<std::ifstream>> _topic_files;

public:
  Topic(std::string name, uint64_t partitions);
  virtual ~Topic();
};

}; // namespace nmq