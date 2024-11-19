#pragma once

#include <functional>

#include "node.h"

using FuncType = std::function<Node(const Node::Vec& args)>;

struct Function {
    std::string name;
    FuncType func;
};

using Functions = std::vector<Function>;
