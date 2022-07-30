#pragma once

#include <cstddef>

namespace nmq {

class Buffer final {
private:
  char *_data;
  const std::size_t _size;

public:
  explicit Buffer(std::size_t size) : _size(size), _data(new char[size]){};
  virtual ~Buffer() {
    if (_data) {
      delete[] _data;
    }
  }

  auto data() noexcept { return _data; };
  auto size() noexcept { return _size; };
};
} // namespace nmq