#include "file_reader.h"

#include <format>

using namespace std;

namespace FS {

FileReaderBackendFactory::~FileReaderBackendFactory() { }

FileReaderBackend::~FileReaderBackend() { }

FileReader::FileReader(Opts&& opts)
    : opts(std::move(opts))
{
}

std::string FileReader::read(const string& filePath)
{
    auto it = fileCache.find(filePath);
    if (it != fileCache.end())
        return it->second;

    for (const auto& b : opts.backends) {
        auto content = b->read(filePath);
        if (content) {
            fileCache[filePath] = *content;
            return *content;
        }
    }
    throw runtime_error(format("<790cf901> File not found \"{}\"", filePath));
}

FileReaderBackendPtr createBackend(const std::string& uri, const std::vector<FileReaderBackendFactoryPtr> factories)
{
    for (const auto& f : factories) {
        if (f->isUriSupported(uri))
            return f->createBackend(uri);
    }
    throw runtime_error(format("<08ff888f> Unable to determine supported file reading backend for uri: {}", uri));
}

}
