#include "file_reader.h"

#include <format>

using namespace std;

namespace FS {

FileReaderBackend::~FileReaderBackend() { }

FileReader::FileReader(Opts&& opts)
    : opts(std::move(opts))
{
}

std::string FileReader::read(const std::string_view& filePath)
{
    for (const auto& b : opts.backends) {
        auto content = b->read(filePath);
        if (content)
            return *content;
    }
    throw runtime_error(format("<790cf901> File not found \"{}\"", filePath));
}
}
