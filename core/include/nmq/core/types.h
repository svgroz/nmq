#pragma once

#include <cinttypes>
#include <ios>

namespace nmq {
using message_offset_t = std::int64_t;
using message_position_t = std::streampos;
using message_size_t = std::size_t;
} // namespace nmq