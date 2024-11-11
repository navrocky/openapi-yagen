#pragma once

#include "../common/node.h"
#include "../common/yaml_or_json_parser.h"
#include "../filesystem/definitions.h"

class JsonSchemaValidator {
public:
    struct Opts {
        FS::FileReaderPtr fileReader;
        std::string schemaFilePath;
    };

    JsonSchemaValidator(Opts&& opts);

    void validate(const Node& node);

private:
    Opts opts;
};
