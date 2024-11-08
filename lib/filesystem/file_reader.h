#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "definitions.h"

namespace FS {

class FileReaderBackendFactory {
public:
    virtual ~FileReaderBackendFactory();
    virtual FileReaderBackendPtr createBackend(const std::string& uri) = 0;
    virtual bool isUriSupported(const std::string& uri) = 0;
};

FileReaderBackendPtr createBackend(const std::string& uri, const std::vector<FileReaderBackendFactoryPtr> factories);

class FileReaderBackend {
public:
    virtual ~FileReaderBackend();

    virtual std::optional<std::string> read(const std::string& filePath) = 0;
};

class FileReader {
public:
    struct Opts {
        std::vector<FileReaderBackendPtr> backends;
    };
    FileReader(Opts&& opts);

    std::string read(const std::string& filePath);

private:
    Opts opts;
    std::map<std::string, std::string> fileCache;
};

}
