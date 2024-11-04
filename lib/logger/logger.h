#pragma once

#include <format>
#include <string>

namespace LogFacade {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
};

class LoggerBackend {
public:
    virtual ~LoggerBackend();
    virtual void write(LogLevel level, const std::string& name, const std::string& message) = 0;
};

class Logger {
public:
    Logger(const std::string& name);

    void write(LogLevel level, const std::string& message);

    bool isLevelEnabled(LogLevel level) const;

    template <typename... Args>
    inline void debug(std::format_string<Args...> msg, Args&&... args)
    {
        write(LogLevel::DEBUG, std::format(msg, std::forward<Args>(args)...));
    }

    inline void debug(const std::string& msg) { write(LogLevel::DEBUG, msg); }

    template <typename... Args>
    inline void info(std::format_string<Args...> msg, Args&&... args)
    {
        write(LogLevel::INFO, std::format(msg, std::forward<Args>(args)...));
    }

    inline void info(const std::string& message) { write(LogLevel::INFO, message); }

    template <typename... Args>
    inline void error(std::format_string<Args...> msg, Args&&... args)
    {
        write(LogLevel::ERROR, std::format(msg, std::forward<Args>(args)...));
    }

private:
    std::string name;
};

void setLogBackend(LoggerBackend* backend);
void setLogLevel(LogLevel level);

}
