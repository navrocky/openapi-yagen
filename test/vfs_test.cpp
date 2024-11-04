// clazy:excludeall=non-pod-global-static

#include <filesystem>
#include <memory>

#include <catch2/catch_all.hpp>

#include <lib/filesystem/dir_file_reader_backend.h>
#include <lib/filesystem/zip_file_reader_backend.h>

#include "common/tools.h"

using namespace std;
using namespace std::filesystem;
using namespace FS;

inline string rtrim(const string& s)
{
    auto res = s;
    res.erase(find_if(res.rbegin(), res.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), res.end());
    return res;
}

TEST_CASE("Directory file system backend", "[vfs]")
{
    DirFileReaderBackend fs(getResourcesDir());
    SECTION("Read existing file")
    {
        auto fileContent = fs.read("test_file").value();
        REQUIRE(rtrim(fileContent) == "Hello world!");
    }
    SECTION("Read non existing file") { REQUIRE(!fs.read("non_existent_file").has_value()); }
}

TEST_CASE("Zip file system backend", "[vfs]")
{
    ZipFileReaderBackend fs(path(getResourcesDir()) / "test.zip");
    SECTION("Read existing files")
    {
        auto fileContent = fs.read("test_file").value();
        REQUIRE(rtrim(fileContent) == "Test");
        fileContent = fs.read("another_file").value();
        REQUIRE(fileContent == "This is a another file");
    }
    SECTION("Read non existing file") { REQUIRE(!fs.read("non_existent_file").has_value()); }
}

TEST_CASE("Filesystem with backends", "[vfs]")
{
    FileReader fs(FileReader::Opts {
        .backends = {
            make_shared<DirFileReaderBackend>(getResourcesDir()),
            make_shared<ZipFileReaderBackend>(path(getResourcesDir()) / "test.zip"),
        },
    });
    SECTION("Read existing file from dir") { REQUIRE(rtrim(fs.read("test_file")) == "Hello world!"); }
    SECTION("Read existing file from zip") { REQUIRE(rtrim(fs.read("another_file")) == "This is a another file"); }
    SECTION("Read non existing file") { REQUIRE_THROWS(fs.read("non_existent_file")); }
}
