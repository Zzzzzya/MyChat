/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#include "MC.Login.grpc.pb.h"
#include "MC.Login.pb.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

ABSL_FLAG(std::string, target, "localhost:50051", "Server address");

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using MC::Login::MCLogin;
using MC::Login::MCRegistRequest;
using MC::Login::MCRegistResponse;

class RegistClient {
public:
    RegistClient(std::shared_ptr<Channel> channel)
        : stub_(MCLogin::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string Regist(const std::string& username, const std::string& password,
                       const std::string& nickname, const std::string& email) {
        // Data we are sending to the server.
        MCRegistRequest request;
        request.set_username(username);
        request.set_password(password);
        request.set_nickname(nickname);
        request.set_email(email);

        // Container for the data we expect from the server.
        MCRegistResponse reply;

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->Regist(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.code() + ": " + reply.err_msg();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MCLogin::Stub> stub_;
};

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    std::string target_str = absl::GetFlag(FLAGS_target);
    RegistClient greeter(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    std::string username, password, nickname, email;
    std::cout << "Please input username: ";
    std::cin >> username;
    std::cout << "Please input password: ";
    std::cin >> password;
    std::cout << "Please input nickname: ";
    std::cin >> nickname;
    std::cout << "Please input email: ";
    std::cin >> email;

    std::string reply = greeter.Regist(username, password, nickname, email);
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}
