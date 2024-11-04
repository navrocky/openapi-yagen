#include "openapi_generator.h"

#include <cstring>
#include <stdexcept>

#include <quickjs/quickjs-libc.h>
#include <quickjs/quickjs.h>

#include "common/finalize.h"
#include "common/node_walker.h"
#include "common/yaml_or_json_parser.h"
#include "filesystem/file_reader.h"
#include "filesystem/file_writer.h"
#include "filesystem/tools.h"
#include "js/executor.h"
#include "js/tools.h"
#include "logger/logger.h"
#include "model/generator_metadata.h"
#include "templates/template_renderer.h"

using namespace std;
using namespace JS;

namespace {

namespace {
LogFacade::Logger logger("OpenApiGenerator");
}

Node readSpecFile(const string& filePath)
{
    try {
        auto specFile = FS::readFile(filePath);
        return parseYamlOrJsonToNode(specFile);
    } catch (const exception& e) {
        throw runtime_error(format("<2b4ec139> Cannot read spec file \"{}\". Error: {}", filePath, e.what()));
    }
}

GeneratorMetadata readMetadata(const FS::FileReaderPtr& fileReader, const string& metadataPath)
{
    try {
        auto metadataNode = parseYamlOrJsonToNode(fileReader->read(metadataPath));
        return parseGeneratorMetadata(NodeWalker(metadataNode));
    } catch (const exception& e) {
        throw runtime_error(
            format("<c20e9799> Cannot read generator metadata from file \"{}\". Error: {}", metadataPath, e.what()));
    }
}

JSValue renderTemplate(JSContext* ctx, JSValueConst thisVal, int argc, JSValueConst* argv, int magic, JSValue* data)
{
    return runAndCatchExceptions(ctx, [&] {
        const auto& opts = *jsValueToPtr<const OpenApiGenerator::Opts>(*data);
        if (argc != 3)
            throw runtime_error("<ff372a54> renderTemplate requires two arguments (templateFileName: string, data: "
                                "object, outFileName: string)");
        auto templateFileName = jsValueToString(ctx, argv[0]);
        Node data = jsValueToNode(ctx, argv[1]);
        auto outFileName = jsValueToString(ctx, argv[2]);
        auto content = opts.templateRenderer->render(templateFileName, data);
        opts.fileWriter->write(outFileName, content);
        return JS_NewBool(ctx, 1);
    });
}

}

OpenApiGenerator::OpenApiGenerator(Opts&& opts)
    : opts(std::move(opts))
{
}

void OpenApiGenerator::generate(const string& specPath)
{
    if (opts.clearOutDir)
        opts.fileWriter->clear();
    auto metadata = readMetadata(opts.fileReader, opts.metadataPath);
    auto mainScriptPath = metadata.mainScriptPath.value_or(opts.defaultMainSciptPath);
    auto schemaNode = readSpecFile(specPath);
    auto optsPtr = &opts;
    opts.jsExecutor->execute(mainScriptPath, [&schemaNode, optsPtr](JSContext* ctx) {
        auto globalObj = JS_GetGlobalObject(ctx);
        finalize { JS_FreeValue(ctx, globalObj); };

        setObjFunction(ctx, globalObj, "renderTemplate", renderTemplate, optsPtr);
        auto v = nodeToJSValue(ctx, schemaNode);
        setObjProperty(ctx, globalObj, "schema", v);
    });
}
