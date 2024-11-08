#pragma once

#include "file_reader.h"

namespace FS {

class RemoteFileReaderBackendFactory : public FileReaderBackendFactory {
public:
    FileReaderBackendPtr createBackend(const std::string& uri) override;
    bool isUriSupported(const std::string& uri) override;
};

class RemoteFileReaderBackend : public FileReaderBackend {
public:
    RemoteFileReaderBackend(const std::string& baseUrl);
    std::optional<std::string> read(const std::string& filePath) override;

private:
    std::string getFileUrl(const std::string& filePath);

    std::string baseUrl;
};

}
