#include <nmq/core/message.h>

#include <limits>
#include <nmq/core/exceptions.h>

namespace nmq::message {

static constexpr std::int64_t MAX_INT64_T =
    std::numeric_limits<std::int64_t>().max();

auto Message::add_header(key_value::KeyValue &header) -> void {
  _headers.emplace_back(std::move(header));
}

auto Message::size() -> std::int64_t {
  std::size_t key_size = _key_value.size();
  std::size_t headers_size = _headers.size();
  std::size_t result = key_size + headers_size;
  for (auto &header : _headers) {
    result = result + header.size();
  }
  if (result > MAX_INT64_T) {
    throw exception::ActualHigherThanExpected(result, MAX_INT64_T);
  }
  return static_cast<std::int64_t>(result);
};

auto Message::read(char *source, std::size_t source_size) -> Message {
  auto initial_key_value = key_value::KeyValue::read(source, source_size);
  std::size_t size = initial_key_value.size();
  auto headers = std::vector<key_value::KeyValue>();
  while (size < source_size) {
    auto key_value = key_value::KeyValue::read(source + size, source_size - size);
    size = size + key_value.size();
    headers.emplace_back(std::move(key_value));
  }

  return Message(std::move(initial_key_value), std::move(headers));
}

auto Message::write(char *target, std::size_t target_size) -> void {
  std::size_t offset = _key_value.write(target, target_size);
  for (auto &header : _headers) {
    offset = offset + header.write(target + offset, target_size - offset);
  }
}

} // namespace nmq