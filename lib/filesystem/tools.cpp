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
    if (!filesystem::exists(filePath))
        throw runtime_error(format("<4966367b> File not found: {}", filePath));
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
    switch (status.type()) {
        case std::filesystem::file_type::directory:
            return std::make_shared<DirFileReaderBackend>(path);
        case std::filesystem::file_type::regular:
        case std::filesystem::file_type::symlink:
            return std::make_shared<ZipFileReaderBackend>(path);
        case std::filesystem::file_type::not_found:
            throw std::runtime_error(std::format("<f4062139> File not found: {}", path));
        default:
            throw std::runtime_error(std::format("<f2cf4c29> Unsupported file type: {}", path));
    }
}

}
