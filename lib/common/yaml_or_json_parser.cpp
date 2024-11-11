#include "yaml_or_json_parser.h"

#include <format>

#include <yaml-cpp/yaml.h>

using namespace std;

Node convertNode(const YAML::Node& n);

Node convertScalar(const YAML::Node& n)
{
    bool b;
    int64_t i;
    string s;
    if (YAML::convert<bool>::decode(n, b)) {
        return { b };
    } else if (YAML::convert<int64_t>::decode(n, i)) {
        return { i };
    } else if (YAML::convert<string>::decode(n, s)) {
        return { s };
    } else {
        throw runtime_error("<bd6fb38c> Unknown scalar node type");
    }
}

Node convertMap(const YAML::Node& n)
{
    Node::Map res;
    for (auto it = n.begin(); it != n.end(); ++it) {
        auto key = it->first.as<string>();
        res[key] = convertNode(it->second);
    }
    return { res };
}

Node convertSequence(const YAML::Node& node)
{
    Node::Vec res;
    for (auto i = 0u; i < node.size(); i++) {
        auto n = node[i];
        res.push_back(convertNode(n));
    }
    return { res };
}

Node convertNode(const YAML::Node& n)
{
    if (n.IsNull()) {
        return { Node::NullValue };
    } else if (n.IsScalar()) {
        return convertScalar(n);
    } else if (n.IsMap()) {
        return convertMap(n);
    } else if (n.IsSequence()) {
        return convertSequence(n);
    } else {
        throw runtime_error(format("<b4cada15> Unsupported YAML node type: {}", static_cast<int>(n.Type())));
    }
}

Node parseYamlOrJsonToNode(const std::string& yaml)
{
    auto node = YAML::Load(yaml);
    return convertNode(node);
}
