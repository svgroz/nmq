#include "hash.h"
#include "hash.hpp"

int_fast32_t nmq::hashCode(const char *value, size_t size) {
  int_fast32_t h = 0;
  for (size_t v = 0; v < size; v++) {
    h = 31 * h + (value[v] & 0xff);
  }
  return h;
};