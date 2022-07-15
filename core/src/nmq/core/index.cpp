#include <nmq/core/exceptions.h>
#include <nmq/core/index.h>

#include <array>
#include <cstring>

namespace nmq {
auto validate_buffer(char *source, std::size_t size) {
  if (source == nullptr) {
    throw NullptrArgumentException("source");
  }

  if (size < INDEX_SIZE) {
    throw ActualSizeLessThanExpectedSize(size, INDEX_SIZE);
  }
}

auto Index::read(char *source, std::size_t size) -> Index {
  validate_buffer(source, size);
  std::array<index_field, 3> index_data = {0, 0, 0};
  std::memcpy(index_data.data(), source, INDEX_SIZE);
  return {index_data[0], index_data[1], index_data[2]};
}
} // namespace nmq