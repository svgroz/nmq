#pragma once

#include <cstdint>
#include <exception>
#include <memory>
#include <string>

namespace nmq {
class NullptrArgumentException final : std::exception {
private:
  const std::string _argument_name;

public:
  explicit NullptrArgumentException(std::string argument_name)
      : _argument_name(std::move(argument_name)){};
  ~NullptrArgumentException() override = default;
  auto argument_name() { return std::string_view(_argument_name); }
};

class ActualSizeLessThanMinSize final : std::exception {
private:
  const std::size_t _actual_size;

public:
  explicit ActualSizeLessThanMinSize(std::size_t actual_size)
      : _actual_size(actual_size){};
  ~ActualSizeLessThanMinSize() override = default;
  auto actual_size() noexcept { return _actual_size; }
};

class ActualSizeHigherThanMinSize final : std::exception {
private:
  const std::size_t _actual_size;

public:
  explicit ActualSizeHigherThanMinSize(std::size_t actual_size)
      : _actual_size(actual_size){};
  ~ActualSizeHigherThanMinSize() override = default;
  auto actual_size() noexcept { return _actual_size; }
};

class ActualSizeLessThanExpectedSize final : std::exception {
private:
  const std::size_t _actual_size;
  const std::size_t _expected_size;

public:
  ActualSizeLessThanExpectedSize(std::size_t actual_size,
                                 std::size_t expected_size)
      : _actual_size(actual_size), _expected_size(expected_size){};
  ~ActualSizeLessThanExpectedSize() override = default;
  auto actual_size() noexcept { return _actual_size; }
  auto expected_size() noexcept { return _expected_size; }
};

class ActualSizeHigherThanMaxSize final : std::exception {
private:
  const std::size_t _actual_size;

public:
  explicit ActualSizeHigherThanMaxSize(std::size_t actual_size)
      : _actual_size(actual_size){};
  ~ActualSizeHigherThanMaxSize() override = default;
  auto actual_size() { return _actual_size; }
};

} // namespace nmq