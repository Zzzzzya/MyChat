#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "MC.Login.grpc.pb.h"
#include "MC.Login.pb.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/check.h"
#include "debug.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

//  MC::Login
using MC::Login::MCLogin;
using MC::Login::MCLoginRequest;
using MC::Login::MCLoginResponse;
using MC::Login::MCResponseStatusCode;

ABSL_FLAG(std::string, target, "localhost:50051", "Server address");

class LoginClient {
public:
    explicit LoginClient(std::shared_ptr<Channel> channel)
        : stub_(MCLogin::NewStub(channel)) {}

    void Login(const std::string& username, const std::string& password,
               const MC::Login::UserStatType& online_status,
               const std::string& client_version) {
        MCLoginRequest request;
        request.set_username(username);
        request.set_password(password);
        request.set_online_status(online_status);
        request.set_client_version(client_version);

        LoginAsyncClientCall* call = new LoginAsyncClientCall;
        call->response_reader =
            stub_->PrepareAsyncLogin(&call->context, request, &cq_);
        call->response_reader->StartCall();

        call->response_reader->Finish(&call->response, &call->status,
                                      (void*)call);
    }

    void AsyncCompleteRpc() {
        void* got_tag;
        bool ok = false;

        while (cq_.Next(&got_tag, &ok)) {
            auto call = static_cast<AsyncClientCall*>(got_tag);

            CHECK(ok);
            call->Proceed();

            debug(), "deal one response";
            delete call;
        }
    }

private:
    // 服务回应基类
    struct AsyncClientCall {
        ClientContext context;
        Status status;
        virtual ~AsyncClientCall() {}
        // 处理回应信息
        virtual void Proceed() = 0;
    };

    // 实际的服务回应
    struct LoginAsyncClientCall : virtual public AsyncClientCall {
        std::unique_ptr<ClientAsyncResponseReader<MCLoginResponse>>
            response_reader;
        MCLoginResponse response;

        ~LoginAsyncClientCall() override {}
        void Proceed() override {
            debug(), "MCLogin proceed";
            auto code = response.code();
            if (code == MCResponseStatusCode::FAILED) {
                debug(), "Login Failed";
                debug(), "err_message: ", response.err_msg();
                return;
            } else if (code == MCResponseStatusCode::OK) {
                auto err_msg = response.err_msg();
                auto user_id = response.user_id();
                auto server_time = response.server_time();

                debug(), "Login Success";
                debug(), "usr_id: ", user_id;
                debug(), "server_time: ", server_time;
            }
        }
    };

    CompletionQueue cq_;

    std::unique_ptr<MCLogin::Stub> stub_;
};

int main() {
    std::string target_str = absl::GetFlag(FLAGS_target);
    LoginClient client(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
    std::thread thread_ = std::thread(&LoginClient::AsyncCompleteRpc, &client);

    while (true) {
        std::string username, password;
        std::string client_version;
        client_version = "1.0.0";

        std::cout << "Please input username: ";
        std::cin >> username;
        std::cout << "Please input password: ";
        std::cin >> password;
        client.Login(username, password, MC::Login::UserStatType::ONLINE,
                     client_version);
    }

    thread_.join();
    return 0;
}