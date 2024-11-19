#pragma once

#include <string>

#include "../common/function.h"
#include "../common/node.h"

namespace Templates {

class TemplateRenderer {
public:
    virtual ~TemplateRenderer();
    virtual std::string render(const std::string& filePath, const Node& data, const Functions& funcs = Functions()) = 0;
};

}
