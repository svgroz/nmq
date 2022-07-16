#include <nmq/core/exceptions.h>
#include <nmq/core/index_chunk.h>

#include <array>
#include <cstring>

namespace nmq {
auto validate_buffer(char *source, std::size_t size) {
  if (source == nullptr) {
    throw NullptrArgumentException();
  }

  if (size < INDEX_CHUNK_SIZE) {
    throw ActualSizeLessThanExpectedSize(size, INDEX_CHUNK_SIZE);
  }
}

auto IndexChunk::read(char *source, std::size_t size) -> IndexChunk {
  validate_buffer(source, size);
  std::array<index_chunk_field, 3> index_data = {0, 0, 0};
  std::memcpy(index_data.data(), source, INDEX_CHUNK_SIZE);
  return {index_data[0], index_data[1], index_data[2]};
}

auto IndexChunk::write(char *target, std::size_t size) -> void {
  validate_buffer(target, size);
  std::array<index_chunk_field, 3> index_data = {_offset, _position, _size};
  std::memcpy(target, index_data.data(), INDEX_CHUNK_SIZE);
}
} // namespace nmq