#include "topic.h"

#include <boost/log/trivial.hpp>

#include "utils.h"

nmq::Topic::Topic(const std::string name, const uint64_t partitions)
    : _random_engine(std::default_random_engine(std::time(nullptr))),
      _distribution(0, partitions - 1),
      _partition_offset_log_filename(name + "_" + "partition_offset_log") {
  for (uint64_t i = 0; i < partitions; i++) {
    auto partition_filename = name + "_" + std::to_string(i);
    this->_partitions.push_back(
        std::make_unique<nmq::Partition>(partition_filename));
  }

  _partition_offset_log.open(_partition_offset_log_filename,
                             std::fstream::in | std::fstream::out |
                                 std::fstream::app | std::fstream::binary);
  nmq::file_is_open_check(_partition_offset_log,
                          _partition_offset_log_filename);

  _partition_offset_log.seekg(0, _partition_offset_log.end);
};

nmq::Topic::~Topic() {
  close_file_gratefully(_partition_offset_log, _partition_offset_log_mutex,
                        _partition_offset_log_filename);
};

nmq::PartitionOffset nmq::Topic::add(const proto::Message &message) {
  const uint64_t partition_by_key =
      message.has_key()
          ? std::hash<std::string>{}(message.key()) % _partitions.size()
          : _distribution(_random_engine);

  uint64_t offset = _partitions.at(partition_by_key)->add(message);

  return nmq::PartitionOffset{partition_by_key, offset};
};