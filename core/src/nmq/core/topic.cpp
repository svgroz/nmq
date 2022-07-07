#include <nmq/core/topic.h>

#include <boost/log/trivial.hpp>

nmq::Topic::Topic(const std::string &name, const uint64_t partitions)
    : _random_engine(std::default_random_engine(std::time(nullptr))),
      _distribution(0, partitions - 1) {
  for (uint64_t i = 0; i < partitions; i++) {
    auto partition_filename = name + "_" + std::to_string(i);
    this->_partitions.push_back(
        std::make_unique<nmq::Partition>(partition_filename));
  }
};

nmq::Topic::~Topic(){};

nmq::PartitionOffset nmq::Topic::add(const proto::Message &message) {
  uint64_t partition_by_key;
  if (message.has_key()) {
    partition_by_key =
        std::hash<std::string>{}(message.key()) % _partitions.size();
  } else {
    partition_by_key = _distribution(_random_engine);
  }

  uint64_t offset = _partitions.at(partition_by_key)->add(message);

  return nmq::PartitionOffset{partition_by_key, offset};
};
