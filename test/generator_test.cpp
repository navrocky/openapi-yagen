// clazy:excludeall=non-pod-global-static

#include <catch2/catch_all.hpp>

#include <sstream>

#include <lib/filesystem/file_reader.h>
#include <lib/filesystem/file_writer.h>
#include <lib/js/executor.h>
#include <lib/generator/openapi_generator.h>
#include <lib/templates/template_renderer.h>

#include "common/tools.h"

using namespace std;
using namespace FS;

class MockedTemplateRenderer : public Templates::TemplateRenderer {
public:
    string render(const string& filePath, const Node& data) override
    {
        stringstream ss;
        ss << "template=" << filePath << ", data=" << data;
        return ss.str();
    }
};

class MockedFileReaderBackend : public FileReaderBackend {
public:
    using Files = std::map<std::string, std::string>;
    MockedFileReaderBackend(const Files& files = {})
        : files(files)
    {
    }
    std::optional<string> read(const string_view& filePath) override
    {
        auto it = files.find(string(filePath));
        if (it != files.end())
            return it->second;
        else
            return nullopt;
    }

private:
    Files files;
};

class MockedFileWriter : public FileWriter {
public:
    void write(const string& fileName, const string& content) override { files[fileName] = content; }
    void clear() override { files.clear(); }

    std::map<std::string, std::string> files;
};

TEST_CASE("Generate", "[generator]")
{
    auto fileWriter = make_shared<MockedFileWriter>();
    auto templateRenderer = make_shared<MockedTemplateRenderer>();

    MockedFileReaderBackend::Files files = {
        { "main.js", "renderTemplate(\"test_template\", schema, \"outfile\")" },
        { "generator.yml", readResource("generator.yml") },
    };

    auto fileReader = make_shared<FileReader>(FileReader::Opts {
        .backends = { make_shared<MockedFileReaderBackend>(files) },
    });

    auto jsExecutor = make_shared<JS::Executor>(JS::Executor::Opts {
        .fileReader = fileReader,
    });

    OpenApiGenerator gen(OpenApiGenerator::Opts {
        .fileReader = fileReader,
        .fileWriter = fileWriter,
        .jsExecutor = jsExecutor,
        .templateRenderer = templateRenderer,
        .defaultMainSciptPath = "main.js",
        .metadataPath = "generator.yml",
    });

    gen.generate(getResourcePath("petstore.yaml"));

    REQUIRE_THAT(fileWriter->files["outfile"], Catch::Matchers::ContainsSubstring("#/components/schemas/Pet"));
}
