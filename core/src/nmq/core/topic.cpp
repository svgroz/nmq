#include <cstddef>
#include <filesystem>
#include <fstream>
#include <nmq/core/topic.h>

#include <boost/log/trivial.hpp>

namespace nmq {
Topic::Topic(const std::filesystem::path &path, const std::string &name,
             const std::size_t partitions)
    : _random_engine(std::default_random_engine(std::time(nullptr))),
      _distribution(0, partitions - 1), _partitions(partitions - 1) {

  for (std::size_t i = 0; i < partitions; i++) {
    _partitions.push_back(std::make_unique<nmq::Partition>(
        path / (name + "_" + std::to_string(i))));
  }
  
};

Topic::~Topic() = default;

auto Topic::add(const proto::Message &message) -> PartitionOffset {
  uint64_t partition_by_key;
  if (message.has_key()) {
    partition_by_key = _key_hasher(message.key()) % _partitions.size();
  } else {
    partition_by_key 
    
    
    = _distribution(_random_engine);
  }

  uint64_t offset = _partitions.at(partition_by_key)->add(message);

  return PartitionOffset{partition_by_key, offset};
};

} // namespace nmq
