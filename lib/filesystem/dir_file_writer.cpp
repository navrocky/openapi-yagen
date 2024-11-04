#include "dir_file_writer.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>

#include "../logger/logger.h"

using namespace std;

namespace FS {
namespace {
LogFacade::Logger logger("FS::DirFileWriter");
}

DirFileWriter::DirFileWriter(const std::string& outDir)
    : outDir(outDir)
{
}

void DirFileWriter::write(const std::string& fileName, const std::string& content)
{
    filesystem::path fullPath(outDir);
    fullPath /= fileName;
    logger.debug("<64aeffb8> Write file: {}", fullPath.string());

    if (fullPath.has_parent_path()) {
        if (!filesystem::create_directories(fullPath.parent_path()))
            throw runtime_error(format("<f60b9569> Cannot create directory: {}", outDir));
    }

    ofstream fs(fullPath, ios_base::out | ios_base::trunc);
    fs << content;
    if (fs.fail() || fs.bad())
        throw runtime_error(format("<a08d4abb> Cannot write file: {}", fullPath.generic_string()));
}

void DirFileWriter::clear()
{
    for (const auto& entry : filesystem::directory_iterator(outDir)) {
        auto fn = entry.path().filename();
        if (fn != "." && fn != "..")
            filesystem::remove_all(entry.path());
    }
}

}
