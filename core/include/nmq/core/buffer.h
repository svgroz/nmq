#pragma once

#include <cstddef>

namespace nmq {

template <typename T> class Buffer final {
private:
  char *_data;
  const std::size_t _size;
  T _context;

public:
  explicit Buffer(std::size_t size, T context)
      : _size(size), _data(new char[size]), _context(context){};
  Buffer(const Buffer &) = delete;
  Buffer(Buffer &&source) noexcept {
    _data = source._data;
    source._data = nullptr;
    _context = source._context;
  };
  virtual ~Buffer() {
    if (_data) {
      delete[] _data;
      _data = nullptr;
    }
  }

  auto data() noexcept { return _data; };
  auto size() noexcept { return _size; };
  auto context() noexcept { return _context; }
  auto set_context(T context) noexcept { this->_context = context; };
};
} // namespace nmq