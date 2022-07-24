#pragma once

#include <cstdint>

namespace nmq {
using message_offset_t = std::int_fast64_t;
using message_position_t = std::int_fast64_t;
using message_partition_t = std::uint_fast32_t;
using message_size_t = std::size_t;
} // namespace nmq