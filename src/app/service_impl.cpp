#include "service_impl.h"

#include <grpcpp/support/status.h>

::grpc::Status nmq::ServiceImpl::Send(::grpc::ServerContext *context,
                                      const ::proto::SendRequest *request,
                                      ::proto::SendResponse *response) {
  return ::grpc::Status();
};