#pragma once

#include <cstdint>

namespace nmq {
using index_field = std::uint64_t;
static const std::size_t INDEX_SIZE = sizeof(index_field) * 3;

class Index final {
private:
  const index_field _offset;
  const index_field _position;
  const index_field _size;

public:
  Index(index_field offset, index_field position, index_field size) noexcept
      : _offset(offset), _position(position), _size(size){};
  Index(Index &&) = default;
  virtual ~Index() = default;

  static auto read(char *source, std::size_t size) -> Index;
  auto write(char *target, std::size_t size) -> void;

  auto offset() noexcept { return _offset; }

  auto position() noexcept { return _position; }

  auto size() noexcept { return _size; }
};
} // namespace nmq