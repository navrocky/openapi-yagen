#pragma once

#include "../filesystem/definitions.h"
#include "../js/definitions.h"
#include "../templates/definitions.h"

class OpenApiGenerator {
public:
    struct Opts {
        FS::FileReaderPtr fileReader;
        FS::FileWriterPtr fileWriter;
        JS::ExecutorPtr jsExecutor;
        Templates::TemplateRendererPtr templateRenderer;
        std::string defaultMainSciptPath;
        std::string metadataPath;
        bool clearOutDir = false;
    };

    OpenApiGenerator(Opts&& opts);

    void generate(const std::string& specPath);

private:
    Opts opts;
};

using GeneratorPtr = std::shared_ptr<OpenApiGenerator>;
