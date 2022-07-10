#include <nmq/core/message.h>

namespace nmq {

Message::Message(std::unique_ptr<KeyValue> key_value)
    : _key_value(std::move(key_value)){};

Message::~Message() = default;

auto Message::add_header(std::unique_ptr<KeyValue> header) -> void {
  _headers.emplace_back(std::move(header));
}

auto read(char *source, const std::size_t source_size)
    -> std::unique_ptr<Message> {
  std::unique_ptr<KeyValue> key_value = KeyValue::read(source, source_size);
  return std::make_unique<Message>(std::move(key_value));
}

} // namespace nmq