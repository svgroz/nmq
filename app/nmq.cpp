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

  nmq::Partition p("/tmp/test-partition");

  proto::Message message1;
  message1.set_key("v1");

  proto::Message message2;
  message2.set_key("v2");

  p.add(message1);
  p.add(message2);

  BOOST_LOG_TRIVIAL(info) << "nmq started";

  return 0;
};
