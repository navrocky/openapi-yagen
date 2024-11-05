#include "file_post_processor.h"

#include <filesystem>
#include <set>

#include "../common/std_tools.h"
#include "../common/string_tools.h"

using namespace std;

namespace FS {

struct Tool {
    set<string> fileExt;
    string toolTemplate;
};

Tool parseToolString(const std::string& s)
{
    auto pos = s.find(":");
    Tool res;
    if (pos != string::npos) {
        res.fileExt = s.substr(0, pos) | split(",") | mapToSet([](const auto& v) { return string(v); });
        res.toolTemplate = s.substr(pos + 1, s.size() - pos - 1);
    } else {
        res.toolTemplate = s;
    }
    return res;
}

FilePostProcessor::~FilePostProcessor() { }

SystemToolsFilePostProcessor::SystemToolsFilePostProcessor(const Tools& tools)
    : tools(tools)
{
}

void SystemToolsFilePostProcessor::postProcess(const std::string& filePath)
{
    for (const auto& s : tools) {
        auto tool = parseToolString(s);
        if (!tool.fileExt.empty()) {
            auto ext = filesystem::path(filePath).extension();
            if (tool.fileExt.find(ext) != tool.fileExt.end()) { }
            // if (tool.fileExt != filesystem::path(filePath).extension())
        }
    }
}
}
