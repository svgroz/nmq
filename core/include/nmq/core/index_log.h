#pragma once

#include <fstream>
#include <memory>
#include <nmq/core/types.h>
#include <optional>
#include <vector>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

namespace nmq {

using IndexChunk = struct {
  message_offset_t _offset;
  message_position_t _position;
  message_size_t _message_size;
};

class IndexLog final {
private:
  std::string _filename;
  message_offset_t _begin;
  message_offset_t _end;
  boost::interprocess::file_mapping _file_mapping;
  boost::interprocess::mapped_region _mapped_region;
  IndexChunk *_mapped_address;

public:
  IndexLog(std::string &filename, message_offset_t begin);
  IndexLog(IndexLog &&) = delete;
  virtual ~IndexLog() = default;
  auto set(IndexChunk &index_chunk) -> void;
  auto find(message_offset_t offset) -> std::optional<IndexChunk>;
  auto begin() noexcept { return _begin; };
  auto end() noexcept { return _end; };
};

} // namespace nmq