// clazy:excludeall=non-pod-global-static

#include <catch2/catch_all.hpp>

#include <lib/common/yaml_or_json_parser.h>

#include "common/tools.h"

using namespace std;

TEST_CASE("Parse schema to Node", "[parser]")
{
    SECTION("Valid YAML")
    {
        auto content = readResource("petstore.yaml");
        auto n = parseYamlOrJsonToNode(content);
        // REQUIRE(n);
    }
    SECTION("Invalid YAML")
    {
        auto content = readResource("petstore.json");
        auto n = parseYamlOrJsonToNode(content);
        // auto n = get_if<Node>(&res);
        // REQUIRE(n);
    }
}
