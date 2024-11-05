#include "executor.h"

#include <stdexcept>

#include <quickjs/quickjs-libc.h>
#include <quickjs/quickjs.h>

#include "../common/finalize.h"
#include "../filesystem/file_reader.h"

#include "tools.h"

using namespace std;

namespace JS {

Executor::Executor(Opts&& opts)
    : opts(std::move(opts))
{
    runtime = JS_NewRuntime();
    if (!runtime)
        throw runtime_error("<cdab9df4> Cannot create runtime");
}

Executor::~Executor() { JS_FreeRuntime(runtime); }

void Executor::execute(const std::string& file, const ContextConfigurator& contextConfigurator)
{
    auto ctx = JS_NewContext(runtime);
    finalize { JS_FreeContext(ctx); };

    js_std_add_helpers(ctx, 0, nullptr);

    if (contextConfigurator)
        contextConfigurator(ctx);

    auto script = opts.fileReader->read(file);
    auto val = JS_Eval(ctx, script.c_str(), script.size(), file.c_str(), JS_EVAL_TYPE_GLOBAL);
    finalize { JS_FreeValue(ctx, val); };
    checkForException(ctx, val, format("<1dc6d63b> Script execution error: {}", file));
}

}
