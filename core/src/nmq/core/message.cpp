#include <nmq/core/message.h>

namespace nmq {

auto Message::add_header(KeyValue &header) -> void {
  _headers.emplace_back(std::move(header));
}

auto Message::size() -> std::size_t {
  std::size_t result = _key_value.size();
  std::size_t headers_size = _headers.size();
  for (std::size_t i = 0; i < headers_size; i++) {
    result = result + _headers[i].size();
  }
  return result;
};

auto Message::read(char *source, std::size_t source_size) -> Message {
  KeyValue initial_key_value = KeyValue::read(source, source_size);
  std::size_t offset = initial_key_value.size();
  std::vector<KeyValue> headers = std::vector<KeyValue>();
  while (offset < source_size) {
    KeyValue key_value = KeyValue::read(source + offset, source_size - offset);
    offset = offset + key_value.size();
    headers.emplace_back(std::move(key_value));
  }

  return Message(std::move(initial_key_value), std::move(headers));
}

//TODO add write

} // namespace nmq