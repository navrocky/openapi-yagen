#include "console_logger.h"

#include <chrono>
#include <format>
#include <iostream>

#include <termcolor/termcolor.hpp>

namespace tc = termcolor;

using namespace std;

void ConsoleLogger::write(LogFacade::LogLevel level, const std::string& name, const std::string& message)
{
    string levelName;

    cout << tc::green << format("{:%T} ", chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now()))
         << tc::reset;

    switch (level) {
        case LogFacade::LogLevel::TRACE:
            cout << tc::bright_grey;
            levelName = "TRACE";
            break;
        case LogFacade::LogLevel::DEBUG:
            cout << tc::white;
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
    cout << format("[{:5}]", levelName) << tc::reset << " " << tc::bright_white << name << tc::reset
         << format(" - {}", message) << endl;
}
