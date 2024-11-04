#pragma once

#include <functional>

#include <quickjs/quickjs.h>

#include "../filesystem/definitions.h"

namespace JS {

class Executor {
public:
    using ContextConfigurator = std::function<void(JSContext* ctx)>;

    struct Opts {
        FS::FileReaderPtr fileReader;
    };

    Executor(Opts&& opts);
    ~Executor();

    void execute(const std::string& file, const ContextConfigurator& contextConfigurator = ContextConfigurator());

private:
    Opts opts;
    JSRuntime* runtime;
};

}
