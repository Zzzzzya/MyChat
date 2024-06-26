// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: MC.Msg.proto

#include "MC.Msg.pb.h"
#include "MC.Msg.grpc.pb.h"

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
namespace Msg {

static const char* MSG_method_names[] = {
  "/MC.Msg.MSG/UpdateUserInfo",
  "/MC.Msg.MSG/UpdateUserHead",
  "/MC.Msg.MSG/GetUserInfo",
  "/MC.Msg.MSG/GetFriends",
  "/MC.Msg.MSG/Chat",
};

std::unique_ptr< MSG::Stub> MSG::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< MSG::Stub> stub(new MSG::Stub(channel, options));
  return stub;
}

MSG::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_UpdateUserInfo_(MSG_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_UpdateUserHead_(MSG_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetUserInfo_(MSG_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetFriends_(MSG_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Chat_(MSG_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::Status MSG::Stub::UpdateUserInfo(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserInfoReq& request, ::MC::Msg::UpdateUserInfoRes* response) {
  return ::grpc::internal::BlockingUnaryCall< ::MC::Msg::UpdateUserInfoReq, ::MC::Msg::UpdateUserInfoRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_UpdateUserInfo_, context, request, response);
}

void MSG::Stub::async::UpdateUserInfo(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserInfoReq* request, ::MC::Msg::UpdateUserInfoRes* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::MC::Msg::UpdateUserInfoReq, ::MC::Msg::UpdateUserInfoRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UpdateUserInfo_, context, request, response, std::move(f));
}

void MSG::Stub::async::UpdateUserInfo(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserInfoReq* request, ::MC::Msg::UpdateUserInfoRes* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UpdateUserInfo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::UpdateUserInfoRes>* MSG::Stub::PrepareAsyncUpdateUserInfoRaw(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserInfoReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::MC::Msg::UpdateUserInfoRes, ::MC::Msg::UpdateUserInfoReq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_UpdateUserInfo_, context, request);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::UpdateUserInfoRes>* MSG::Stub::AsyncUpdateUserInfoRaw(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserInfoReq& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncUpdateUserInfoRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MSG::Stub::UpdateUserHead(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserHeadReq& request, ::MC::Msg::UpdateUserHeadRes* response) {
  return ::grpc::internal::BlockingUnaryCall< ::MC::Msg::UpdateUserHeadReq, ::MC::Msg::UpdateUserHeadRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_UpdateUserHead_, context, request, response);
}

void MSG::Stub::async::UpdateUserHead(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserHeadReq* request, ::MC::Msg::UpdateUserHeadRes* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::MC::Msg::UpdateUserHeadReq, ::MC::Msg::UpdateUserHeadRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UpdateUserHead_, context, request, response, std::move(f));
}

void MSG::Stub::async::UpdateUserHead(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserHeadReq* request, ::MC::Msg::UpdateUserHeadRes* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_UpdateUserHead_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::UpdateUserHeadRes>* MSG::Stub::PrepareAsyncUpdateUserHeadRaw(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserHeadReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::MC::Msg::UpdateUserHeadRes, ::MC::Msg::UpdateUserHeadReq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_UpdateUserHead_, context, request);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::UpdateUserHeadRes>* MSG::Stub::AsyncUpdateUserHeadRaw(::grpc::ClientContext* context, const ::MC::Msg::UpdateUserHeadReq& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncUpdateUserHeadRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MSG::Stub::GetUserInfo(::grpc::ClientContext* context, const ::MC::Msg::GetUserInfoReq& request, ::MC::Msg::GetUserInfoRes* response) {
  return ::grpc::internal::BlockingUnaryCall< ::MC::Msg::GetUserInfoReq, ::MC::Msg::GetUserInfoRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetUserInfo_, context, request, response);
}

void MSG::Stub::async::GetUserInfo(::grpc::ClientContext* context, const ::MC::Msg::GetUserInfoReq* request, ::MC::Msg::GetUserInfoRes* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::MC::Msg::GetUserInfoReq, ::MC::Msg::GetUserInfoRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUserInfo_, context, request, response, std::move(f));
}

void MSG::Stub::async::GetUserInfo(::grpc::ClientContext* context, const ::MC::Msg::GetUserInfoReq* request, ::MC::Msg::GetUserInfoRes* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetUserInfo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::GetUserInfoRes>* MSG::Stub::PrepareAsyncGetUserInfoRaw(::grpc::ClientContext* context, const ::MC::Msg::GetUserInfoReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::MC::Msg::GetUserInfoRes, ::MC::Msg::GetUserInfoReq, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetUserInfo_, context, request);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::GetUserInfoRes>* MSG::Stub::AsyncGetUserInfoRaw(::grpc::ClientContext* context, const ::MC::Msg::GetUserInfoReq& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetUserInfoRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status MSG::Stub::GetFriends(::grpc::ClientContext* context, const ::MC::Msg::UserID& request, ::MC::Msg::UserIDList* response) {
  return ::grpc::internal::BlockingUnaryCall< ::MC::Msg::UserID, ::MC::Msg::UserIDList, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetFriends_, context, request, response);
}

void MSG::Stub::async::GetFriends(::grpc::ClientContext* context, const ::MC::Msg::UserID* request, ::MC::Msg::UserIDList* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::MC::Msg::UserID, ::MC::Msg::UserIDList, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetFriends_, context, request, response, std::move(f));
}

void MSG::Stub::async::GetFriends(::grpc::ClientContext* context, const ::MC::Msg::UserID* request, ::MC::Msg::UserIDList* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetFriends_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::UserIDList>* MSG::Stub::PrepareAsyncGetFriendsRaw(::grpc::ClientContext* context, const ::MC::Msg::UserID& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::MC::Msg::UserIDList, ::MC::Msg::UserID, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetFriends_, context, request);
}

::grpc::ClientAsyncResponseReader< ::MC::Msg::UserIDList>* MSG::Stub::AsyncGetFriendsRaw(::grpc::ClientContext* context, const ::MC::Msg::UserID& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetFriendsRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientReaderWriter< ::MC::Msg::Message, ::MC::Msg::Message>* MSG::Stub::ChatRaw(::grpc::ClientContext* context) {
  return ::grpc::internal::ClientReaderWriterFactory< ::MC::Msg::Message, ::MC::Msg::Message>::Create(channel_.get(), rpcmethod_Chat_, context);
}

void MSG::Stub::async::Chat(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::MC::Msg::Message,::MC::Msg::Message>* reactor) {
  ::grpc::internal::ClientCallbackReaderWriterFactory< ::MC::Msg::Message,::MC::Msg::Message>::Create(stub_->channel_.get(), stub_->rpcmethod_Chat_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::MC::Msg::Message, ::MC::Msg::Message>* MSG::Stub::AsyncChatRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::MC::Msg::Message, ::MC::Msg::Message>::Create(channel_.get(), cq, rpcmethod_Chat_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::MC::Msg::Message, ::MC::Msg::Message>* MSG::Stub::PrepareAsyncChatRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderWriterFactory< ::MC::Msg::Message, ::MC::Msg::Message>::Create(channel_.get(), cq, rpcmethod_Chat_, context, false, nullptr);
}

MSG::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MSG_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MSG::Service, ::MC::Msg::UpdateUserInfoReq, ::MC::Msg::UpdateUserInfoRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MSG::Service* service,
             ::grpc::ServerContext* ctx,
             const ::MC::Msg::UpdateUserInfoReq* req,
             ::MC::Msg::UpdateUserInfoRes* resp) {
               return service->UpdateUserInfo(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MSG_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MSG::Service, ::MC::Msg::UpdateUserHeadReq, ::MC::Msg::UpdateUserHeadRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MSG::Service* service,
             ::grpc::ServerContext* ctx,
             const ::MC::Msg::UpdateUserHeadReq* req,
             ::MC::Msg::UpdateUserHeadRes* resp) {
               return service->UpdateUserHead(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MSG_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MSG::Service, ::MC::Msg::GetUserInfoReq, ::MC::Msg::GetUserInfoRes, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MSG::Service* service,
             ::grpc::ServerContext* ctx,
             const ::MC::Msg::GetUserInfoReq* req,
             ::MC::Msg::GetUserInfoRes* resp) {
               return service->GetUserInfo(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MSG_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< MSG::Service, ::MC::Msg::UserID, ::MC::Msg::UserIDList, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](MSG::Service* service,
             ::grpc::ServerContext* ctx,
             const ::MC::Msg::UserID* req,
             ::MC::Msg::UserIDList* resp) {
               return service->GetFriends(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      MSG_method_names[4],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< MSG::Service, ::MC::Msg::Message, ::MC::Msg::Message>(
          [](MSG::Service* service,
             ::grpc::ServerContext* ctx,
             ::grpc::ServerReaderWriter<::MC::Msg::Message,
             ::MC::Msg::Message>* stream) {
               return service->Chat(ctx, stream);
             }, this)));
}

MSG::Service::~Service() {
}

::grpc::Status MSG::Service::UpdateUserInfo(::grpc::ServerContext* context, const ::MC::Msg::UpdateUserInfoReq* request, ::MC::Msg::UpdateUserInfoRes* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MSG::Service::UpdateUserHead(::grpc::ServerContext* context, const ::MC::Msg::UpdateUserHeadReq* request, ::MC::Msg::UpdateUserHeadRes* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MSG::Service::GetUserInfo(::grpc::ServerContext* context, const ::MC::Msg::GetUserInfoReq* request, ::MC::Msg::GetUserInfoRes* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MSG::Service::GetFriends(::grpc::ServerContext* context, const ::MC::Msg::UserID* request, ::MC::Msg::UserIDList* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status MSG::Service::Chat(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::MC::Msg::Message, ::MC::Msg::Message>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace MC
}  // namespace Msg

