#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include <spdlog/spdlog.h>

namespace nmq {

template <class Key>
concept ComparableKey = requires(Key a, Key b) {
  { a < b } -> std::convertible_to<bool>;
};

template <ComparableKey K, typename V, std::int_fast64_t size> class LRU {
private:
  using Context = struct Context {
    std::int_fast64_t id = std::numeric_limits<std::int_fast64_t>::min();
    K key;
    V value;
    bool has_value = false;
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

      if (c.has_value && c.key == key) {
        return std::make_optional(c.value);
      }
    }

    return std::nullopt;
  };

  auto insert(const K key, V value) -> bool {
    std::int_fast64_t current_seq_id = ++seq;
    std::int_fast64_t min_id = current_seq_id;
    std::int_fast64_t min_index = 0;

    for (std::int_fast64_t i = 0; i < size; i++) {
      Context &c = cache[i];
      if (c.has_value && c.key == key) {
        cache[i] = {.id = current_seq_id,
                    .key = key,
                    .value = value,
                    .has_value = true};
        return true;
      }

      if (c.id < min_id) {
        min_id = c.id;
        min_index = i;
      }
    }

    if (!cache[min_index].has_value) {
      current_size = current_size + 1;
    }

    cache[min_index] = {
        .id = current_seq_id, .key = key, .value = value, .has_value = true};

    return false;
  }
};

} // namespace nmq