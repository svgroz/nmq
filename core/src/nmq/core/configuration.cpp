#include <nmq/core/configuration.h>

#include <string>
#include <vector>

namespace nmq {

TopicConfiguration::TopicConfiguration(const std::string &name,
                                       std::size_t partitions)
    : _name(name), _partitions(partitions){};

TopicConfiguration::~TopicConfiguration() = default;

auto TopicConfiguration::name() -> std::string { return _name; }

auto TopicConfiguration::partitions() -> std::size_t { return _partitions; }

Configuration::Configuration() = default;

Configuration::~Configuration() = default;

auto Configuration::topics() -> std::vector<nmq::TopicConfiguration> {
  auto result = std::vector<nmq::TopicConfiguration>();
  result.emplace_back("foo", 1);
  result.emplace_back("bar", 2);
  return result;
};

} // namespace nmq
