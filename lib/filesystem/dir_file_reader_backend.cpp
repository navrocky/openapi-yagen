#include "dir_file_reader_backend.h"

#include <filesystem>
#include <format>

#include "../logger/logger.h"
#include "tools.h"

using namespace std;
using namespace std::filesystem;

namespace FS {

namespace {
LogFacade::Logger logger("FS::DirFileReaderBackend");
}

DirFileReaderBackend::DirFileReaderBackend(const std::string_view& rootDir)
    : rootDir(rootDir)
{
    if (status(rootDir).type() != file_type::directory)
        throw runtime_error(format("<e6b710fc> Root dir path is not directory: {}", rootDir));
}

std::optional<string> DirFileReaderBackend::read(const std::string_view& filePath)
{
    auto fullPath = path(rootDir) / filePath;
    logger.debug("<06a09bf9> Read file: {}", fullPath.string());
    if (!exists(fullPath))
        return nullopt;
    return FS::readFile(fullPath);
}

}
