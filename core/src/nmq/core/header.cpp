#include <nmq/core/header.h>

#include <utility>

namespace nmq {

Header::Header(const std::vector<char> &key, const std::vector<char> &value)
    : _key(key), _value(value){};

Header::~Header() = default;

} // namespace nmq