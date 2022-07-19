#include <nmq/core/exceptions.h>
#include <nmq/core/index_chunk.h>

#include <array>
#include <cstring>

namespace nmq {
auto validate_buffer(char *source, std::size_t size) {
  if (source == nullptr) {
    throw NullptrArgumentException();
  }

  if (size < IndexChunk::INDEX_CHUNK_RAW_SIZE) {
    throw ActualLessThanExpected(size, IndexChunk::INDEX_CHUNK_RAW_SIZE);
  }
}

auto IndexChunk::read(
    std::array<char, IndexChunk::INDEX_CHUNK_RAW_SIZE> &source) -> IndexChunk {
  message_offset_t offset = 0;
  std::memcpy(source.data(), &offset, sizeof(offset));

  message_position_t position = 0;
  std::memcpy(source.data() + sizeof(offset), &position, sizeof(position));

  message_size_t message_size = 0;
  std::memcpy(source.data() + sizeof(offset) + sizeof(position), &message_size,
              sizeof(message_size));

  return IndexChunk(offset, position, message_size);
}

auto IndexChunk::write(
    std::array<char, IndexChunk::INDEX_CHUNK_RAW_SIZE> &target) -> void {
  std::memcpy(target.data(), &_offset, sizeof(_offset));
  std::memcpy(target.data() + sizeof(_offset), &_position, sizeof(_position));
  std::memcpy(target.data() + sizeof(_offset) + sizeof(_position),
              &_message_size, sizeof(_message_size));
}
} // namespace nmq