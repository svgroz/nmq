#include <nmq/core/configuration.h>

#include <boost/log/trivial.hpp>
#include <vector>

namespace nmq {
auto Configuration::topics() -> std::vector<nmq::TopicConfiguration> {
  auto result = std::vector<nmq::TopicConfiguration>();
  result.emplace_back("foo", 1);
  result.emplace_back("bar", 2);

  for (nmq::TopicConfiguration c : result) {
    BOOST_LOG_TRIVIAL(info)
        << "topic: " << c.name() << " partitions: " << c.partitions();
  }

  return result;
};

} // namespace nmq
