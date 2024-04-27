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

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
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

private:
    std::unique_ptr<MCData::Stub> stub_;
};
