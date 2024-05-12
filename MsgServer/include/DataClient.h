#pragma once

#include <debug.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

using MC::Data::Friend;
using MC::Data::MCDataUserFriendsResponse;
using MC::Data::MCDataUserIDRequest;

using MC::Data::MCDataUserInfoReq;
using MC::Data::MCDataUserInfoRes;

using MC::Data::MCDataUserHeadReq;
using MC::Data::MCDataUserHeadRes;

using MC::Data::MCDataAddFriendReq;
using MC::Data::MCDataAddFriendRes;

class DataMsgClient {
public:
    static DataMsgClient& GetInstance() {
        static DataMsgClient client(grpc::CreateChannel(
            "localhost:50061", grpc::InsecureChannelCredentials()));

        return client;
    }

    DataMsgClient(std::shared_ptr<Channel> channel)
        : stub_(MCData::NewStub(channel)) {}

    std::string GetUserFriends(const int& userid,
                               MCDataUserFriendsResponse& reply) {
        // Data we are sending to the server.
        MCDataUserIDRequest request;
        request.set_userid(userid);

        // Container for the data we expect from the server.

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->GetUserFriends(&context, request, &reply);

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

    std::string UpdateUserInfo(const int& userid, const std::string& field,
                               const std::string& value) {
        // Data we are sending to the server.
        MCDataUserInfoReq request;
        request.set_userid(userid);
        request.set_field(field);
        request.set_value(value);

        // Container for the data we expect from the server.

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        MCDataUserInfoRes reply;
        // The actual RPC.
        Status status = stub_->UpdateUserInfo(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            auto code = reply.code();
            if (code != MCDataResponseStatusCode::OK) {
                std::cout << "Error: " << reply.err_msg() << std::endl;
                return reply.err_msg();
            }

            return "OK";
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

    std::string UpdateUserHead(const int& userid, const std::string& image) {
        // Data we are sending to the server.
        MCDataUserHeadReq request;
        request.set_userid(userid);
        request.set_image_data(image);

        // Container for the data we expect from the server.

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        MCDataUserHeadRes reply;
        // The actual RPC.
        Status status = stub_->UpdateUserHead(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            auto code = reply.code();
            if (code != MCDataResponseStatusCode::OK) {
                std::cout << "Error: " << reply.err_msg() << std::endl;
                return reply.err_msg();
            }

            return "OK";
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }

    std::string AddFriend(const int& userid, const std::string& friendname) {
        // Data we are sending to the server.
        MCDataAddFriendReq request;
        MCDataAddFriendRes reply;
        request.set_userid(userid);
        request.set_friendname(friendname);

        // Container for the data we expect from the server.

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;
        // The actual RPC.
        Status status = stub_->AddFriend(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            auto code = reply.code();
            if (code != MCDataResponseStatusCode::OK) {
                std::cout << "Error: " << reply.errmsg() << std::endl;
                return reply.errmsg();
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
