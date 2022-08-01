#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>

#include <boost/numeric/conversion/cast.hpp>

#include <nmq/core/exceptions.h>
#include <nmq/core/key_value.h>

namespace nmq {

inline constexpr std::int_fast32_t KEY_VALUE_HEADER_CHUNK_MAX_SIZE =
    std::numeric_limits<key_value_t>().max();

inline constexpr std::int_fast32_t KEY_VALUE_HEADER_SIZE =
    static_cast<std::int_fast32_t>(sizeof(key_value_t) * 2);

auto check_expected_size(std::int_fast64_t expected, std::int_fast64_t actual) {
  if (actual < expected) {
    throw ActualLessThanExpected(actual, expected);
  }
}

auto check_min_size(std::int_fast64_t actual) {
  if (actual < KEY_VALUE_HEADER_SIZE) {
    throw ActualLessThanExpected(actual, KEY_VALUE_HEADER_SIZE);
  }
}

auto check_max_size(std::int_fast64_t actual) {
  if (actual > KEY_VALUE_HEADER_CHUNK_MAX_SIZE) {
    throw ActualHigherThanExpected(actual, KEY_VALUE_HEADER_CHUNK_MAX_SIZE);
  }
}

auto init_vector(char *source, std::int_fast64_t size) -> std::vector<char> {
  std::vector<char> result(size);
  std::memcpy(result.data(), source, size);
  return result;
}

auto size_of_inner_vector(key_value_t source_size) {
  if (source_size > KEY_VALUE_HEADER_CHUNK_MAX_SIZE) {
    throw ActualHigherThanExpected(source_size,
                                   KEY_VALUE_HEADER_CHUNK_MAX_SIZE);
  }
  return static_cast<key_value_t>(source_size);
}

auto KeyValue::read(char *source, const std::int_fast64_t size) -> KeyValue {
  if (source == nullptr) {
    throw NullptrArgumentException();
  }
  check_min_size(size);

  auto key_value_header = std::array<key_value_t, 2>();
  std::memcpy(key_value_header.data(), source, KEY_VALUE_HEADER_SIZE);

  key_value_t key_size = key_value_header[0];
  key_value_t value_size = key_value_header[1];

  std::int_fast64_t expected_size =
      KEY_VALUE_HEADER_SIZE + std::max(key_size, 0) + std::max(value_size, 0);

  check_expected_size(expected_size, size);

  bool has_key = key_size > -1;
  std::vector<char> key =
      key_size > 0 ? init_vector(source + KEY_VALUE_HEADER_SIZE, key_size)
                   : std::vector<char>(0);

  bool has_value = value_size > -1;
  std::vector<char> value =
      value_size > 0
          ? init_vector(source + KEY_VALUE_HEADER_SIZE + key_size, value_size)
          : std::vector<char>(0);

  return {key, has_key, value, has_value};
}

auto KeyValue::write(char *target, std::int_fast64_t size)
    -> std::int_fast64_t {
  if (target == nullptr) {
    throw NullptrArgumentException();
  }
  
  check_min_size(size);

  key_value_t key_size =
      _has_key
          ? size_of_inner_vector(boost::numeric_cast<key_value_t>(_key.size()))
          : -1;

  key_value_t value_size =
      _has_value ? size_of_inner_vector(
                       boost::numeric_cast<key_value_t>(_value.size()))
                 : -1;

  std::int_fast64_t expected_size = KEY_VALUE_HEADER_SIZE +
                                    (_has_key ? key_size : 0) +
                                    (_has_value ? value_size : 0);

  check_expected_size(expected_size, size);

  std::array<key_value_t, 2> key_value_header = {key_size, value_size};
  std::memcpy(target, key_value_header.data(), KEY_VALUE_HEADER_SIZE);

  target = target + KEY_VALUE_HEADER_SIZE;

  if (key_size > 0) {
    std::memcpy(target, _key.data(), key_size);
    target = target + key_size;
  }

  if (value_size > 0) {
    std::memcpy(target, _value.data(), value_size);
  }

  return KEY_VALUE_HEADER_SIZE + key_size + value_size;
}

auto KeyValue::size() noexcept -> std::int_fast64_t {
  return KEY_VALUE_HEADER_SIZE + (_has_key ? _key.size() : 0) +
         (_has_value ? _value.size() : 0);
};
} // namespace nmq