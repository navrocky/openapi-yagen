#include "logger.h"

#include "../common/string_tools.h"

using namespace std;

namespace LogFacade {

LoggerBackend* loggerBackend = nullptr;
LogLevel currentLogLevel = LogLevel::INFO;

LoggerBackend::~LoggerBackend() { }

Logger::Logger(const std::string& name)
    : name(name)
{
}

void Logger::write(LogLevel level, const std::string& message)
{
    if (loggerBackend && isLevelEnabled(level))
        loggerBackend->write(level, name, message);
}

bool Logger::isLevelEnabled(LogLevel level) const
{
    switch (level) {
        case LogLevel::TRACE:
            return currentLogLevel == LogLevel::TRACE;
        case LogLevel::DEBUG:
            return currentLogLevel == LogLevel::TRACE || currentLogLevel == LogLevel::DEBUG;
        case LogLevel::INFO:
            return currentLogLevel == LogLevel::TRACE || currentLogLevel == LogLevel::DEBUG
                || currentLogLevel == LogLevel::INFO;
        case LogLevel::WARN:
            return currentLogLevel == LogLevel::TRACE || currentLogLevel == LogLevel::DEBUG
                || currentLogLevel == LogLevel::INFO || currentLogLevel == LogLevel::WARN;
        case LogLevel::ERROR:
            return currentLogLevel == LogLevel::TRACE || currentLogLevel == LogLevel::DEBUG
                || currentLogLevel == LogLevel::INFO || currentLogLevel == LogLevel::WARN
                || currentLogLevel == LogLevel::WARN;
    }
    return false;
}

void setLogBackend(LoggerBackend* backend) { loggerBackend = backend; }

void setLogLevel(LogLevel level) { currentLogLevel = level; }

LogLevel strToLogLevel(const std::string& s)
{
    auto sl = s | ansiToLower();
    if (sl == "trace")
        return LogLevel::TRACE;
    if (sl == "debug")
        return LogLevel::DEBUG;
    if (sl == "info")
        return LogLevel::INFO;
    if (sl == "warn")
        return LogLevel::WARN;
    if (sl == "error")
        return LogLevel::ERROR;
    throw runtime_error(format("<41b1314f> Unsupported log level: {}", s));
}
}
