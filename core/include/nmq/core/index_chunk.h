#pragma once

#include <cstdint>
#include <ios>
#include <nmq/core/types.h>

namespace nmq {

class IndexChunk final {
private:
  const message_offset_t _offset;
  const message_position_t _position;
  const message_size_t _message_size;

public:
  static constexpr std::int64_t INDEX_CHUNK_RAW_SIZE =
      sizeof(_offset) + sizeof(_position) + sizeof(_message_size);

  IndexChunk(message_offset_t offset, message_position_t position,
             message_size_t message_size) noexcept
      : _offset(offset), _position(position), _message_size(message_size){};
  IndexChunk(const IndexChunk &) = default;
  IndexChunk(IndexChunk &&) = default;
  virtual ~IndexChunk() = default;

  static auto read(std::array<char, IndexChunk::INDEX_CHUNK_RAW_SIZE> &source)
      -> IndexChunk;
  auto write(std::array<char, IndexChunk::INDEX_CHUNK_RAW_SIZE> &target) -> void;

  auto offset() noexcept { return _offset; }

  auto position() noexcept { return _position; }

  auto message_size() noexcept { return _message_size; }
};
} // namespace nmq