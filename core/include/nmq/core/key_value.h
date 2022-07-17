#pragma once

#include <cinttypes>
#include <exception>
#include <limits>
#include <memory>
#include <vector>

namespace nmq::key_value {

using key_value_t = std::int32_t;

static constexpr std::size_t KEY_VALUE_HEADER_CHUNK_MAX_SIZE =
    std::numeric_limits<key_value_t>().max();

static constexpr std::size_t KEY_VALUE_HEADER_SIZE = sizeof(key_value_t) * 2;

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

  auto key_size() { return _key.size(); };
  auto has_key() noexcept { return _has_key; };
  auto value_size() { return _value.size(); };
  auto has_value() noexcept { return _has_value; };

  static auto read(char *source, std::size_t size) -> KeyValue;
  auto write(char *target, std::size_t size) -> std::size_t;

  auto size() noexcept -> std::size_t {
    return KEY_VALUE_HEADER_SIZE + (_has_key ? _key.size() : 0) +
           (_has_value ? _value.size() : 0);
  };
};

} // namespace nmq::key_value