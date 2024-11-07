#include "executor.h"

#include <stdexcept>

#include <quickjs/quickjs-libc.h>
#include <quickjs/quickjs.h>

#include "../common/finalize.h"
#include "../filesystem/file_reader.h"
#include "../logger/logger.h"

#include "tools.h"

using namespace std;

namespace {
LogFacade::Logger logger("JS::Executor");
}

namespace JS {

namespace {
JSModuleDef* jsModuleLoader(JSContext* ctx, const char* moduleName, void* opaque)
{
    logger.debug("<5124df20> Load module: {}", moduleName);
    const Executor::Opts& opts = *(Executor::Opts*)opaque;
    auto moduleContent = opts.fileReader->read(moduleName);
    auto funcVal = JS_Eval(
        ctx, moduleContent.data(), moduleContent.size(), moduleName, JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
    checkForException(ctx, funcVal, format("<ceb13b02> Module evaluating error: {}", moduleName));
    JSModuleDef* m = (JSModuleDef*)JS_VALUE_GET_PTR(funcVal);
    JS_FreeValue(ctx, funcVal);
    return m;
}
}

Executor::Executor(Opts&& opts)
    : opts(std::move(opts))
{
    runtime = JS_NewRuntime();
    if (!runtime)
        throw runtime_error("<cdab9df4> Cannot create runtime");

    JS_SetModuleLoaderFunc(runtime, nullptr, jsModuleLoader, &this->opts);
}

Executor::~Executor() { JS_FreeRuntime(runtime); }

void Executor::execute(const std::string& file, const ContextConfigurator& contextConfigurator)
{
    logger.debug("<bfecc0f Executing: {}", file);
    auto ctx = JS_NewContext(runtime);
    finalize { JS_FreeContext(ctx); };

    js_std_add_helpers(ctx, 0, nullptr);

    if (contextConfigurator)
        contextConfigurator(ctx);

    auto script = opts.fileReader->read(file);
    auto val = JS_Eval(ctx, script.c_str(), script.size(), file.c_str(), JS_EVAL_TYPE_MODULE);
    finalize { JS_FreeValue(ctx, val); };
    checkForException(ctx, val, format("<1dc6d63b> Script execution error: {}", file));
}

}
