#include <nmq/core/service_impl.h>

#include <grpcpp/support/status.h>

auto nmq::ServiceImpl::Send(::grpc::ServerContext *context,
                            const ::proto::SendRequest *request,
                            ::proto::SendResponse *response) -> ::grpc::Status {
  (void)context;
  (void)request;
  (void)response;

  return {};
};