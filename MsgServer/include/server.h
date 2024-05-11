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
#include "MC.Msg.grpc.pb.h"
#include "MC.Msg.pb.h"

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

//  MC::Msg
using MC::Msg::MCResponseStatusCode;
using MC::Msg::MSG;
using MC::Msg::MsgFriend;
using MC::Msg::UpdateUserInfoReq;
using MC::Msg::UpdateUserInfoRes;
using MC::Msg::UserID;
using MC::Msg::UserIDList;
using MC::Msg::UpdateUserHeadReq;
using MC::Msg::UpdateUserHeadRes;

class MsgServer final {
public:
    MsgServer() = default;
    ~MsgServer();

    // 运行服务端 run = bind builder + HandleRpcs
    void Run(uint16_t port);

private:
    struct CallData {
    public:
        CallData(MSG::AsyncService* service, ServerCompletionQueue* cq)
            : service_(service), cq_(cq), status_(CREATE) {}

        void Proceed();

    protected:
        virtual void creating() { debug(), "virtual creating"; };
        virtual void processing() { debug(), "virtual processing"; };
        virtual void finishing() { debug(), "virtual finishing"; };

        MSG::AsyncService* service_;
        ServerCompletionQueue* cq_;
        ServerContext ctx_;

        enum CallStatus { CREATE, PROCESS, FINISH };

        CallStatus status_;
    };

    // 业务1 : 获取好友列表
    struct GetFriendsCallData : public CallData {
        GetFriendsCallData(MSG::AsyncService* service,
                           ServerCompletionQueue* cq)
            : CallData(service, cq), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "GetFriends CallData creating";
            status_ = PROCESS;
            service_->RequestGetFriends(&ctx_, &request_, &responder_, cq_, cq_,
                                        this);
        }

        void processing() override {
            new GetFriendsCallData(service_, cq_);

            auto userid = request_.userid();
            debug(), "GetFriendsReq: userid: ", userid;

            auto RetStatus = Status::OK;

            MCDataUserFriendsResponse reply;

            auto ret =
                DataMsgClient::GetInstance().GetUserFriends(userid, reply);

            // 2. Check password
            if (ret == "OK") {
                response_.set_code(MCResponseStatusCode::OK);
                response_.set_errmsg("OK");

                for (auto& friend_ : reply.friends()) {
                    auto* f = response_.add_friends();
                    f->set_friendid(friend_.friendid());
                    f->set_friendname(friend_.friendname());
                    f->set_friendsign(friend_.friendsign());
                    f->set_lastcontacttime(friend_.lastcontacttime());
                    debug(), friend_.friendid(), friend_.friendname(),
                        friend_.friendsign(), friend_.lastcontacttime();
                }

                RetStatus = Status::OK;

            } else if (ret == "RPC failed") {
                response_.set_code(MCResponseStatusCode::ERROR);
                response_.set_errmsg("RPC failed");
                RetStatus = Status::OK;
            } else {
                response_.set_code(MCResponseStatusCode::ERROR);
                response_.set_errmsg("User not found");
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
        UserID request_;
        UserIDList response_;
        ServerAsyncResponseWriter<UserIDList> responder_;
    };

    // 业务2 : 修改个人信息
    struct UpdateUserInfoCallData : public CallData {
        UpdateUserInfoCallData(MSG::AsyncService* service,
                               ServerCompletionQueue* cq)
            : CallData(service, cq), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "UpdateUserInfoCallData creating";
            status_ = PROCESS;
            service_->RequestUpdateUserInfo(&ctx_, &request_, &responder_, cq_,
                                            cq_, this);
        }

        void processing() override {
            new UpdateUserInfoCallData(service_, cq_);

            auto userid = request_.userid();
            auto field = request_.field();
            auto value = request_.value();

            auto RetStatus = Status::OK;

            auto ret = DataMsgClient::GetInstance().UpdateUserInfo(
                userid, field, value);

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
                response_.set_err_msg(ret);
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
        UpdateUserInfoReq request_;
        UpdateUserInfoRes response_;
        ServerAsyncResponseWriter<UpdateUserInfoRes> responder_;
    };

    // 业务3 : 修改头像
    struct UpdateUserHeadCallData : public CallData {
        UpdateUserHeadCallData(MSG::AsyncService* service,
                               ServerCompletionQueue* cq)
            : CallData(service, cq), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "UpdateUserHeadCallData creating";
            status_ = PROCESS;
            service_->RequestUpdateUserHead(&ctx_, &request_, &responder_, cq_,
                                            cq_, this);
        }

        void processing() override {
            new UpdateUserHeadCallData(service_, cq_);

            auto userid = request_.userid();
            auto image_data = request_.image_data();

            auto RetStatus = Status::OK;

            auto ret = DataMsgClient::GetInstance().UpdateUserHead(
                userid, image_data);

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
                response_.set_err_msg(ret);
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
        UpdateUserHeadReq request_;
        UpdateUserHeadRes response_;
        ServerAsyncResponseWriter<UpdateUserHeadRes> responder_;
    };

    void HandleRpcs();

    std::unique_ptr<ServerCompletionQueue> cq_;
    MSG::AsyncService service_;
    std::unique_ptr<Server> server_;
};