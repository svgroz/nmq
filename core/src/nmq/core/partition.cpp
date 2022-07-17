#include <filesystem>
#include <mutex>
#include <nmq/core/partition.h>

namespace nmq::partition {
Partition::Partition(const std::filesystem::path &path)
    : _partition_log(path / "partition.log"), _partition_mutex(){};

auto Partition::push_back(Message &message) -> std::uint64_t {
  std::lock_guard<std::mutex> guard(_partition_mutex);
  return _partition_log.push_back(message);
};

} // namespace nmq::partition
