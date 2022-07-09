#include <nmq/core/message.h>

namespace nmq {

Message::Message(const std::vector<char> &key, const std::vector<char> &value)
    : _key(key), _value(value){};

Message::~Message() = default;

auto Message::add_header(const std::vector<char> &key,
                         const std::vector<char> &value) -> void {
  _headers.emplace_back(std::make_unique<Header>(key, value));
}

} // namespace nmq