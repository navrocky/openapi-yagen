#include "schema_validator.h"

#include "../common/std_tools.h"
#include "../common/yaml_or_json_parser.h"
#include <nlohmann/json-schema.hpp>
#include <yaml-cpp/yaml.h>

#include "../filesystem/file_reader.h"

using namespace std;
using nlohmann::json;
using nlohmann::json_schema::json_validator;

inline json nodeToJson(const Node::Null& v) { return nullptr; }
inline json nodeToJson(Node::Bool v) { return v; }
inline json nodeToJson(Node::Int v) { return v; }
inline json nodeToJson(const Node::String& v) { return v; }

template <typename T>
json nodeToJson(const std::vector<T>& v)
{
    return v | mapToVector([](const T& v) { return nodeToJson(v); });
}

template <typename T>
json nodeToJson(const std::map<std::string, T>& v)
{
    std::map<std::string, json> res;
    for (const auto& it : v) {
        res[it.first] = nodeToJson(it.second);
    }
    return res;
}

json nodeToJson(const Node& data)
{
    return visit([](auto&& v) { return nodeToJson(v); }, data.value);
}

JsonSchemaValidator::JsonSchemaValidator(Opts&& opts)
    : opts(std::move(opts))
{
}

void JsonSchemaValidator::validate(const Node& node)
{
    auto schemaContent = opts.fileReader->read(opts.schemaFilePath);
    auto formatChecker = [](const std::string & /*format*/, const std::string & /*value*/) {};
    json_validator validator(nullptr, formatChecker);
    auto schemaJson = nodeToJson(parseYamlOrJsonToNode(schemaContent));
    validator.set_root_schema(schemaJson);
    validator.validate(nodeToJson(node));
}
