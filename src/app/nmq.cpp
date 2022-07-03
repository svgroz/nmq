#include <grpcpp/support/status.h>

#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "configuration.h"
#include "nmq_server.grpc.pb.h"
#include "nmq_server.pb.h"
#include "topic.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  nmq::Configuration conf;

  std::vector<nmq::TopicConfiguration> topic_configurations = conf.topics();
  std::vector<std::unique_ptr<nmq::Topic>> topics;

  for (auto topic_configuration : topic_configurations) {
    topics.push_back(
        std::make_unique<nmq::Topic>("/tmp/" + topic_configuration.name(),
                                     topic_configuration.partitions()));
  }

  BOOST_LOG_TRIVIAL(info) << "nmq started";

  return 0;
};
