#pragma once

#include <cstdint>
#include <stdlib>

namespace nmq {
int_fast32_t hashCode(const char *value, size_t size);
} // namespace nmq