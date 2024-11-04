#include "zip_file_reader_backend.h"

#include <format>

#include <zip/zip.h>

#include "../common/finalize.h"

using namespace std;

namespace FS {

class ZipError : public std::runtime_error {
public:
    ZipError(const std::string& message, int errnum)
        : std::runtime_error(format("{}: {}", message, zip_strerror(errnum)))
    {
    }
};

ZipFileReaderBackend::ZipFileReaderBackend(const std::string& zipPath)
{
    int errnum;
    auto res = zip_openwitherror(zipPath.c_str(), 0, 'r', &errnum);
    if (!res)
        throw ZipError(format("<84e9b859> Cannot open zip=\"{}\"", zipPath), errnum);
    zip = shared_ptr<zip_t>(res, [](auto z) { zip_close(z); });
}

std::optional<string> ZipFileReaderBackend::read(const std::string_view& filePath)
{
    string fp(filePath);
    auto res = zip_entry_open(zip.get(), fp.c_str());
    if (res == ZIP_ENOENT)
        return nullopt;
    if (res != 0)
        throw ZipError(format("<84e9b859> Cannot open zip entry=\"{}\"", filePath), res);
    finalize { zip_entry_close(zip.get()); };
    void* buf;
    size_t size;
    res = zip_entry_read(zip.get(), &buf, &size);
    if (res < 0)
        throw ZipError(format("<bb2b6f5a> Cannot read zip entry=\"{}\"", filePath), res);
    return string((char*)buf, size);
}

}
