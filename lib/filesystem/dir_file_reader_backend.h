#pragma once

#include "file_reader.h"

namespace FS {

class DirFileReaderBackend : public FileReaderBackend {
public:
    DirFileReaderBackend(const std::string_view& rootDir);
    std::optional<std::string> read(const std::string_view& filePath) override;

private:
    std::string rootDir;
};

}
