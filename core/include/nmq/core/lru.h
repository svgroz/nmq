#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include <spdlog/spdlog.h>

namespace nmq {

template <class Key>
concept ComparableKey = std::copyable<Key>;

template <ComparableKey K, typename V, std::int_fast64_t size> class LRU {
private:
  using Context = struct Context {
    std::int_fast64_t id = std::numeric_limits<std::int_fast64_t>::min();
    K key;
    V value;
  };

  std::array<Context, size> cache;
  std::int_fast64_t seq{0};
  std::size_t current_size{0};

public:
  explicit LRU() = default;
  virtual ~LRU() = default;

  auto get(const K key) -> std::optional<V> {
    for (std::size_t i = 0; i < current_size; i++) {
      Context &c = cache[i];

      if (c.key == key) {
        return std::make_optional(c.value);
      }
    }

    return std::nullopt;
  };

  auto insert(const K key, V value) -> bool {
    std::int_fast64_t current_seq_id = ++seq;
    std::int_fast64_t min_id = std::numeric_limits<std::int_fast64_t>::max();
    std::int_fast64_t min_index = 0;

    for (std::int_fast64_t i = 0; i < current_size; i++) {
      Context &c = cache[i];
      if (c.key == key) {
        c.id = current_seq_id;
        c.value = value;
        return true;
      }

      if (c.id < min_id) {
        min_id = c.id;
        min_index = i;
      }
    }

    if (current_size < size) {
      min_index = current_size++;
    }

    cache[min_index] = {.id = current_seq_id, .key = key, .value = value};

    return false;
  }
};

} // namespace nmq