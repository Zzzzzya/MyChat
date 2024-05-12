#include "DataServer.h"

DataServer::~DataServer() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
    if (pool_) delete pool_;
}

void DataServer::Run(uint16_t port) {
    // MysqlPool的初始化
    // 初始化mysql连接池
    pool_ =
        new MysqlPool("127.0.0.1", "3306", "r-zya", "zya20040429", "MyChat");

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

void DataServer::HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new GetUserPasswordCallData(&service_, cq_.get(), pool_);
    new TryRegistCallData(&service_, cq_.get(), pool_);
    new GetFriendsCallData(&service_, cq_.get(), pool_);
    new UpdateUserInfoCallData(&service_, cq_.get(), pool_);
    new UpdateUserHeadCallData(&service_, cq_.get(), pool_);
    new AddFriendCallData(&service_, cq_.get(), pool_);
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
        debug(), "Get one!";
        static_cast<CallData*>(tag)->Proceed();
    }
}

void DataServer::CallData::Proceed() {
    if (status_ == CREATE) {
        this->creating();
    } else if (status_ == PROCESS) {
        this->processing();
    } else {
        this->finishing();
    }
}