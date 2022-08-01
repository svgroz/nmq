#pragma once

#include <cstdint>
#include <vector>

#include <memory>
#include <nmq/core/key_value.h>
#include <nmq/core/types.h>

namespace nmq {

class Message final {
private:
  std::vector<KeyValue> _headers;

public:
  explicit Message(std::vector<KeyValue> &headers)
      : _headers(std::move(headers)){};
  Message(const Message &) = delete;
  Message(Message &&message) = default;
  virtual ~Message() = default;
  auto add_header(KeyValue &header) -> void;
  auto size() noexcept -> message_size_t;
  static auto read(char *source, std::int_fast64_t source_size) -> Message;
  auto write(char *target, std::int_fast64_t target_size) -> void;
};
} // namespace nmq