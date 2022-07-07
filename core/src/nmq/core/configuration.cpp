#include <nmq/core/configuration.h>

#include <boost/log/trivial.hpp>
#include <vector>

std::vector<nmq::TopicConfiguration> nmq::Configuration::topics() {
  auto result = std::vector<nmq::TopicConfiguration>();
  result.push_back(TopicConfiguration("foo", 1));
  result.push_back(TopicConfiguration("bar", 2));

  for (nmq::TopicConfiguration c : result) {
    BOOST_LOG_TRIVIAL(info)
        << "topic: " << c.name() << " partitions: " << c.partitions();
  }

  return result;
};
