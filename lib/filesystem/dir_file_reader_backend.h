#pragma once

#include "file_reader.h"

namespace FS {

class DirFileReaderBackendFactory : public FileReaderBackendFactory {
public:
    FileReaderBackendPtr createBackend(const std::string& uri) override;
    bool isUriSupported(const std::string& uri) override;
};

class DirFileReaderBackend : public FileReaderBackend {
public:
    DirFileReaderBackend(const std::string_view& rootDir);
    std::optional<std::string> read(const std::string& filePath) override;
    bool isUriSupported();

private:
    std::string rootDir;
};

}
