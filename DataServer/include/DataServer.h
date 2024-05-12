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
using MC::Data::MCDataRegistRequest;
using MC::Data::MCDataRegistResponse;
using MC::Data::MCDataResponseStatusCode;
using MC::Data::MCDataUserRequest;
using MC::Data::MCDataUserResponse;

using MC::Data::Friend;
using MC::Data::MCDataUserFriendsResponse;
using MC::Data::MCDataUserIDRequest;

using MC::Data::MCDataUserInfoReq;
using MC::Data::MCDataUserInfoRes;

using MC::Data::MCDataUserHeadReq;
using MC::Data::MCDataUserHeadRes;

using MC::Data::MCDataAddFriendReq;
using MC::Data::MCDataAddFriendRes;

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

    // Login用 拿取对应密码
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
                response_.set_nickname(res->getString("nickname"));
                response_.set_email(res->getString("email"));
                response_.set_gender(res->getBoolean("gender"));
                response_.set_signature(res->getString("signature"));
                response_.set_phone(res->getString("phone"));
                response_.set_birthday(res->getString("birthday"));
                response_.set_userid(res->getInt("id"));
                response_.set_img(res->getString("profile_image"));

                debug(), "email: ", res->getString("email");

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

    // Regist用 检查是否已存在用户
    struct TryRegistCallData : public CallData {
        TryRegistCallData(MCData::AsyncService* service,
                          ServerCompletionQueue* cq, MysqlPool* pool)
            : CallData(service, cq, pool), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "TryRegistCallData creating";
            status_ = PROCESS;
            service_->RequestTryRegist(&ctx_, &request_, &responder_, cq_, cq_,
                                       this);
            debug(), "Creating down";
        }

        void processing() override {
            new TryRegistCallData(service_, cq_, pool_);

            debug(), "Processing!";

            auto username_ = request_.username();
            auto password_ = request_.password();
            auto nickname_ = request_.nickname();
            auto email_ = request_.email();

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
            auto sql = absl::StrFormat(
                "INSERT INTO User(username, md5password, nickname, email) "
                "SELECT '%s','%s', '%s','%s' FROM dual WHERE NOT "
                "EXISTS(SELECT* FROM User WHERE username = '%s' OR nickname = "
                "'%s');",
                username_.c_str(), password_.c_str(), nickname_.c_str(),
                email_.c_str(), username_.c_str(), nickname_.c_str());

            debug(), "sql = ", sql;

            auto res = conn->ExecuteUpdate(sql);

            // 归还一个链接
            pool_->ReleaseConnection(conn);

            if (res == 0) {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_errmsg("用户已存在");
            } else if (res == 1) {
                response_.set_code(MCDataResponseStatusCode::OK);
                response_.set_errmsg("注册成功");
            } else {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_errmsg("未知错误");
            }

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
        MCDataRegistRequest request_;
        MCDataRegistResponse response_;
        ServerAsyncResponseWriter<MCDataRegistResponse> responder_;
        int usrid_ = 0;
    };

    // Msg业务1 获取好友列表
    struct GetFriendsCallData : public CallData {
        GetFriendsCallData(MCData::AsyncService* service,
                           ServerCompletionQueue* cq, MysqlPool* pool)
            : CallData(service, cq, pool), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "GetFriendsCallData creating";
            status_ = PROCESS;
            service_->RequestGetUserFriends(&ctx_, &request_, &responder_, cq_,
                                            cq_, this);
            debug(), "Creating down";
        }

        void processing() override {
            new GetFriendsCallData(service_, cq_, pool_);

            debug(), "Processing!";

            auto userid = request_.userid();

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
            auto sql = absl::StrFormat(
                "SELECT * FROM Friends WHERE UserID1 = %d and Status = 1;",
                userid);

            debug(), "sql = ", sql;

            auto res = conn->ExecuteQuery(sql);

            while (res->next()) {
                auto friendid = res->getInt("UserID2");
                auto lastContactTime = res->getString("lastContactTime");

                // 创建一个新的Friend对象
                Friend* new_friend = response_.add_friends();

                // 设置Friend对象的字段
                new_friend->set_friendid(friendid);

                // 查询User表获取用户名
                auto sql_user = absl::StrFormat(
                    "SELECT nickname,profile_image FROM User WHERE id = %d;",
                    friendid);
                auto res_user = conn->ExecuteQuery(sql_user);
                std::string friendname;
                std::string friendsign;
                if (res_user->next()) {
                    friendname = res_user->getString("nickname");
                    friendsign = res_user->getString("profile_image");
                }

                new_friend->set_friendname(friendname);
                new_friend->set_friendsign(friendsign);
                new_friend->set_lastcontacttime(lastContactTime);

                debug(), "friendsid: ", res->getString("UserID2");

                debug(), " friendsname:", res->getString("friendName");
            }

            // 归还一个链接
            pool_->ReleaseConnection(conn);

            response_.set_code(MCDataResponseStatusCode::OK);
            response_.set_errmsg("获取好友列表成功");

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
        MCDataUserIDRequest request_;
        MCDataUserFriendsResponse response_;
        ServerAsyncResponseWriter<MCDataUserFriendsResponse> responder_;
        int usrid_ = 0;
    };

    // Msg业务2 修改个人信息
    struct UpdateUserInfoCallData : public CallData {
        UpdateUserInfoCallData(MCData::AsyncService* service,
                               ServerCompletionQueue* cq, MysqlPool* pool)
            : CallData(service, cq, pool), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "UpdateUserInfoCallData creating";
            status_ = PROCESS;
            service_->RequestUpdateUserInfo(&ctx_, &request_, &responder_, cq_,
                                            cq_, this);
            debug(), "Creating down";
        }

        void processing() override {
            new UpdateUserInfoCallData(service_, cq_, pool_);

            debug(), "Processing!";

            auto userid = request_.userid();
            auto field = request_.field();
            auto value = request_.value();

            // 请求一个数据库连接
            auto conn = pool_->GetConnection(0);

            if (conn == nullptr) {
                debug(), "!conn";
                response_.set_code(MCDataResponseStatusCode::DATABASE_ERROR);
                response_.set_err_msg("数据库连接失败");
                responder_.Finish(response_, Status::OK, this);
                status_ = FINISH;
                return;
            }

            std::string sql;

            if (field == "nickname") {
                sql = absl::StrFormat(
                    "UPDATE User SET nickname = '%s' WHERE id = %d;",
                    value.c_str(), userid);
            } else if (field == "email") {
                sql = absl::StrFormat(
                    "UPDATE User SET email = '%s' WHERE id = %d;",
                    value.c_str(), userid);
            } else if (field == "gender") {
                sql = absl::StrFormat(
                    "UPDATE User SET gender = '%s' WHERE id = %d;",
                    value.c_str(), userid);
            } else if (field == "signature") {
                sql = absl::StrFormat(
                    "UPDATE User SET signature = '%s' WHERE id = %d;",
                    value.c_str(), userid);
            } else if (field == "phone") {
                sql = absl::StrFormat(
                    "UPDATE User SET phone = '%s' WHERE id = %d;",
                    value.c_str(), userid);
            } else if (field == "birthday") {
                sql = absl::StrFormat(
                    "UPDATE User SET birthday = STR_TO_DATE('%s', "
                    "'%%Y-%%m-%%d') WHERE id = %d;",
                    value.c_str(), userid);
            } else {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_err_msg("字段错误");
                responder_.Finish(response_, Status::OK, this);
                status_ = FINISH;
                return;
            }

            auto res = conn->ExecuteUpdate(sql);

            // 归还一个链接
            pool_->ReleaseConnection(conn);

            if (res == 0) {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_err_msg("昵称重复！");
            } else if (res == 1) {
                response_.set_code(MCDataResponseStatusCode::OK);
                response_.set_err_msg("注册成功");
            } else {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_err_msg("值重复 请更换并重试！");
            }

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
        MCDataUserInfoReq request_;
        MCDataUserInfoRes response_;
        ServerAsyncResponseWriter<MCDataUserInfoRes> responder_;
        int usrid_ = 0;
    };

    // Msg业务3 修改头像
    struct UpdateUserHeadCallData : public CallData {
        UpdateUserHeadCallData(MCData::AsyncService* service,
                               ServerCompletionQueue* cq, MysqlPool* pool)
            : CallData(service, cq, pool), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "UpdateUserHeadCallData creating";
            status_ = PROCESS;
            service_->RequestUpdateUserHead(&ctx_, &request_, &responder_, cq_,
                                            cq_, this);
            debug(), "Creating down";
        }

        void processing() override {
            new UpdateUserHeadCallData(service_, cq_, pool_);

            debug(), "Processing!";

            auto userid = request_.userid();

            // 请求一个数据库连接
            auto conn = pool_->GetConnection(0);

            if (conn == nullptr) {
                debug(), "!conn";
                response_.set_code(MCDataResponseStatusCode::DATABASE_ERROR);
                response_.set_err_msg("数据库连接失败");
                responder_.Finish(response_, Status::OK, this);
                status_ = FINISH;
                return;
            }

            std::string sql;

            sql = absl::StrFormat(
                "UPDATE User SET profile_image = '%s' WHERE id = %d;",
                request_.image_data().c_str(), userid);

            auto res = conn->ExecuteUpdate(sql);

            // 归还一个链接
            pool_->ReleaseConnection(conn);

            if (res == 0) {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_err_msg("昵称重复！");
            } else if (res == 1) {
                response_.set_code(MCDataResponseStatusCode::OK);
                response_.set_err_msg("注册成功");
            } else {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_err_msg("请重试！");
            }

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
        MCDataUserHeadReq request_;
        MCDataUserHeadRes response_;
        ServerAsyncResponseWriter<MCDataUserHeadRes> responder_;
        int usrid_ = 0;
    };

    // Msg业务4 添加好友
    struct AddFriendCallData : public CallData {
        AddFriendCallData(MCData::AsyncService* service,
                          ServerCompletionQueue* cq, MysqlPool* pool)
            : CallData(service, cq, pool), responder_(&ctx_) {
            Proceed();
        }

        void creating() override {
            debug(), "AddFriendCallData";
            status_ = PROCESS;
            service_->RequestAddFriend(&ctx_, &request_, &responder_, cq_, cq_,
                                       this);
            debug(), "Creating down";
        }

        void processing() override {
            new AddFriendCallData(service_, cq_, pool_);

            auto userid = request_.userid();
            auto friendname = request_.friendname();

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
            auto sql = absl::StrFormat(
                "SELECT id FROM User WHERE username = '%s';", friendname);

            debug(), "sql = ", sql;

            auto res = conn->ExecuteQuery(sql);

            int res2 = 0;
            int res3 = 0;

            while (res && res->next()) {
                auto friendid = res->getInt("id");
                auto sql2 = absl::StrFormat(
                    "INSERT INTO Friends (UserID1, UserID2, Status) VALUES "
                    "(%d, %d, 1) "
                    "ON DUPLICATE KEY UPDATE status = 1;",
                    userid, friendid);
                res2 = conn->ExecuteUpdate(sql2);
                auto sql3 = absl::StrFormat(
                    "INSERT INTO Friends (UserID1, UserID2, Status) VALUES "
                    "(%d, %d, 1) "
                    "ON DUPLICATE KEY UPDATE status = 1;",
                    friendid, userid);
                res3 = conn->ExecuteUpdate(sql3);
            }

            // 归还一个链接
            pool_->ReleaseConnection(conn);

            if (!res || res2 == 0 || res3 == 0) {
                response_.set_code(MCDataResponseStatusCode::ERROR);
                response_.set_errmsg("添加好友失败");
            } else {
                response_.set_code(MCDataResponseStatusCode::OK);
                response_.set_errmsg("添加好友成功");
            }

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
        MCDataAddFriendReq request_;
        MCDataAddFriendRes response_;
        ServerAsyncResponseWriter<MCDataAddFriendRes> responder_;
        int usrid_ = 0;
    };

    void HandleRpcs();

    std::unique_ptr<ServerCompletionQueue> cq_;
    MCData::AsyncService service_;
    std::unique_ptr<Server> server_;
    MysqlPool* pool_;
};