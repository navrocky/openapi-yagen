#pragma once

#include <functional>
#include <string>

#include "../common/node.h"

namespace Templates {

struct TemplateFunction {
    using Func = std::function<Node(const Node::Vec&)>;
    std::string name;
    Func func;
};

class TemplateRenderer {
public:
    using Functions = std::vector<TemplateFunction>;

    virtual ~TemplateRenderer();
    virtual std::string render(const std::string& filePath, const Node& data, const Functions& funcs = Functions()) = 0;
};

}
