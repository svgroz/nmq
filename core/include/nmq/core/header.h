#include <vector>

namespace nmq {

class Header {
private:
  const std::vector<char> _key;
  const std::vector<char> _value;

public:
  Header(const std::vector<char> &key, const std::vector<char> &value);
  Header(const Header &) = delete;
  virtual ~Header();
};

} // namespace nmq