#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace nmq {
class TopicConfiguration {
private:
  std::string _name;
  std::size_t _partitions;

public:
  TopicConfiguration(const std::string &name, std::size_t partitions);
  virtual ~TopicConfiguration();
  auto name() -> std::string;
  auto partitions() -> std::size_t;
};

class Configuration {
public:
  Configuration();
  virtual ~Configuration();
  auto topics() -> std::vector<TopicConfiguration>;
};
}; // namespace nmq