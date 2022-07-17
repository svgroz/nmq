#pragma once

#include <vector>

#include <memory>
#include <nmq/core/key_value.h>

namespace nmq::message {

class Message final {
 private:
  key_value::KeyValue _key_value;
  std::vector<key_value::KeyValue> _headers;

 public:
  Message(key_value::KeyValue key_value,
          std::vector<key_value::KeyValue> headers)
      : _key_value(std::move(key_value)), _headers(std::move(headers)){};
  Message(const Message &) = delete;
  Message(Message &&message) noexcept
      : _key_value(std::move(message._key_value)),
        _headers(std::move(message._headers)){};
  virtual ~Message() = default;
  auto add_header(key_value::KeyValue &header) -> void;
  auto size() -> std::int64_t;
  static auto read(char *source, std::size_t source_size) -> Message;
  auto write(char *target, std::size_t target_size) -> void;
};
} // namespace nmq::message