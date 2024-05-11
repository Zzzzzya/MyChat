#include "server.h"

MsgServer::~MsgServer() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
}

void MsgServer::Run(uint16_t port) {
    //
    std::string server_address = absl::StrFormat("0.0.0.0:%d", port);

    /*创建builder*/
    // 1.绑定接听端口
    // 2.注册服务
    // 3.绑定完成队列
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    cq_ = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();

    debug(), "Server listening on ", server_address;
    LOG_INFO("Server listening on " + server_address);

    // 处理rpc
    HandleRpcs();
}

void MsgServer::HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new GetFriendsCallData(&service_, cq_.get());
    new UpdateUserInfoCallData(&service_, cq_.get());
    new UpdateUserHeadCallData(&service_, cq_.get());

    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
        // Block waiting to read the next event from the completion queue. The
        // event is uniquely identified by its tag, which in this case is the
        // memory address of a CallData instance.
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the completion queue
        // has been shut down.
        CHECK(cq_->Next(&tag, &ok));
        CHECK(ok);
        static_cast<CallData*>(tag)->Proceed();
    }
}

void MsgServer::CallData::Proceed() {
    if (status_ == CREATE) {
        this->creating();
    } else if (status_ == PROCESS) {
        this->processing();
    } else {
        this->finishing();
    }
}