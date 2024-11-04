#pragma once

#include <string>

namespace FS {

class FileWriter {
public:
    virtual ~FileWriter();
    virtual void write(const std::string& fileName, const std::string& content) = 0;

    /// remove all files by configured path
    virtual void clear() = 0;
};

}
