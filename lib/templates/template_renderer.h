#pragma once

#include <string>

#include "../common/node.h"

namespace Templates {

class TemplateRenderer {
public:
    virtual ~TemplateRenderer();
    virtual std::string render(const std::string& filePath, const Node& data) = 0;
};

}
