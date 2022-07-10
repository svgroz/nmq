#pragma once

#include <vector>

#include <memory>
#include <nmq/core/key_value.h>

namespace nmq {

class Message {
private:
  std::unique_ptr<KeyValue> _key_value;
  std::vector<std::unique_ptr<KeyValue>> _headers;

public:
  explicit Message(std::unique_ptr<KeyValue> key_value);
  Message(const Message &) = delete;
  virtual ~Message();
  auto add_header(std::unique_ptr<KeyValue> header) -> void;
  static auto read(char *source, const std::size_t source_size)
      -> std::unique_ptr<Message>;
  auto write(char *target, const std::size_t target_size) -> void;
};
} // namespace nmq