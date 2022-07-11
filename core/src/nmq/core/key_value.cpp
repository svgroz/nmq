#include <cstring>
#include <limits>
#include <stdexcept>

#include <nmq/core/key_value.h>

namespace nmq {

static const std::size_t KEY_VALUE_META_SIZE = sizeof(nmq::KeyValueHeader) * 2;
static const KeyValueHeader MAX_KEY_VALUE_HEADER_VALUE =
    std::numeric_limits<KeyValueHeader>::max();

inline void check_min_size(std::size_t actual) {
  if (KEY_VALUE_META_SIZE > actual) {
    throw std::invalid_argument("check_min_size");
  }
}

inline void check_expected_size(std::size_t expected, std::size_t actual) {
  if (expected != actual) {
    throw std::invalid_argument(
        "check_expected_size expected: " + std::to_string(expected) +
        " actual: " + std::to_string(actual));
  }
}

inline void check_max_size(std::size_t actual) {
  if (actual > MAX_KEY_VALUE_HEADER_VALUE) {
    throw std::invalid_argument("check_max_size expected <: " +
                                std::to_string(MAX_KEY_VALUE_HEADER_VALUE) +
                                " actual: " + std::to_string(actual));
  }
}

inline auto init_array(char *source, std::size_t size) -> std::vector<char> {
  std::vector<char> result(size);
  std::memcpy(result.data(), source, size);
  return result;
}

inline auto size_of_inner_vector(std::size_t source_size) -> KeyValueHeader {
  if (source_size > MAX_KEY_VALUE_HEADER_VALUE) {
    throw std::invalid_argument(
        "sizeOf: this exception should never occur. source vector size " +
        std::to_string(source_size) + " > " +
        std::to_string(MAX_KEY_VALUE_HEADER_VALUE));
  }
  return static_cast<KeyValueHeader>(source_size);
}

KeyValue::KeyValue(std::vector<char> key, bool has_key, std::vector<char> value,
                   bool has_value)
    : _key(std::move(key)), _has_key(has_key), _value(std::move(value)),
      _has_value(has_value){};

KeyValue::~KeyValue() = default;

auto KeyValue::read(char *source, std::size_t size)
    -> std::unique_ptr<KeyValue> {
  check_min_size(size);

  KeyValueHeader key_size = reinterpret_cast<KeyValueHeader *>(source)[0];
  KeyValueHeader value_size = reinterpret_cast<KeyValueHeader *>(source)[1];

  std::size_t expected_size =
      KEY_VALUE_META_SIZE + std::max(key_size, 0) + std::max(value_size, 0);

  check_expected_size(expected_size, size);

  bool has_key = key_size > 0;
  bool has_value = value_size > 0;
  std::vector<char> key =
      has_key ? init_array(source + KEY_VALUE_META_SIZE, key_size)
              : std::vector<char>(0);

  std::vector<char> value =
      has_value > 0
          ? init_array(source + KEY_VALUE_META_SIZE + key_size, value_size)
          : std::vector<char>(0);

  return std::make_unique<KeyValue>(key, has_key, value, has_value);
}

auto KeyValue::write(char *target, std::size_t size) -> void {
  check_min_size(size);

  KeyValueHeader key_size = _has_key ? size_of_inner_vector(_key.size()) : -1;
  KeyValueHeader value_size =
      _has_value ? size_of_inner_vector(_value.size()) : -1;
  std::size_t expected_size = KEY_VALUE_META_SIZE + key_size + value_size;

  check_expected_size(expected_size, size);

  reinterpret_cast<KeyValueHeader *>(target)[0] = key_size;
  reinterpret_cast<KeyValueHeader *>(target)[1] = value_size;
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