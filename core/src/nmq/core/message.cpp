#include <nmq/core/message.h>

namespace nmq {

auto Message::add_header(KeyValue &header) -> void {
  _headers.emplace_back(std::move(header));
}

auto read(char *source, const std::size_t source_size) -> Message {
  KeyValue key_value = KeyValue::read(source, source_size);
  return Message(std::move(key_value));
}

} // namespace nmq