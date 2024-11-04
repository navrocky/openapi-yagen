#include "tools.h"

#include <fstream>
#include <sstream>

#include <filesystem>
#include <format>

#include "dir_file_reader_backend.h"
#include "zip_file_reader_backend.h"

using namespace std;

namespace FS {

std::string readFile(const std::string& filePath)
{
    try {
        std::ifstream s(filePath);
        std::stringstream buffer;
        buffer << s.rdbuf();
        return buffer.str();
    } catch (const exception& e) {
        throw runtime_error(format("<d70e5c1f> Cannot read file \"{}\"", filePath));
    }
}

FileReaderBackendPtr openAutodetectedFileSystem(const std::string& path)
{
    auto status = std::filesystem::status(path);
    if (status.type() == std::filesystem::file_type::directory) {
        return std::make_shared<DirFileReaderBackend>(path);
    } else if (status.type() == std::filesystem::file_type::regular
        || status.type() == std::filesystem::file_type::symlink) {
        return std::make_shared<ZipFileReaderBackend>(path);
    } else {
        throw std::runtime_error(std::format("<f2cf4c29> Unsupported file type: {}", path));
    }
}

}
