# Yet another OpenAPI generator (openapi-yagen)

Main features:

- a small generator core written in C++
- specific generators are written in JavaScript with support of ES2023 features ([QuickJS](https://bellard.org/quickjs/)) 
  and [Inja](https://pantor.github.io/inja/) templates (like Jinja)
- extending templates with additional functions defined in JS
- possibility to extend existing generators by overriding some files from a specified directory
- postprocessing of output files using custom tools (code formatters, linters, checkers...).
- using generators available via HTTP/S (directly from Github, or other sources). The `curl` tool is required.

## CLI reference

Supported CLI root options and subcommands:
```
Usage: ./openapi-yagen [OPTIONS] [SUBCOMMAND]

Options:
  -h,--help                   Print this help message and exit

Subcommands:
  generate, g                 Generate sources from openapi specification
```

Generate subcommand:

```
Generate sources from openapi specification
Usage: ./openapi-yagen generate [OPTIONS] [spec-file]

Positionals:
  spec-file TEXT [openapi.yaml] 
                              Specification file

Options:
  -h,--help                   Print this help message and exit
  --override-dir TEXT         Directory with overridden generator files
  -o,--out-dir TEXT [.]       Output directory for generated code
  -g,--generator TEXT REQUIRED
                              Path to generator. It can be directory, zip archive or HTTP URL
  -p,--post-process TEXT ...  Post process file with specified tool for extension
  -c,--clear                  Clear output directory before generating
  -v,--var TEXT ...           Set variable. Syntax is: -v (var_name)=(var_value)
```

## Generator reference

The generator is a folder of files or a zip archive. The structure of the generator files is as follows:

- `generator.yml` - generator metadata, descriptor
- `main.js` - main JavaScript file
- other resoucres - Inja templates, other JS files imported into `main.js`

Example: 
```
├── generator.yml
├── head.h.j2
├── includes.h.j2
├── main.js
└── model.h.j2
```

The file `generator.yml` is a descriptor of the generator. This is a example file with comments:

```yaml
# Generator name
name: sample_cpp_models

# Generator description
description: Example of C++ model generator from OpenAPI v3 specification

# Main JavaScript file (entrypoint).
mainScriptPath: main.js

# Json schema for input data validation 
jsonSchemaPath: openapi_v3_schema.json

# Variables that can be used to customize script execution
variables:
  - name: namespace
    description: С++ namespace for model classes
```

### Common built-in functions

These functions are available in both JavaScript and template code.

#### dump

Dumps specified values to log output. It is a replacement for `console.log`.

```typescript
dump(...args: any)
```

#### toCamelCase, toPascalCase, toSnakeCase, toScreamingSnakeCase

Converts string identifier from any case convention to specified case convention.

```typescript
toCamelCase(s: string): string // -> camelCase
toPascalCase(s: string): string // -> PascalCase
toSnakeCase(s: string): string // -> snake_case
toScreamingSnakeCase(s: string): string // SCREAMING_SNAKE_CASE
```

## JavaScript reference

The generator core supports all modern JavaScript features from ES2023 (string interpolation, classes, let, const, 
modules ...) thanks to QuickJS. 

### Built-in functions

#### renderTemplate

Renders specified template (`templateFilePath`) in generator folder into `outFilePath` with provided `data` object. 
Additionally, you can pass a set of JS defined functions (`functions`) that will be available for use in the templates.

```typescript
renderTemplate(
    templateFilePath: string, 
    data: { [key: string]: any }, 
    outFilePath: string,
    functions?: { [name: string]: Function }
): void
```

#### renderTemplateToString

Renders specified template (`templateFilePath`) in generator folder into string with provided `data` object. 
Additionally, you can pass a set of JS defined functions (`functions`) that will be available for use in the templates.

```typescript
renderTemplateToString(
    templateFilePath: string, 
    data: { [key: string]: any }, 
    functions?: { [name: string]: Function }
): string
```

## Templating reference

`Inja` is used as the template rendering engine. The main documentation can be found here
https://pantor.github.io/inja/

You can call common built-in functions described above in this way:

```jinja
{% set value = toSnakeCase("FirstSecondThird") %}
{{ dump(value) }}
```

## Example generators 

Example generators are located in the `examples` folder.

## TODO

- [ ] Add schema validation with JSON schema (conan: json-schema-validator/2.3.0)
- [x] Add configuration variables
- [ ] Improve documentation and add more examples
- [ ] Use https://github.com/batterycenter/embed to embed some popular templates into binary
- [x] Add remote templates reading (from github for example)
- [ ] Command to create generator stub
- [ ] Command to show available variables for generator
- [ ] Restrict access to files outside working folder
