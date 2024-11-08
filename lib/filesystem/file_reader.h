#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "definitions.h"

namespace FS {

class FileReaderBackend {
public:
    virtual ~FileReaderBackend();

    virtual std::optional<std::string> read(const std::string_view& filePath) = 0;
};

class FileReader {
public:
    struct Opts {
        std::vector<FileReaderBackendPtr> backends;
    };
    FileReader(Opts&& opts);

    std::string read(const std::string_view& filePath);

private:
    Opts opts;
    std::map<std::string, std::string> fileCache;
};

}
