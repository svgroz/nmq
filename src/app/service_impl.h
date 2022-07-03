#pragma once

#include <grpcpp/support/status.h>

#include "nmq_server.grpc.pb.h"

namespace nmq {
class ServiceImpl final : public proto::Api::Service {
public:
  ServiceImpl();
  virtual ~ServiceImpl();
  virtual ::grpc::Status Send(::grpc::ServerContext *context,
                              const ::proto::SendRequest *request,
                              ::proto::SendResponse *response);
  virtual ::grpc::Status Poll(::grpc::ServerContext *context,
                              const ::proto::PollRequest *request,
                              ::proto::PollResponse *response);
};
} // namespace nmq
