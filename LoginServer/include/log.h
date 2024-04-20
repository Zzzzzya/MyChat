// logger.h
#pragma once

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

// 日志的配置项
struct LogConfig {
    std::string level;
    std::string path;
    int64_t size;
    int count;
};

// 日志的单例模式
class Logger {
public:
    static Logger* getInstance() {
        static Logger instance;
        return &instance;
    }

    // c++14返回值可设置为auto
    std::shared_ptr<spdlog::logger> getLogger() { return loggerPtr; }

    void Init(const LogConfig& conf);

    std::string GetLogLevel();

    void SetLogLevel(const std::string& level);

private:
    Logger() = default;
    std::shared_ptr<spdlog::logger> loggerPtr;
};

// 日志相关操作的宏封装
#define INITLOG(conf) Logger::getInstance()->Init(conf)
#define GETLOGLEVEL() Logger::getInstance()->GetLogLevel()
#define SETLOGLEVEL(level) Logger::getInstance()->SetLogLevel(level)
#define BASELOG(logger, level, ...) (logger)->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, level, __VA_ARGS__)
#define LOG_TRACE(...) BASELOG(Logger::getInstance()->getLogger(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...) BASELOG(Logger::getInstance()->getLogger(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...) BASELOG(Logger::getInstance()->getLogger(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...) BASELOG(Logger::getInstance()->getLogger(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...) BASELOG(Logger::getInstance()->getLogger(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...) BASELOG(Logger::getInstance()->getLogger(), spdlog::level::critical, __VA_ARGS__)
