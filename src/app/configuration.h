#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace nmq {
class TopicConfiguration {
 private:
  std::string _name;
  uint64_t _partitions;

 public:
  TopicConfiguration(std::string name, uint64_t partitions)
      : _name(name), _partitions(partitions){};
  virtual ~TopicConfiguration(){};
  std::string name() { return _name; };
  uint64_t partitions() { return _partitions; };
};

class Configuration {
 public:
  Configuration(){};
  virtual ~Configuration(){};
  std::vector<TopicConfiguration> topics();
};
};  // namespace nmq