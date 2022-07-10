#include <filesystem>
#include <mutex>
#include <nmq/core/partition.h>

namespace nmq {
Partition::Partition(const std::filesystem::path &path)
    : _partition_log(path / "partition.log"), _partition_mutex(){};

Partition::~Partition() = default;

auto Partition::add(const Message &message) -> std::uint64_t {
  std::lock_guard<std::mutex> guard(_partition_mutex);
  return _partition_log.add(message);
};

} // namespace nmq
