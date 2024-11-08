#include "process_executor.h"

#include <array>
#include <cstdio>
#include <format>
#include <stdexcept>
#include <thread>
#include <unistd.h>

#include "../logger/logger.h"
#include "finalize.h"

using namespace std;

namespace ProcessExecutor {
namespace {
LogFacade::Logger logger("ProcessExecutor");
}

ExecutionResult execute(const std::string& cmd)
{
    logger.debug("<495e6bfa> Execute process: {}", cmd);
    int pfd[2];
    if (pipe(pfd) < 0)
        throw runtime_error("<baf687fe> Cannot create pipe");
    finalize
    {
        close(pfd[0]);
        close(pfd[1]);
    };
    auto perr = fdopen(pfd[0], "r");
    finalize { fclose(perr); };

    array<char, 128> buffer;
    ExecutionResult res;
    auto finalCmd = format("{} 2>&{}", cmd, pfd[1]);
    auto pout = popen(finalCmd.c_str(), "r");
    if (!pout)
        throw runtime_error(format("<2f3ef842> Execution of process failed: {}", cmd));
    try {
        close(pfd[1]);

        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pout) != nullptr) {
            res.stdOut += buffer.data();
        }

        // read std err in separated thread
        thread th([perr, &res]() {
            array<char, 128> buffer;
            while (fgets(buffer.data(), static_cast<int>(buffer.size()), perr) != nullptr) {
                res.stdErr += buffer.data();
            }
        });
        finalize { th.join(); };

    } catch (...) {
        pclose(pout);
        throw;
    }
    res.result = pclose(pout);
    return res;
}

ExecutionResult executeAndCheckResult(const std::string& cmd)
{
    auto result = execute(cmd);
    checkExecutionResult(result, cmd);
    return result;
}

void checkExecutionResult(const ExecutionResult& result, const std::string& cmd)
{
    if (result.result != 0)
        throw runtime_error(
            format("<7cc999ce> Execution process failed. command=\"{}\", result={}, stdOut=\"{}\", stdErr=\"{}\"", cmd,
                result.result, result.stdOut, result.stdErr));
}
}
