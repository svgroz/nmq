#include <cstddef>
#include <nmq/core/types.h>

namespace nmq {
class IndexChunk final {
private:
  message_offset_t _offset;
  message_position_t _position;
  message_size_t _size;

public:
  constexpr static std::size_t MIN_BUFFER_SIZE =
      sizeof(_offset) + sizeof(_position) + sizeof(_size);
  explicit IndexChunk() noexcept : _offset(-1), _position(-1), _size(0){};
  IndexChunk(message_offset_t offset, message_position_t position,
             message_size_t size) noexcept
      : _offset(offset), _position(position), _size(size){};
  IndexChunk(const IndexChunk &) = default;
  IndexChunk(IndexChunk &&) = default;
  virtual ~IndexChunk() = default;
  auto offset() noexcept { return _offset; }
  auto position() noexcept { return _position; }
  auto size() noexcept { return _size; }
  static auto read(char *source, std::size_t size) -> IndexChunk;
  auto write(char *source, std::size_t size) -> void;
};
} // namespace nmq