#pragma once
#include <debug.h>

#include "log.h"

// grpc库
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "absl/strings/str_format.h"
// my protos
#include "MC.Data.grpc.pb.h"
#include "MC.Data.pb.h"

// namespace
//  grpc
using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

//  MC::Data
using MC::Data::MCData;
using MC::Data::MCDataUserRequest;
using MC::Data::MCDataUserResponse;
using MC::Data::MCResponseStatusCode;

// 异步数据库处理服务端构建
class DataServer final {
public:
    DataServer() = default;
    ~DataServer();

    // 运行服务端 run = bind builder + HandleRpcs
    void Run(uint16_t port);

private:
    struct CallData {
    public:
        CallData(MCData::AsyncService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), status_(CREATE) {}

        void Proceed();

    protected:
        virtual void creating() { debug(), "virtual creating"; };
        virtual void processing() { debug(), "virtual processing"; };
        virtual void finishing() { debug(), "virtual finishing"; };

        MCData::AsyncService* service_;
        ServerCompletionQueue* cq_;
        ServerContext ctx_;

        enum CallStatus { CREATE, PROCESS, FINISH };

        CallStatus status_;
    };

    struct GetUserPasswordCallData : public CallData {
        GetUserPasswordCallData(MCData::AsyncService* service,
                                ServerCompletionQueue* cq)
            : CallData(service, cq), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "LoginCallData creating";
            status_ = PROCESS;
            service_->RequestGetUserPassword(&ctx_, &request_, &responder_, cq_,
                                             cq_, this);
        }

        void processing() override {
            new GetUserPasswordCallData(service_, cq_);

            // TODO: compelete the function

            auto RetStatus = Status::OK;
            responder_.Finish(response_, RetStatus, this);
            status_ = FINISH;
        }

        void finishing() override {
            CHECK_EQ(status_, FINISH);
            delete this;
        }

    private:
        MCDataUserRequest request_;
        MCDataUserResponse response_;
        ServerAsyncResponseWriter<MCDataUserResponse> responder_;
        int usrid_ = 0;
    };

    void HandleRpcs();

    std::unique_ptr<ServerCompletionQueue> cq_;
    MCData::AsyncService service_;
    std::unique_ptr<Server> server_;
};