#pragma once

// 库
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

// mysql 库
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>

#include "debug.h"
#include "log.h"

struct MysqlPool;
struct MysqlConn;

// Mysql连接封装
struct MysqlConn {
    sql::Connection* conn;
    MysqlPool* pool;
    sql::Statement* stmt = nullptr;

    MysqlConn(sql::Connection* conn, MysqlPool* pool) : conn(conn), pool(pool) {
        stmt = conn->createStatement();
    }

    ~MysqlConn() {
        if (stmt) {
            delete stmt;
        }
        if (conn) {
            delete conn;
        }
    }

    sql::ResultSet* ExecuteQuery(const std::string& sql) {
        try {
            return stmt->executeQuery(sql);
        } catch (sql::SQLException& e) {
            debug(), "SQLException: ", e.what();
            return nullptr;
        }
    }

    int ExecuteUpdate(const std::string& sql) {
        try {
            return stmt->executeUpdate(sql);
        } catch (sql::SQLException& e) {
            debug(), "SQLException: ", e.what();
            return -1;
        }
    }
};

// Mysql连接池
struct MysqlPool {
private:
    // 连接池
    std::queue<MysqlConn*> pool = {};
    std::unordered_set<MysqlConn*> poolSet = {};
    // 连接池大小
    int poolSize = 0;
    // 锁
    std::mutex mtx;
    // 条件变量
    std::condition_variable cv;
    // 是否关闭
    bool isClosed = false;

    /*------- 连接类信息 --------*/
    std::string server_ip;    // mysql server ip
    std::string server_port;  // mysql server port
    std::string user;         // mysql user
    std::string password;     // mysql password
    std::string database;     // mysql database

public:
    explicit MysqlPool(std::string server_ip, std::string server_port,
                       std::string user, std::string password,
                       std::string database, int poolSize = 2)
        : server_ip(server_ip),
          server_port(server_port),
          user(user),
          password(password),
          database(database),
          poolSize(poolSize) {
        Init();
    }

    ~MysqlPool();

    void Init();
    void Destroy();

    MysqlConn* GetConnection(const int timeout = 0);
    void ReleaseConnection(MysqlConn* conn);
};