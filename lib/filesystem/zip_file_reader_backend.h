#pragma once

#include "file_reader.h"

struct zip_t;

namespace FS {

class ZipFileReaderBackend : public FileReaderBackend {
public:
    ZipFileReaderBackend(const std::string& zipPath);
    std::optional<std::string> read(const std::string_view& filePath) override;

private:
    std::string zipPath;
    std::shared_ptr<zip_t> zip;
};

}
