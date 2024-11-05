#include "inja_template_renderer.h"

#include <inja/inja.hpp>

#include "../filesystem/file_reader.h"
#include "../logger/logger.h"

using namespace std;

inline inja::json valueToJson(const Node::Null& v) { return nullptr; }
inline inja::json valueToJson(int v) { return v; }
inline inja::json valueToJson(const std::string& v) { return v; }
template <typename T>
inja::json valueToJson(const std::vector<T>& v)
{
    std::vector<inja::json> res;
    for (const auto& it : v) {
        res.push_back(valueToJson(it));
    }
    return res;
}
template <typename T>
inja::json valueToJson(const std::map<std::string, T>& v)
{
    std::map<std::string, inja::json> res;
    for (const auto& it : v) {
        res[it.first] = valueToJson(it.second);
    }
    return res;
}

inja::json valueToJson(const Node& data)
{
    return visit([](auto&& v) { return valueToJson(v); }, data.value);
}

namespace Templates {

namespace {
LogFacade::Logger logger("Templates::InjaTemplateRenderer");
}

InjaTemplateRenderer::InjaTemplateRenderer(Opts&& opts)
    : opts(std::move(opts))
{
}

std::string InjaTemplateRenderer::render(const std::string& filePath, const Node& data)
{
    logger.debug("<53daa47f> Render template: {}", filePath);
    auto tmpl = opts.fileReader->read(filePath);
    inja::Environment env;
    env.set_include_callback([&](const std::string& path, const std::string& templateName) {
        return env.parse(opts.fileReader->read(templateName));
    });
    env.set_search_included_templates_in_files(false);
    auto jsonData = valueToJson(data);
    return env.render(tmpl, jsonData);
}

}
