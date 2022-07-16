#pragma once

#include <cstdint>
#include <exception>
#include <memory>
#include <string>

namespace nmq {
class NullptrArgumentException final : std::exception {
 public:
  NullptrArgumentException() = default;
  ~NullptrArgumentException() override = default;
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
  const std::size_t _max_size;

 public:
  ActualSizeHigherThanMaxSize(std::size_t actual_size, std::size_t max_size)
      : _actual_size(actual_size), _max_size(max_size){};
  ~ActualSizeHigherThanMaxSize() override = default;
  auto actual_size() { return _actual_size; }
  auto max_size() { return _max_size; }
};

class CouldNotOpenFile final : std::exception {
 private:
  const std::string _filename;

 public:
  explicit CouldNotOpenFile(std::string &filename) : _filename(filename){};
  ~CouldNotOpenFile() override = default;
  auto filename() { return _filename; };
};

} // namespace nmq