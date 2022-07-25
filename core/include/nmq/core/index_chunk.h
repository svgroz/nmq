#include <cstddef>
#include <nmq/core/types.h>

namespace nmq {

using IndexChunk = struct {
  message_offset_t _offset;
  message_position_t _position;
  message_size_t _size;
};
} // namespace nmq