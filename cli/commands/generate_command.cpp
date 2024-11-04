#include "generate_command.h"

#include <lib/filesystem/dir_file_reader_backend.h>
#include <lib/filesystem/dir_file_writer.h>
#include <lib/filesystem/tools.h>
#include <lib/js/executor.h>
#include <lib/logger/logger.h>
#include <lib/openapi_generator.h>
#include <lib/templates/inja_template_renderer.h>

using namespace std;

namespace {
LogFacade::Logger logger("GenerateCommand");
}

GenerateCommand::GenerateCommand() { }

void GenerateCommand::reg(CLI::App& app)
{
    auto cmd = app.add_subcommand("generate", "Generate sources from openapi specification")
                   ->alias("g")
                   ->callback(std::bind(&GenerateCommand::process, this));
    cmd->add_option("spec-file", specPath, "Specification file")->default_str("openapi.yaml");
    cmd->add_option("--override-dir", overrideDir, "Directory with overridden generator files");
    cmd->add_option("-o, --out-dir", outDir, "Output directory for generated code")->default_str(".");
    cmd->add_option("-g, --generator", generatorPath, "Path to generator. It can be directory or zip archive")
        ->required();
    cmd->add_flag("-c, --clear", clearOutDir, "Clear output directory before generating");
}

void GenerateCommand::process()
{
    vector<FS::FileReaderBackendPtr> fsBackends;
    if (!overrideDir.empty()) {
        fsBackends.push_back(make_shared<FS::DirFileReaderBackend>(overrideDir));
    }
    fsBackends.push_back(FS::openAutodetectedFileSystem(generatorPath));
    auto fileReader = make_shared<FS::FileReader>(FS::FileReader::Opts { fsBackends });

    auto templateRenderer = make_shared<Templates::InjaTemplateRenderer>(Templates::InjaTemplateRenderer::Opts {
        .fileReader = fileReader,
    });

    auto fileWriter = make_shared<FS::DirFileWriter>(outDir);

    auto jsExecutor = make_shared<JS::Executor>(JS::Executor::Opts {
        .fileReader = fileReader,
    });

    OpenApiGenerator g(OpenApiGenerator::Opts {
        .fileReader = fileReader,
        .fileWriter = fileWriter,
        .jsExecutor = jsExecutor,
        .templateRenderer = templateRenderer,
        .defaultMainSciptPath = "main.js",
        .metadataPath = "generator.yml",
        .clearOutDir = clearOutDir,
    });

    logger.debug(
        "<b1f2a941> Generate output: schema={}, generator={}, clearOutputDir={}", specPath, generatorPath, clearOutDir);
    g.generate(specPath);
}
