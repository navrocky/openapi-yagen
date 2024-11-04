#include "console_logger.h"

#include <cstdint>
#include <format>
#include <iostream>

#include <termcolor/termcolor.hpp>

namespace tc = termcolor;

using namespace std;

void ConsoleLogger::write(LogFacade::LogLevel level, const std::string& name, const std::string& message)
{
    string levelName;

    switch (level) {
        case LogFacade::LogLevel::DEBUG:
            cout << tc::bright_grey;
            levelName = "DEBUG";
            break;
        case LogFacade::LogLevel::INFO:
            cout << tc::bright_blue;
            levelName = "INFO";
            break;
        case LogFacade::LogLevel::WARN:
            cout << tc::yellow;
            levelName = "WARN";
            break;
        case LogFacade::LogLevel::ERROR:
            cout << tc::bright_red;
            levelName = "ERROR";
            break;
    }
    cout << format("[{:5}]", levelName) << tc::reset << format(" {} - {}", name, message) << endl;
}
