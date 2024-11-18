#include <exception>
#include <format>

#include <CLI/CLI.hpp>

#include <lib/common/std_tools.h>
#include <lib/logger/console_logger.h>
#include <lib/logger/logger.h>

#include "commands/generate_command.h"
#include "config.h"

using namespace std;
using namespace LogFacade;

using Commands = vector<CommandPtr>;

int main(int argc, char** argv)
{
    ConsoleLogger consoleLogger;
    setLogBackend(&consoleLogger);
    setLogLevel(LogLevel::INFO);

    Logger log("main");
    try {
        Commands commands = {
            make_shared<GenerateCommand>(),
        };

        CLI::App app { format("OpenAPI Yet Another Generator (v{})", APP_VERSION) };
        for (const auto& cmd : commands) {
            cmd->reg(app);
        }
        app.require_subcommand();
        app.add_option(
               "-l, --log-level",
               [](const auto& v) {
                   setLogLevel(LogFacade::strToLogLevel(v | firstOrThrow()));
                   return true;
               },
               "Set log level. Supported values: TRACE, DEBUG, INFO, WARN, ERROR")
            ->default_val("INFO");

        CLI11_PARSE(app, argc, argv);

        return 0;
    } catch (const exception& e) {
        log.error("<88489c35> Error: {}", e.what());
        return 1;
    }
}
