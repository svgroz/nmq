#include <cstdlib>
#include <fstream>
#include <nmq/core/partition_log.h>

#include <mutex>
#include <stdexcept>

namespace nmq {
PartitionLog::PartitionLog(const std::string &filename)
    : _log_file(filename, std::fstream::in | std::fstream::out |
                              std::fstream::app | std::fstream::binary) {
  if (!_log_file.good()) {
    throw std::invalid_argument("could not open file: " + filename);
  }
}

PartitionLog::~PartitionLog() = default;

auto PartitionLog::add(const Message &message) -> std::uint64_t {
  (void)message;
  // TODO
  return 0;
}

} // namespace nmq
