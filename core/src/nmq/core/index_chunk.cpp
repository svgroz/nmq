#include <nmq/core/exceptions.h>
#include <nmq/core/index_chunk.h>

#include <array>
#include <cstring>

namespace nmq::index_chunk {
auto validate_buffer(char *source, std::size_t size) {
  if (source == nullptr) {
    throw nmq::exception::NullptrArgumentException();
  }

  if (size < INDEX_CHUNK_SIZE) {
    throw nmq::exception::ActualLessThanExpected(size, INDEX_CHUNK_SIZE);
  }
}

auto IndexChunk::read(char *source, std::size_t size) -> IndexChunk {
  validate_buffer(source, size);
  std::array<char, INDEX_CHUNK_SIZE> index_data = {};
  offset_t offset = 0;
  std::memcpy(index_data.data(), &offset, sizeof(offset));

  position_t position = 0;
  std::memcpy(index_data.data() + sizeof(offset), &position, sizeof(position));

  size_t message_size = 0;
  std::memcpy(index_data.data() + sizeof(offset) + sizeof(position),
              &message_size, sizeof(message_size));

  return {offset, position, message_size};
}

auto IndexChunk::write(char *target, std::size_t size) -> void {
  validate_buffer(target, size);
  std::memcpy(target, &_offset, sizeof(_offset));
  std::memcpy(target + sizeof(_offset), &_position, sizeof(_position));
  std::memcpy(target + sizeof(_offset) + sizeof(_position), &_size,
              sizeof(_size));
}
} // namespace nmq