#include <grpcpp/support/status.h>

#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "configuration.h"
#include "nmq_server.grpc.pb.h"
#include "nmq_server.pb.h"
#include "topic.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  nmq::Topic topic("/tmp/foo", 2);

  for (int i = 0; i < 30; i++) {
    proto::Message message;
    message.set_key("message-" + std::to_string(i));
    topic.add(message);
  }

  BOOST_LOG_TRIVIAL(info) << "nmq started";

  return 0;
};
