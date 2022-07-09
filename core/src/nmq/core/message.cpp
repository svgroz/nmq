#include <nmq/core/message.h>

#include <algorithm>
#include <cstdint>
#include <cstring>

#include <boost/log/trivial.hpp>

namespace nmq {

Message::Message(std::vector<char> &key, std::vector<char> &value)
    : _key(key), _value(value){};

Message::~Message() = default;

auto Message::add_header(std::vector<char> &key, std::vector<char> &value)
    -> void {
  _headers.emplace_back(std::make_unique<Header>(key, value));
}

inline auto write_vector(char *dest, std::vector<char> &data) -> std::size_t {
  std::uint64_t data_size = data.size();
  std::memcpy(dest, &data_size, sizeof(std::uint64_t));
  if (data_size < 1) {
    return sizeof(std::uint64_t);
  }

  dest = dest + sizeof(std::uint64_t);
  std::memcpy(dest, data.data(), data.size());
  return sizeof(std::uint64_t) + data.size();
}

auto Message::serialize() -> std::shared_ptr<std::vector<char>> {
  std::uint64_t message_size = 0;
  message_size = message_size + sizeof(std::uint64_t) + _key.size();
  message_size = message_size + sizeof(std::uint64_t) + _value.size();

  std::shared_ptr<std::vector<char>> buffer =
      std::make_shared<std::vector<char>>(message_size);

  char *buffer_ptr = buffer->data();

  buffer_ptr = buffer_ptr + write_vector(buffer_ptr, _key);
  write_vector(buffer_ptr, _value);
  return buffer;
}

inline auto check_length(std::size_t source_size, std::size_t expected_min_size)
    -> void {
  if (source_size < expected_min_size) {
    BOOST_LOG_TRIVIAL(error) << "min expected size: " << expected_min_size
                             << " actual size: " << source_size;
    throw std::invalid_argument("illegal source length");
  }
}

inline auto read_vector(char *source, std::size_t size)
    -> std::shared_ptr<std::vector<char>> {

  std::uint64_t vector_size = 0;
  std::memcpy(&vector_size, source, sizeof(std::uint64_t));
  if (vector_size == 0) {
    return std::make_shared<std::vector<char>>(0);
  }

  check_length(size, sizeof(std::uint64_t) + vector_size);

  auto result = std::make_shared<std::vector<char>>(vector_size);
  source = source + sizeof(std::uint64_t);
  std::memcpy(result->data(), source, vector_size);

  return result;
}

auto Message::desserialize(std::vector<char> &source)
    -> std::shared_ptr<Message> {

  std::size_t source_size = source.size();
  check_length(source_size, sizeof(std::uint64_t) * 2);

  char *sourse_ptr = source.data();
  std::shared_ptr<std::vector<char>> key = read_vector(sourse_ptr, source_size);

  auto min_expected_size =
      sizeof(std::uint64_t) + key->size() + sizeof(std::uint64_t);

  check_length(source_size, min_expected_size);

  sourse_ptr = sourse_ptr + sizeof(std::uint64_t) + key->size();
  std::shared_ptr<std::vector<char>> value = read_vector(
      sourse_ptr, source_size - key->size() - sizeof(std::uint64_t));
  return std::make_unique<Message>(*key, *value);
}

auto Message::key() -> std::vector<char> { return _key; }

auto Message::value() -> std::vector<char> { return _value; }

} // namespace nmq