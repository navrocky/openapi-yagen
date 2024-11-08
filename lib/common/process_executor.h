#pragma once

#include <string>

namespace ProcessExecutor {

struct ExecutionResult {
    int result;
    std::string stdOut;
    std::string stdErr;
};

ExecutionResult execute(const std::string& cmd);
void checkExecutionResult(const ExecutionResult& result, const std::string& cmd);
ExecutionResult executeAndCheckResult(const std::string& cmd);

}
