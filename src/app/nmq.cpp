#include "nmq_server.grpc.pb.h"

class Foo final : public proto::Api::Service
{
};

int main()
{
  proto::SendMessageRequest req;
  req.mutable_payload()->mutable_value();
  return 0;
}