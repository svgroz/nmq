#pragma once

#include <cinttypes>
#include <exception>
#include <limits>
#include <memory>
#include <vector>

namespace nmq {

using key_value_t = std::int32_t;

class KeyValue final {
private:
  std::vector<char> _key;
  bool _has_key;
  std::vector<char> _value;
  bool _has_value;

public:
  KeyValue(std::vector<char> &key, bool has_key, std::vector<char> &value,
           bool has_value)
      : _key(std::move(key)), _has_key(has_key), _value(std::move(value)),
        _has_value(has_value){};
  KeyValue(const KeyValue &) = delete;
  KeyValue(KeyValue &&) = default;
  virtual ~KeyValue() = default;

  auto key_size() noexcept { return _key.size(); };
  auto has_key() noexcept { return _has_key; };
  auto value_size() noexcept { return _value.size(); };
  auto has_value() noexcept { return _has_value; };

  static auto read(char *source, std::int_fast64_t size) -> KeyValue;
  auto write(char *target, std::int_fast64_t size) -> std::int_fast64_t;

  auto size() noexcept -> std::int_fast64_t;
};

} // namespace nmq