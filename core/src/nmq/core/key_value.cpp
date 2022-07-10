#include <cstring>
#include <stdexcept>

#include <nmq/core/key_value.h>

static const std::size_t KEY_VALUE_META_SIZE = sizeof(std::int32_t) * 2;

namespace nmq {

inline void checkMinSize(std::size_t actual) {
  if (KEY_VALUE_META_SIZE > actual) {
    throw std::invalid_argument("checkMinSize");
  }
}

inline void checkExpectedSize(std::size_t expected, std::size_t actual) {
  if (expected != actual) {
    throw std::invalid_argument("checkExpectedSize expected: " + std::to_string(expected) + " actual: " + std::to_string(actual));
  }
}

inline auto init_array(char *source, std::size_t size) -> std::vector<char> {
  std::vector<char> result(size);
  std::memcpy(result.data(), source, size);
  return result;
}

KeyValue::KeyValue(std::vector<char> key, std::int32_t key_size,
                   std::vector<char> value, std::int32_t value_size)
    : _key(std::move(key)), _key_size(key_size), _value(std::move(value)),
      _value_size(value_size){};

KeyValue::~KeyValue() = default;

auto KeyValue::read(char *source, std::size_t size)
    -> std::unique_ptr<KeyValue> {
  checkMinSize(size);

  std::int32_t key_size = -1;
  std::int32_t value_size = -1;

  std::memcpy(&key_size, source, sizeof(std::int32_t));
  std::memcpy(&value_size, source + sizeof(std::int32_t), sizeof(std::int32_t));

  std::size_t expected_size =
      KEY_VALUE_META_SIZE + std::max(key_size, 0) + std::max(value_size, 0);

  checkExpectedSize(expected_size, size);

  std::vector<char> key =
      key_size > 0 ? init_array(source + KEY_VALUE_META_SIZE, key_size)
                   : std::vector<char>(0);
  
  std::vector<char> value =
      value_size > 0
          ? init_array(source + KEY_VALUE_META_SIZE + key_size, key_size)
          : std::vector<char>(0);

  return std::make_unique<KeyValue>(key, key_size, value, value_size);
}

auto KeyValue::write(char *target, std::size_t size) -> void {
  checkMinSize(size);

  std::size_t expected_size =
      KEY_VALUE_META_SIZE + std::max(_key_size, 0) + std::max(_value_size, 0);

  checkExpectedSize(expected_size, size);

  std::memcpy(target, &_key_size, sizeof(std::int32_t));
  target = target + sizeof(std::int32_t);
  std::memcpy(target, &_value_size, sizeof(std::int32_t));
  target = target + sizeof(std::int32_t);

  if (_key_size > 0) {
    std::memcpy(target, _key.data(), _key_size);
    target = target + _key_size;
  }

  if (_value_size > 0) {
    std::memcpy(target, _value.data(), _value_size);
  }
}
} // namespace nmq