#include <fstream>
#include <grpcpp/support/status.h>

#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "configuration.h"
#include "topic.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

/*
  nmq::Topic topic("/tmp/foo", 2);

  for (int i = 0; i < 30; i++) {
    proto::Message message;
    message.set_key("message-" + std::to_string(i));
    topic.add(message);
  }

  BOOST_LOG_TRIVIAL(info) << "nmq started";
*/

  std::ofstream o("/tmp/t");
  std::ifstream i("/tmp/t");
  
  BOOST_LOG_TRIVIAL(info) << "o is ok: " << o.good();
  BOOST_LOG_TRIVIAL(info) << "i is ok: " << i.good();

  std::string x = "asdada";
  auto buf = x.c_str();
  char b[10];

  o.write(buf, x.size());
  i.read(b, 10);

  BOOST_LOG_TRIVIAL(debug) << std::string(b);
  
  return 0;
};
