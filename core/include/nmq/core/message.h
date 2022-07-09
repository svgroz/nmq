#include <vector>

#include <memory>
#include <nmq/core/header.h>

namespace nmq {

class Message {
private:
  std::vector<std::unique_ptr<Header>> _headers;
  std::vector<char> _key;
  std::vector<char> _value;

public:
  Message(const std::vector<char> &key, const std::vector<char> &value);
  Message(const Message &) = delete;
  virtual ~Message();
  auto add_header(const std::vector<char> &key, const std::vector<char> &value) -> void;
};
} // namespace nmq