#pragma once

#include <fstream>
#include <memory>
#include <nmq/core/types.h>
#include <optional>
#include <vector>

namespace nmq {

using IndexChunk = struct {
  message_offset_t _offset;
  message_position_t _position;
  message_size_t _size;
};

class IndexLog final {
private:
  std::string _filename;
  message_offset_t _begin;
  message_offset_t _end;
  std::fstream _file;

public:
  IndexLog(std::string &filename, message_offset_t begin);
  IndexLog(IndexLog &&) = delete;
  virtual ~IndexLog() = default;
  auto push_back(IndexChunk &index_chunk) -> bool;
  auto find(message_offset_t offset) -> IndexChunk;
  auto begin() noexcept { return _begin; };
  auto end() noexcept { return _end; };
};

} // namespace nmq