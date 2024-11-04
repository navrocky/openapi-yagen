#include "logger.h"

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
        case LogLevel::DEBUG:
            return currentLogLevel == LogLevel::DEBUG;
        case LogLevel::INFO:
            return currentLogLevel == LogLevel::DEBUG || currentLogLevel == LogLevel::INFO;
        case LogLevel::WARN:
            return currentLogLevel == LogLevel::DEBUG || currentLogLevel == LogLevel::INFO
                || currentLogLevel == LogLevel::WARN;
        case LogLevel::ERROR:
            return currentLogLevel == LogLevel::DEBUG || currentLogLevel == LogLevel::INFO
                || currentLogLevel == LogLevel::WARN || currentLogLevel == LogLevel::WARN;
    }
    return false;
}

void setLogBackend(LoggerBackend* backend) { loggerBackend = backend; }

void setLogLevel(LogLevel level) { currentLogLevel = level; }

}
