#pragma once

#include "file_reader.h"

struct zip_t;

namespace FS {

class ZipFileReaderBackendFactory : public FileReaderBackendFactory {
public:
    FileReaderBackendPtr createBackend(const std::string& uri) override;
    bool isUriSupported(const std::string& uri) override;
};

class ZipFileReaderBackend : public FileReaderBackend {
public:
    ZipFileReaderBackend(const std::string& zipPath);
    std::optional<std::string> read(const std::string& filePath) override;

private:
    std::string zipPath;
    std::shared_ptr<zip_t> zip;
};

}
