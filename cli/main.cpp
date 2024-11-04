#include <exception>
#include <format>

#include <CLI/CLI.hpp>

#include "commands/generate_command.h"
#include "config.h"
#include <lib/logger/console_logger.h>
#include <lib/logger/logger.h>

using namespace std;
using namespace LogFacade;

using Commands = vector<CommandPtr>;

int main(int argc, char** argv)
{
    ConsoleLogger consoleLogger;
    setLogBackend(&consoleLogger);
    setLogLevel(LogLevel::DEBUG);

    Logger log("main");
    try {
        Commands commands = {
            make_shared<GenerateCommand>(),
        };

        CLI::App app { format("OpenAPI Yet Another Generator (v{})", APP_VERSION) };
        for (const auto& cmd : commands) {
            cmd->reg(app);
        }

        CLI11_PARSE(app, argc, argv);

        return 0;
    } catch (const exception& e) {
        log.error("<88489c35> Error: {}", e.what());
        return 1;
    }
}
