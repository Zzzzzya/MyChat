#include "debug.h"

// grpc server
#include "server.h"

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

int main() {
    // 程序启动
    debug(), "LoginServer start...";

    // 初始化日志
    LogConfig conf2 = {
        .level = "trace",
        .path = "../log/Login.log",
        .size = 5 * 1024 * 1024,
        .count = 10,
    };
    INITLOG(conf2);

    // 启动服务
    LoginServer server;
    server.Run(absl::GetFlag(FLAGS_port));

    return 0;
}