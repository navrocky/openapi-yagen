// clazy:excludeall=non-pod-global-static

#include <filesystem>
#include <memory>

#include <catch2/catch_all.hpp>

#include <lib/common/string_tools.h>
#include <lib/filesystem/dir_file_reader_backend.h>
#include <lib/filesystem/file_post_processor.h>
#include <lib/filesystem/tools.h>
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

TEST_CASE("Post process file", "[vfs]")
{
    string outFile = "/tmp/out";
    FS::SystemToolsFilePostProcessor pp({ format("h,cpp:echo \"Processed file is: %file%.\">{}", outFile) });
    pp.postProcess("test.h");
    auto outFileContent = FS::readFile(outFile);
    REQUIRE((outFileContent | trim()) == "Processed file is: test.h.");
    filesystem::remove(outFile);
}
