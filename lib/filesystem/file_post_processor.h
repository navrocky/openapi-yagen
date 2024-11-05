#pragma once

#include <string>
#include <vector>

namespace FS {

class FilePostProcessor {
public:
    virtual ~FilePostProcessor();
    virtual void postProcess(const std::string& filePath) = 0;
};

class SystemToolsFilePostProcessor : public FilePostProcessor {
public:
    using Tools = std::vector<std::string>;

    SystemToolsFilePostProcessor(const Tools& tools);
    void postProcess(const std::string& filePath) override;

private:
    Tools tools;
};

}
