#pragma once

#include "logger.h"

class ConsoleLogger : public LogFacade::LoggerBackend {
public:
    void write(LogFacade::LogLevel level, const std::string& name, const std::string& message) override;
};
