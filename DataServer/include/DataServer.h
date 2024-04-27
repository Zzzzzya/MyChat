#pragma once
#include <debug.h>

#include "log.h"
#include "mysqlpool.h"

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
using MC::Data::MCDataResponseStatusCode;
using MC::Data::MCDataUserRequest;
using MC::Data::MCDataUserResponse;

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
        CallData(MCData::AsyncService* service, ServerCompletionQueue* cq,
                 MysqlPool* pool)
            : service_(service), cq_(cq), pool_(pool), status_(CREATE) {}

        void Proceed();

    protected:
        virtual void creating() { debug(), "virtual creating"; };
        virtual void processing() { debug(), "virtual processing"; };
        virtual void finishing() { debug(), "virtual finishing"; };

        MCData::AsyncService* service_;
        ServerCompletionQueue* cq_;
        ServerContext ctx_;
        MysqlPool* pool_;

        enum CallStatus { CREATE, PROCESS, FINISH };

        CallStatus status_;
    };

    struct GetUserPasswordCallData : public CallData {
        GetUserPasswordCallData(MCData::AsyncService* service,
                                ServerCompletionQueue* cq, MysqlPool* pool)
            : CallData(service, cq, pool), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "GetUserPasswordCallData creating";
            status_ = PROCESS;
            service_->RequestGetUserPassword(&ctx_, &request_, &responder_, cq_,
                                             cq_, this);
            debug(), "Creating down";
        }

        void processing() override {
            new GetUserPasswordCallData(service_, cq_, pool_);

            debug(), "Processing!";

            // TODO: compelete the function
            // 拿取username和password以进行比对
            auto username_ = request_.username();
            debug(), "username = ", username_;

            // 请求一个数据库连接
            auto conn = pool_->GetConnection(0);
            if (conn == nullptr) {
                debug(), "!conn";
                response_.set_code(MCDataResponseStatusCode::DATABASE_ERROR);
                response_.set_errmsg("数据库连接失败");
                responder_.Finish(response_, Status::OK, this);
                status_ = FINISH;
                return;
            }

            // 查询数据库
            auto sql = absl::StrFormat("SELECT * FROM User WHERE username='%s'",
                                       username_.c_str());

            auto res = conn->ExecuteQuery(sql);
            debug(), "get res";
            debug(), "res is nullptr: ", (res == nullptr);

            int currenti = 0;
            while (res->next()) {
                response_.set_password(res->getString("md5password"));
                response_.set_hxm(res->getString("hxm"));

                debug(), "i:", currenti++,
                    " password:", res->getString("md5password");
            }

            // 归还一个链接
            pool_->ReleaseConnection(conn);

            auto RetStatus = Status::OK;

            responder_.Finish(response_, RetStatus, this);
            status_ = FINISH;
        }

        void finishing() override {
            CHECK_EQ(status_, FINISH);
            debug(), "finishing";
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
    MysqlPool* pool_;
};