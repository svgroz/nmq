#include <cstddef>
#include <filesystem>
#include <fstream>
#include <nmq/core/topic.h>

namespace nmq {
Topic::Topic(const std::filesystem::path &path, const std::string &name,
             const std::size_t partitions)
    : _random_engine(std::default_random_engine(std::time(nullptr))),
      _distribution(0, partitions - 1), _partitions(partitions - 1) {

  for (std::size_t i = 0; i < partitions; i++) {
    _partitions.push_back(std::make_unique<partition::Partition>(
        path / (name + "_" + std::to_string(i))));
  }
};

auto Topic::add(message::Message &message) -> PartitionOffset {
  (void)_key_hasher;
  std::uint64_t partition_by_key = _distribution(_random_engine);

  std::uint64_t offset = _partitions.at(partition_by_key)->push_back(message);

  return PartitionOffset{partition_by_key, offset};
};

} // namespace nmq
