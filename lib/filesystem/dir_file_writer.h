#pragma once

#include <string>

#include "file_writer.h"

namespace FS {

class DirFileWriter : public FileWriter {
public:
    DirFileWriter(const std::string& outDir);

    void write(const std::string& fileName, const std::string& content) override;
    void clear() override;

private:
    std::string outDir;
};
}
