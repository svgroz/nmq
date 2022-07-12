#include <cinttypes>
#include <exception>
#include <memory>
#include <vector>

namespace nmq {

using KeyValueHeader = std::int32_t;

class ActualSizeLessThanMinSize : std::exception {
private:
  const std::size_t _actual_size;

public:
  explicit ActualSizeLessThanMinSize(std::size_t actual_size)
      : _actual_size(actual_size){};
  ~ActualSizeLessThanMinSize() override = default;
  auto actual_size() -> std::size_t { return _actual_size; }
};

class ActualSizeHigherThanMinSize : std::exception {
private:
  const std::size_t _actual_size;

public:
  explicit ActualSizeHigherThanMinSize(std::size_t actual_size)
      : _actual_size(actual_size){};
  ~ActualSizeHigherThanMinSize() override = default;
  auto actual_size() -> std::size_t { return _actual_size; }
};

class ActualSizeLessThanExpectedSize : std::exception {
private:
  const std::size_t _actual_size;
  const std::size_t _expected_size;

public:
  ActualSizeLessThanExpectedSize(std::size_t actual_size,
                                 std::size_t expected_size)
      : _actual_size(actual_size), _expected_size(expected_size){};
  ~ActualSizeLessThanExpectedSize() override = default;
  auto actual_size() -> std::size_t { return _actual_size; }
  auto expected_size() -> std::size_t { return _expected_size; }
};

class ActualSizeHigherThanMaxSize : std::exception {
private:
  const std::size_t _actual_size;

public:
  explicit ActualSizeHigherThanMaxSize(std::size_t actual_size)
      : _actual_size(actual_size){};
  ~ActualSizeHigherThanMaxSize() override = default;
  auto actual_size() -> std::size_t { return _actual_size; }
};

class KeyValue {
private:
  std::vector<char> _key;
  const bool _has_key;
  std::vector<char> _value;
  const bool _has_value;

public:
  KeyValue(std::vector<char> key, bool has_key, std::vector<char> value,
           bool has_value);
  KeyValue(const KeyValue &) = delete;
  virtual ~KeyValue();

  auto key() -> std::vector<char> { return _key; };
  auto has_key() -> bool { return _has_key; };
  auto value() -> std::vector<char> { return _value; };
  auto has_value() -> bool { return _has_value; };

  static auto read(char *source, std::size_t size) -> std::unique_ptr<KeyValue>;
  auto write(char *target, std::size_t size) -> void;
};

} // namespace nmq