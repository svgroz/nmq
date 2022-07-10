#include <cinttypes>
#include <memory>
#include <vector>

namespace nmq {
class KeyValue {
private:
  std::vector<char> _key;
  std::int32_t _key_size;
  std::vector<char> _value;
  std::int32_t _value_size;

public:
  KeyValue(std::vector<char> key, std::int32_t key_size,
           std::vector<char> value, std::int32_t value_size);
  KeyValue(const KeyValue &) = delete;
  virtual ~KeyValue();

  auto key() -> std::vector<char> { return _key; };
  auto key_size() -> std::int32_t { return _key_size; };
  auto value() -> std::vector<char> { return _value; };
  auto value_size() -> std::int32_t { return _value_size; };

  static auto read(char *source, std::size_t size) -> std::unique_ptr<KeyValue>;
  auto write(char *target, std::size_t size) -> void;
};

} // namespace nmq