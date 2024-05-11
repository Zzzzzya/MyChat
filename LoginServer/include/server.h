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
#include "MC.Data.pb.h"
#include "MC.Login.grpc.pb.h"
#include "MC.Login.pb.h"

// client
#include "DataClient.h"

// namespace
//  grpc
using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

//  MC::Login
using MC::Login::MCLogin;
using MC::Login::MCLoginRequest;
using MC::Login::MCLoginResponse;
using MC::Login::MCRegistRequest;
using MC::Login::MCRegistResponse;
using MC::Login::MCResponseStatusCode;

// 异步注册服务端构建
class LoginServer final {
public:
    LoginServer() = default;
    ~LoginServer();

    // 运行服务端 run = bind builder + HandleRpcs
    void Run(uint16_t port);

private:
    struct CallData {
    public:
        CallData(MCLogin::AsyncService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), status_(CREATE) {}

        void Proceed();

    protected:
        virtual void creating() { debug(), "virtual creating"; };
        virtual void processing() { debug(), "virtual processing"; };
        virtual void finishing() { debug(), "virtual finishing"; };

        MCLogin::AsyncService* service_;
        ServerCompletionQueue* cq_;
        ServerContext ctx_;

        enum CallStatus { CREATE, PROCESS, FINISH };

        CallStatus status_;
    };

    // 登录业务
    struct LoginCallData : public CallData {
        LoginCallData(MCLogin::AsyncService* service, ServerCompletionQueue* cq)
            : CallData(service, cq), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "LoginCallData creating";
            status_ = PROCESS;
            service_->RequestLogin(&ctx_, &request_, &responder_, cq_, cq_,
                                   this);
        }

        void processing() override {
            new LoginCallData(service_, cq_);

            auto username = request_.username();
            auto password = request_.password();
            auto online = request_.online_status();
            auto client_version = request_.client_version();

            debug(), "Login request: ", username, " ", password, " ", online,
                " ", client_version;

            auto RetStatus = Status::OK;
            std::vector<std::string> res;
            // 1. Get password

            DataLoginClient::GetInstance().GetUserPassword(username, res);
            auto password_ret = res[0];
            debug(), "password_Ret= ", password_ret;

            // 2. Check password
            if (password_ret == "Error") {
                response_.set_code(MCResponseStatusCode::ERROR);
                response_.set_err_msg("User not found");
                RetStatus = Status::OK;
            } else if (password_ret == "RPC failed") {
                response_.set_code(MCResponseStatusCode::ERROR);
                response_.set_err_msg("RPC failed");
                RetStatus = Status::OK;
            } else if (password_ret != password) {
                response_.set_code(MCResponseStatusCode::ERROR);
                response_.set_err_msg("Password not match");
                RetStatus = Status::OK;
            } else {
                response_.set_code(MCResponseStatusCode::OK);
                response_.set_err_msg("OK");

                response_.set_nickname(res[1]);
                response_.set_email(res[2]);
                response_.set_gender(res[3]);
                response_.set_signature(res[4]);
                response_.set_phone(res[5]);
                response_.set_birthday(res[6]);
                response_.set_user_id(std::stoi(res[8]));
                response_.set_img(res[7]);

                response_.set_msg_server_ip("192.168.146.131:50088");

                debug(), "res[3] = ", res[3];

                RetStatus = Status::OK;
            }

            responder_.Finish(response_, RetStatus, this);
            status_ = FINISH;
        }

        void finishing() override {
            CHECK_EQ(status_, FINISH);
            delete this;
        }

    private:
        MCLoginRequest request_;
        MCLoginResponse response_;
        ServerAsyncResponseWriter<MCLoginResponse> responder_;
        int usrid_ = 0;
    };

    // 注册业务
    struct RegisterCallData : public CallData {
        RegisterCallData(MCLogin::AsyncService* service,
                         ServerCompletionQueue* cq)
            : CallData(service, cq), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "Register CallData creating";
            status_ = PROCESS;
            service_->RequestRegist(&ctx_, &request_, &responder_, cq_, cq_,
                                    this);
        }

        void processing() override {
            new RegisterCallData(service_, cq_);

            auto username = request_.username();
            auto password = request_.password();
            auto nickname = request_.nickname();
            auto email = request_.email();

            debug(), "Regist request: ", username, " ", password, " ", nickname;

            auto RetStatus = Status::OK;

            auto ret = DataLoginClient::GetInstance().TryRegist(
                username, password, nickname, email);

            // 2. Check password
            if (ret == "OK") {
                response_.set_code(MCResponseStatusCode::OK);
                response_.set_err_msg("OK");
                RetStatus = Status::OK;

            } else if (ret == "RPC failed") {
                response_.set_code(MCResponseStatusCode::ERROR);
                response_.set_err_msg("RPC failed");
                RetStatus = Status::OK;
            } else {
                response_.set_code(MCResponseStatusCode::ERROR);
                response_.set_err_msg("User Exits");
                RetStatus = Status::OK;
            }

            responder_.Finish(response_, RetStatus, this);
            status_ = FINISH;
        }

        void finishing() override {
            CHECK_EQ(status_, FINISH);
            delete this;
        }

    private:
        MCRegistRequest request_;
        MCRegistResponse response_;
        ServerAsyncResponseWriter<MCRegistResponse> responder_;
    };

    void HandleRpcs();

    std::unique_ptr<ServerCompletionQueue> cq_;
    MCLogin::AsyncService service_;
    std::unique_ptr<Server> server_;
};