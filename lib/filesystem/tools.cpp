#include "tools.h"

#include <fstream>
#include <sstream>

#include <filesystem>
#include <format>

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

}
