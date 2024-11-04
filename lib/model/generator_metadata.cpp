#include "generator_metadata.h"

#include "../common/node_walker.h"

VariableDescriptor parseVariableDescriptor(const NodeWalker& w)
{
    return {
        .name = w["name"].required<Str>(),
        .description = w["description"].optional<Str>(),
        .defaultValue = w["defaultValue"].optional<Node>().value_or(Node()),
    };
}

GeneratorMetadata parseGeneratorMetadata(const NodeWalker& w)
{
    return {
        .name = w["name"].required<Str>(),
        .description = w["description"].optional<Str>(),
        .mainScriptPath = w["mainScriptPath"].optional<Str>(),
        .jsonSchemaPath = w["jsonSchemaPath"].optional<Str>(),
        .variables = w["variables"].optionalList(parseVariableDescriptor).value_or(std::vector<VariableDescriptor>()),
    };
}
