#pragma once

#include <cstdint>
#include <ios>

namespace nmq::index_chunk {
using offset_t = std::size_t;
using position_t = std::streampos;
using size_t = std::streampos;

static constexpr std::size_t INDEX_CHUNK_SIZE =
    sizeof(std::streampos) + (sizeof(std::size_t) * 2);

class IndexChunk final {
 private:
  const offset_t _offset;
  const position_t _position;
  const size_t _size;

 public:
  IndexChunk(offset_t offset, position_t position, size_t size) noexcept
      : _offset(offset), _position(position), _size(size){};
  IndexChunk(IndexChunk &&) = default;
  virtual ~IndexChunk() = default;

  static auto read(char *source, std::size_t size) -> IndexChunk;
  auto write(char *target, std::size_t size) -> void;

  auto offset() noexcept { return _offset; }

  auto position() noexcept { return _position; }

  auto size() noexcept { return _size; }
};
} // namespace nmq::index_chunk