#include <nmq/core/message.h>

#include <limits>
#include <nmq/core/exceptions.h>

namespace nmq {

auto Message::add_header(KeyValue &header) -> void {
  _headers.emplace_back(std::move(header));
}

auto Message::size() -> message_size_t {
  message_size_t result = 0;
  for (auto &header : _headers) {
    result = result + header.size();
  }

  return result;
};

auto Message::read(char *source, std::size_t source_size) -> Message {
  std::size_t size = 0;
  std::vector<KeyValue> headers = std::vector<KeyValue>();
  while (size < source_size) {
    auto key_value = KeyValue::read(source + size, source_size - size);
    size = size + key_value.size();
    headers.emplace_back(std::move(key_value));
  }

  return Message(headers);
}

auto Message::write(char *target, std::size_t target_size) -> void {
  std::size_t offset = 0;
  for (auto &header : _headers) {
    offset = offset + header.write(target + offset, target_size - offset);
  }
}

} // namespace nmq