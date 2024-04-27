#pragma once

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#include "MC.Data.grpc.pb.h"
#include "MC.Data.pb.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using MC::Data::MCData;
using MC::Data::MCDataRegistRequest;
using MC::Data::MCDataRegistResponse;
using MC::Data::MCDataResponseStatusCode;
using MC::Data::MCDataUserRequest;
using MC::Data::MCDataUserResponse;

class DataLoginClient {
public:
    static DataLoginClient& GetInstance() {
        static DataLoginClient client(grpc::CreateChannel(
            "localhost:50061", grpc::InsecureChannelCredentials()));

        return client;
    }

    DataLoginClient(std::shared_ptr<Channel> channel)
        : stub_(MCData::NewStub(channel)) {}

    std::string GetUserPassword(const std::string& user) {
        // Data we are sending to the server.
        MCDataUserRequest request;
        request.set_username(user);

        // Container for the data we expect from the server.
        MCDataUserResponse reply;

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->GetUserPassword(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            auto code = reply.code();
            if (code != MCDataResponseStatusCode::OK) {
                std::cout << "Error: " << reply.errmsg() << std::endl;
                return "Error";
            }
            return reply.password();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

    std::string TryRegist(const std::string& username,
                          const std::string& password,
                          const std::string& nickname,
                          const std::string& email) {
        // Data we are sending to the server.
        MCDataRegistRequest request;
        request.set_username(username);
        request.set_password(password);
        request.set_nickname(nickname);
        request.set_email(email);

        // Container for the data we expect from the server.
        MCDataRegistResponse reply;

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->TryRegist(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            auto code = reply.code();
            if (code != MCDataResponseStatusCode::OK) {
                std::cout << "Error: " << reply.errmsg() << std::endl;
                return "Error";
            }
            return "OK";
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<MCData::Stub> stub_;
};
