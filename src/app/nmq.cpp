#include "nmq_server.grpc.pb.h"

class Foo final : public proto::Api::Service
{
  public:
    Foo();
    virtual ~Foo();
    virtual ::grpc::Status Send(::grpc::ServerContext* context, const ::proto::SendRequest* request, ::proto::SendResponse* response);
    virtual ::grpc::Status Poll(::grpc::ServerContext* context, const ::proto::PollRequest* request, ::proto::PollResponse* response);
};

int main()
{
  return 0;
};
