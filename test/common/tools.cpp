#include "tools.h"

#include <filesystem>

#include <lib/filesystem/tools.h>

using namespace std;

#define xstr(s) str(s)
#define str(s) #s

string_view getResourcesDir() { return xstr(__RESOURCES_DIR__); }

std::string getResourcePath(const std::string_view& relativePath)
{
    return std::filesystem::path(getResourcesDir()) / relativePath;
}

string readResource(const std::string_view& relativePath) { return FS::readFile(getResourcePath(relativePath)); }
