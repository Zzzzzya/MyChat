#include "mysqlpool.h"

MysqlPool::~MysqlPool() { Destroy(); }

void MysqlPool::Init() {
    for (int i = 0; i < poolSize; i++) {
        sql::mysql::MySQL_Driver* driver =
            sql::mysql::get_mysql_driver_instance();

        std::string url = "tcp://" + server_ip + ":" + server_port;

        sql::Connection* conn = driver->connect(url, user, password);
        debug(), "new Connection!";
        conn->setSchema(database);
        auto newConn = new MysqlConn(conn, this);
        pool.push(newConn);
        poolSet.insert(newConn);
    }
}

void MysqlPool::Destroy() {
    // TODO: lock
    std::lock_guard<std::mutex> lock(mtx);

    isClosed = true;
    cv.notify_all();

    while (!pool.empty()) {
        MysqlConn* conn = pool.front();
        poolSet.erase(conn);
        pool.pop();
        delete conn;
    }
}

MysqlConn* MysqlPool::GetConnection(const int timeout) {
    std::unique_lock<std::mutex> lock(mtx);
    if (pool.empty()) {
        if (timeout <= 0) {
            cv.wait(lock, [this] { return !pool.empty() || isClosed; });
            if (pool.empty())  // 如果连接池还是没有空闲则退出
            {
                return nullptr;
            }

            MysqlConn* conn = pool.front();
            pool.pop();
            poolSet.erase(conn);
            return conn;
        } else {
            cv.wait_for(lock, std::chrono::milliseconds(timeout),
                        [this] { return !pool.empty() || isClosed; });

            if (pool.empty())  // 如果连接池还是没有空闲则退出
            {
                return nullptr;
            }
            MysqlConn* conn = pool.front();
            pool.pop();
            poolSet.erase(conn);
            return conn;
        }
    }

    // 非空
    MysqlConn* conn = pool.front();
    pool.pop();
    poolSet.erase(conn);
    return conn;
}

void MysqlPool::ReleaseConnection(MysqlConn* conn) {
    if (conn == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(mtx);

    // 检查set，避免重复归还
    if (poolSet.find(conn) != poolSet.end()) {
        debug(), "避免重复归还";
        return;
    }

    pool.push(conn);
    poolSet.insert(conn);

    cv.notify_one();
    return;
}