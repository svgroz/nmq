#pragma once

#include <vector>

#include <functional>
#include <iostream>
#include <memory>
#include <nmq/core/header.h>

namespace nmq {

class Message {
private:
  std::vector<std::unique_ptr<Header>> _headers;
  std::vector<char> _key;
  std::vector<char> _value;

public:
  Message(std::vector<char> &key, std::vector<char> &value);
  Message(const Message &) = delete;
  virtual ~Message();
  auto add_header(std::vector<char> &key, std::vector<char> &value) -> void;
  auto serialize() -> std::shared_ptr<std::vector<char>>;
  static auto desserialize(std::vector<char> &source)
      -> std::shared_ptr<Message>;
  auto key() -> std::vector<char>;
  auto value() -> std::vector<char>;
};
} // namespace nmq