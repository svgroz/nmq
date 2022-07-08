#include <nmq/core/hash.h>

namespace nmq {
auto hashCode(const char *value, size_t size) {
  int_fast32_t result = 0;
  for (size_t v = 0; v < size; v++) {
    result = 31 * result + (value[v] & 0xff);
  }
  return result;   
};
} // namespace nmq
