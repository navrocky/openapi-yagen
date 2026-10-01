#include "dir_file_writer.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>

#include "../logger/logger.h"
#include "file_post_processor.h"
#include "tools.h"

using namespace std;

namespace FS {
namespace {
LogFacade::Logger logger("FS::DirFileWriter");
}

DirFileWriter::DirFileWriter(Opts&& opts)
    : opts(std::move(opts))
{
}

void DirFileWriter::write(const std::string& fileName, const std::string& content)
{
    if (!isPathTraversalSafe(fileName))
        throw runtime_error(format("<c92e0a71> Refusing to write outside output directory: \"{}\"", fileName));

    auto confined = confineToRoot(opts.outDir, fileName, /*mustExist=*/false);
    if (!confined)
        throw runtime_error(format("<d4b6f813> Refusing to write outside output directory: \"{}\"", fileName));
    auto fullPath = *confined;

    logger.debug("<64aeffb8> Write file: {}", fullPath.string());

    if (fullPath.has_parent_path()) {
        auto outDirPath = fullPath.parent_path();
        if (!filesystem::exists(outDirPath) && !filesystem::create_directories(outDirPath))
            throw runtime_error(format("<f60b9569> Cannot create directory: {}", outDirPath.string()));
    }

    // write file
    {
        ofstream fs(fullPath, ios_base::out | ios_base::trunc);
        fs << content;
        if (fs.fail() || fs.bad())
            throw runtime_error(format("<a08d4abb> Cannot write file: {}", fullPath.generic_string()));
    }

    // post process file
    if (opts.filePostProcessor)
        opts.filePostProcessor->postProcess(fullPath);
}

void DirFileWriter::clear()
{
    if (!filesystem::exists(opts.outDir))
        return;
    // Clearing the working directory (the default "-o .") or one of its parents would wipe the
    // user's own sources, so refuse instead of guessing what was meant.
    auto outDir = filesystem::weakly_canonical(opts.outDir);
    auto rel = filesystem::weakly_canonical(filesystem::current_path()).lexically_relative(outDir);
    if (!rel.empty() && *rel.begin() != "..")
        throw runtime_error(format("<7d2fe6a4> Refusing to clear output directory \"{}\": it is the current "
                                   "directory or one of its parents, specify a dedicated --out-dir",
            opts.outDir));
    for (const auto& entry : filesystem::directory_iterator(opts.outDir)) {
        auto fn = entry.path().filename();
        if (fn != "." && fn != "..")
            filesystem::remove_all(entry.path());
    }
}

}
