#include <cstring>
#include <nmq/core/exceptions.h>
#include <nmq/core/index_chunk.h>
#include <nmq/core/types.h>

namespace nmq {

auto IndexChunk::read(char *source, std::size_t size) -> IndexChunk {
  if (size < MIN_BUFFER_SIZE) {
    throw ActualLessThanExpected(size, MIN_BUFFER_SIZE);
  }

  message_offset_t message_offset = 0;
  message_position_t message_position = 0;
  message_size_t message_size = 0;

  std::memcpy(&message_offset, source, sizeof(message_offset));
  std::size_t offset = sizeof(message_offset);

  std::memcpy(&message_position + offset, source, sizeof(message_position));
  offset = offset + sizeof(message_position);

  std::memcpy(&message_offset + offset, source, sizeof(message_position));

  return IndexChunk(message_offset, message_position, message_size);
}

auto IndexChunk::write(char *source, std::size_t size) -> void {
  if (size < MIN_BUFFER_SIZE) {
    throw ActualLessThanExpected(size, MIN_BUFFER_SIZE);
  }

  std::memcpy(source, &_offset, sizeof(_offset));
  std::size_t offset = sizeof(_offset);

  std::memcpy(source + offset, &_position, sizeof(_position));
  offset = offset + sizeof(_position);

  std::memcpy(source + offset, &_size, sizeof(_size));
}

} // namespace nmq