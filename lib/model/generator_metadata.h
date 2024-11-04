#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../common/node_walker.h"

using Str = std::string;
using OptStr = std::optional<std::string>;

struct VariableDescriptor {
    Str name;
    OptStr description;
    Node defaultValue;
};

struct GeneratorMetadata {
    Str name;
    OptStr description;
    OptStr mainScriptPath;
    OptStr jsonSchemaPath;
    std::vector<VariableDescriptor> variables;
};

GeneratorMetadata parseGeneratorMetadata(const NodeWalker& n);
