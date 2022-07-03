#pragma once

#include <string>
namespace nmq {
class Topic {
private:
  std::string _name;

public:
  Topic();
  virtual ~Topic();
};

}; // namespace nmq