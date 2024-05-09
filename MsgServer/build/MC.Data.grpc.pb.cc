// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: MC.Data.proto

#include "MC.Data.pb.h"
#include "MC.Data.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace MC {
namespace Data {

static const char* MCData_method_names[] = {
  "/MC.Data.MCData/GetUserPassword",
  "/MC.Data.MCData/TryRegist",
  "/MC.Data.MCData/GetUserFriends",
};

std::unique_ptr< MCData::Stub> MCData::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MCData::Stub> stub(new MCData::Stub(channel, options));
  return stub;
}

MCData::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetUserPassword_(MCData_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_TryRegist_(MCData_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetUserFriends_(MCData_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status MCData::Stub::GetUserPassword(::grpc::ClientContext* context, const ::MC::Data::MCDataUserRequest& request, ::MC::Data::MCDataUserResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::MC::Data::MCDataUserRequest, ::MC::Data::MCDataUserResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetUserPassword_, context, request, response);
}

void MCData::Stub::async::GetUserPassword(::grpc::ClientContext* context, const ::MC::Data::MCDataUserRequest* request, ::MC::Data::MCDataUserResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::MC::Data::MCDataUserRequest, ::MC::Data::MCDataUserResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUserPassword_, context, request, response, std::move(f));
}

void MCData::Stub::async::GetUserPassword(::grpc::ClientContext* context, const ::MC::Data::MCDataUserRequest* request, ::MC::Data::MCDataUserResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUserPassword_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::MC::Data::MCDataUserResponse>* MCData::Stub::PrepareAsyncGetUserPasswordRaw(::grpc::ClientContext* context, const ::MC::Data::MCDataUserRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::MC::Data::MCDataUserResponse, ::MC::Data::MCDataUserRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetUserPassword_, context, request);
}

::grpc::ClientAsyncResponseReader< ::MC::Data::MCDataUserResponse>* MCData::Stub::AsyncGetUserPasswordRaw(::grpc::ClientContext* context, const ::MC::Data::MCDataUserRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetUserPasswordRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MCData::Stub::TryRegist(::grpc::ClientContext* context, const ::MC::Data::MCDataRegistRequest& request, ::MC::Data::MCDataRegistResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::MC::Data::MCDataRegistRequest, ::MC::Data::MCDataRegistResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_TryRegist_, context, request, response);
}

void MCData::Stub::async::TryRegist(::grpc::ClientContext* context, const ::MC::Data::MCDataRegistRequest* request, ::MC::Data::MCDataRegistResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::MC::Data::MCDataRegistRequest, ::MC::Data::MCDataRegistResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_TryRegist_, context, request, response, std::move(f));
}

void MCData::Stub::async::TryRegist(::grpc::ClientContext* context, const ::MC::Data::MCDataRegistRequest* request, ::MC::Data::MCDataRegistResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_TryRegist_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::MC::Data::MCDataRegistResponse>* MCData::Stub::PrepareAsyncTryRegistRaw(::grpc::ClientContext* context, const ::MC::Data::MCDataRegistRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::MC::Data::MCDataRegistResponse, ::MC::Data::MCDataRegistRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_TryRegist_, context, request);
}

::grpc::ClientAsyncResponseReader< ::MC::Data::MCDataRegistResponse>* MCData::Stub::AsyncTryRegistRaw(::grpc::ClientContext* context, const ::MC::Data::MCDataRegistRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncTryRegistRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MCData::Stub::GetUserFriends(::grpc::ClientContext* context, const ::MC::Data::MCDataUserIDRequest& request, ::MC::Data::MCDataUserFriendsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::MC::Data::MCDataUserIDRequest, ::MC::Data::MCDataUserFriendsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetUserFriends_, context, request, response);
}

void MCData::Stub::async::GetUserFriends(::grpc::ClientContext* context, const ::MC::Data::MCDataUserIDRequest* request, ::MC::Data::MCDataUserFriendsResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::MC::Data::MCDataUserIDRequest, ::MC::Data::MCDataUserFriendsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUserFriends_, context, request, response, std::move(f));
}

void MCData::Stub::async::GetUserFriends(::grpc::ClientContext* context, const ::MC::Data::MCDataUserIDRequest* request, ::MC::Data::MCDataUserFriendsResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUserFriends_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::MC::Data::MCDataUserFriendsResponse>* MCData::Stub::PrepareAsyncGetUserFriendsRaw(::grpc::ClientContext* context, const ::MC::Data::MCDataUserIDRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::MC::Data::MCDataUserFriendsResponse, ::MC::Data::MCDataUserIDRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetUserFriends_, context, request);
}

::grpc::ClientAsyncResponseReader< ::MC::Data::MCDataUserFriendsResponse>* MCData::Stub::AsyncGetUserFriendsRaw(::grpc::ClientContext* context, const ::MC::Data::MCDataUserIDRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetUserFriendsRaw(context, request, cq);
  result->StartCall();
  return result;
}

MCData::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MCData_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MCData::Service, ::MC::Data::MCDataUserRequest, ::MC::Data::MCDataUserResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MCData::Service* service,
             ::grpc::ServerContext* ctx,
             const ::MC::Data::MCDataUserRequest* req,
             ::MC::Data::MCDataUserResponse* resp) {
               return service->GetUserPassword(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MCData_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MCData::Service, ::MC::Data::MCDataRegistRequest, ::MC::Data::MCDataRegistResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MCData::Service* service,
             ::grpc::ServerContext* ctx,
             const ::MC::Data::MCDataRegistRequest* req,
             ::MC::Data::MCDataRegistResponse* resp) {
               return service->TryRegist(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MCData_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MCData::Service, ::MC::Data::MCDataUserIDRequest, ::MC::Data::MCDataUserFriendsResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MCData::Service* service,
             ::grpc::ServerContext* ctx,
             const ::MC::Data::MCDataUserIDRequest* req,
             ::MC::Data::MCDataUserFriendsResponse* resp) {
               return service->GetUserFriends(ctx, req, resp);
             }, this)));
}

MCData::Service::~Service() {
}

::grpc::Status MCData::Service::GetUserPassword(::grpc::ServerContext* context, const ::MC::Data::MCDataUserRequest* request, ::MC::Data::MCDataUserResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MCData::Service::TryRegist(::grpc::ServerContext* context, const ::MC::Data::MCDataRegistRequest* request, ::MC::Data::MCDataRegistResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MCData::Service::GetUserFriends(::grpc::ServerContext* context, const ::MC::Data::MCDataUserIDRequest* request, ::MC::Data::MCDataUserFriendsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace MC
}  // namespace Data

