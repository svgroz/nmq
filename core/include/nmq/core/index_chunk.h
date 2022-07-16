#pragma once

#include <cstdint>

namespace nmq {
using index_chunk_field = std::int64_t;
static constexpr std::size_t INDEX_CHUNK_SIZE = sizeof(index_chunk_field) * 3;

class IndexChunk final {
 private:
  const index_chunk_field _offset;
  const index_chunk_field _position;
  const index_chunk_field _size;

 public:
  IndexChunk(index_chunk_field offset, index_chunk_field position,
             index_chunk_field size) noexcept
      : _offset(offset), _position(position), _size(size){};
  IndexChunk(IndexChunk &&) = default;
  virtual ~IndexChunk() = default;

  static auto read(char *source, std::size_t size) -> IndexChunk;
  auto write(char *target, std::size_t size) -> void;

  auto offset() noexcept { return _offset; }

  auto position() noexcept { return _position; }

  auto size() noexcept { return _size; }
};
} // namespace nmq