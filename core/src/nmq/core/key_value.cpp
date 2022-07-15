#include <cstring>
#include <limits>

#include <nmq/core/exceptions.h>
#include <nmq/core/key_value.h>

namespace nmq {

static const std::size_t KEY_VALUE_META_SIZE = sizeof(nmq::KeyValueHeader) * 2;
static const KeyValueHeader MAX_KEY_VALUE_HEADER_VALUE =
    std::numeric_limits<KeyValueHeader>::max();

void check_min_size(std::size_t actual) {
  if (KEY_VALUE_META_SIZE > actual) {
    throw ActualSizeLessThanMinSize(actual);
  }
}

void check_expected_size(std::size_t expected, std::size_t actual) {
  if (actual < expected) {
    throw ActualSizeLessThanExpectedSize(actual, expected);
  }
}

void check_max_size(std::size_t actual) {
  if (actual > MAX_KEY_VALUE_HEADER_VALUE) {
    throw ActualSizeHigherThanMinSize(actual);
  }
}

inline auto init_vector(char *source, std::size_t size) -> std::vector<char> {
  std::vector<char> result(size);
  std::copy(source, source + size, result.data());
  return result;
}

inline auto size_of_inner_vector(std::size_t source_size) -> KeyValueHeader {
  if (source_size > MAX_KEY_VALUE_HEADER_VALUE) {
    throw ActualSizeHigherThanMaxSize(source_size);
  }
  return static_cast<KeyValueHeader>(source_size);
}

auto KeyValue::read(char *source, const std::size_t size) -> KeyValue {
  if (source == nullptr) {
    throw NullptrArgumentException("source");
  }
  check_min_size(size);

  std::array<KeyValueHeader, 2> key_value_header;
  std::memcpy(key_value_header.data(), source, KEY_VALUE_META_SIZE);

  KeyValueHeader key_size = key_value_header[0];
  KeyValueHeader value_size = key_value_header[1];

  std::size_t expected_size =
      KEY_VALUE_META_SIZE + std::max(key_size, 0) + std::max(value_size, 0);

  check_expected_size(expected_size, size);

  bool has_key = key_size > -1;
  std::vector<char> key =
      key_size > 0 ? init_vector(source + KEY_VALUE_META_SIZE, key_size)
                   : std::vector<char>(0);

  bool has_value = value_size > -1;
  std::vector<char> value =
      value_size > 0
          ? init_vector(source + KEY_VALUE_META_SIZE + key_size, value_size)
          : std::vector<char>(0);

  return {key, has_key, value, has_value};
}

auto KeyValue::write(char *target, std::size_t size) -> void {
  if (target == nullptr) {
    throw NullptrArgumentException("target");
  }
  check_min_size(size);

  KeyValueHeader key_size = _has_key ? size_of_inner_vector(_key.size()) : -1;
  KeyValueHeader value_size =
      _has_value ? size_of_inner_vector(_value.size()) : -1;
  std::size_t expected_size = KEY_VALUE_META_SIZE + (_has_key ? key_size : 0) +
                              (_has_value ? value_size : 0);

  check_expected_size(expected_size, size);

  std::array<KeyValueHeader, 2> key_value_header = {key_size, value_size};
  std::memcpy(target, key_value_header.data(), KEY_VALUE_META_SIZE);

  target = target + KEY_VALUE_META_SIZE;

  if (key_size > 0) {
    std::memcpy(target, _key.data(), key_size);
    target = target + key_size;
  }

  if (value_size > 0) {
    std::memcpy(target, _value.data(), value_size);
  }
}
} // namespace nmq