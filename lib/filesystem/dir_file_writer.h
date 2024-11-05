#pragma once

#include <string>

#include "definitions.h"
#include "file_writer.h"

namespace FS {

class DirFileWriter : public FileWriter {
public:
    struct Opts {
        std::string outDir;
        FilePostProcessorPtr filePostProcessor;
    };

    DirFileWriter(Opts&& opts);

    void write(const std::string& fileName, const std::string& content) override;
    void clear() override;

private:
    Opts opts;
};
}
