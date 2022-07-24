#pragma once

#include <cerrno>
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

class ActualLessThanExpected final : std::exception {
private:
  const std::int64_t _actual;
  const std::int64_t _expected;

public:
  ActualLessThanExpected(std::int64_t actual, std::int64_t expected)
      : _actual(actual), _expected(expected){};
  ~ActualLessThanExpected() override = default;
  auto actual() noexcept { return _actual; }
  auto expected() noexcept { return _expected; }
};

class ActualHigherThanExpected final : std::exception {
private:
  const std::int64_t _actual;
  const std::int64_t _expected;

public:
  ActualHigherThanExpected(std::int64_t actual, std::int64_t expected)
      : _actual(actual), _expected(expected){};
  ~ActualHigherThanExpected() override = default;
  auto actual() { return _actual; }
  auto expected() { return _expected; }
};

class CouldNotOpenFile final : std::exception {
private:
  const std::string _filename;

public:
  explicit CouldNotOpenFile(std::string &filename) : _filename(filename){};
  ~CouldNotOpenFile() override = default;
  auto filename() { return _filename; };
};

class SystemError final : std::exception {
private:
  int _error_number;

public:
  explicit SystemError(int error_number) : _error_number(error_number){};
  ~SystemError() override = default;
  auto error_number() { return _error_number; };
};

} // namespace nmq