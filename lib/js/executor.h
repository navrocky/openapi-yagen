#pragma once

#include <functional>

#include "../filesystem/definitions.h"

typedef struct JSContext JSContext;
typedef struct JSRuntime JSRuntime;

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
