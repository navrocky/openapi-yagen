#pragma once

#include "../filesystem/definitions.h"
#include "template_renderer.h"

namespace Templates {

class InjaTemplateRenderer : public TemplateRenderer {
public:
    struct Opts {
        FS::FileReaderPtr fileReader;
    };
    InjaTemplateRenderer(Opts&& opts);

    std::string render(const std::string& filePath, const Node& data, const Functions& funcs) override;

private:
    Opts opts;
};

}
