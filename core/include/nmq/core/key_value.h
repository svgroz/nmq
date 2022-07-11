#include <cinttypes>
#include <memory>
#include <vector>

namespace nmq {

using KeyValueHeader = std::int32_t;

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