# Yet another OpenAPI generator (openapi-yagen)

Main features:

- small core written in C++
- extensible with generators written in JavaScript ES2023 ([QuickJS](https://bellard.org/quickjs/)) and 
  [Inja](https://pantor.github.io/inja/) templates (like Jinja)
- extending templates with additional functions defined in JS
- possibility to extend existing generators by overriding some files from a specified directory

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
                              Path to generator. It can be directory or zip archive
  -p,--post-process TEXT ...  Post process file with specified tool for extension
  -c,--clear                  Clear output directory before generating
```

## Generator reference

The generator is a folder with files or a zip archive. The structure of the generator files is as follows:

- `generator.yml` - generator metadata
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

## JavaScript reference

```typescript
renderTemplate(
    templateFilePath: string, 
    data: { [key: string]: any }, 
    outFilePath: string,
    functions?: { [name: string]: function }
): void
```

Renders specified template (`templateFilePath`) in generator folder into `outFilePath` with provided `data` object. 
Additionally, you can pass a set of JS defined functions (`functions`) that will be available for use in the templates.

```typescript
renderTemplateToString(
    templateFilePath: string, 
    data: { [key: string]: any }, 
    functions?: { [name: string]: function }
): string
```

Renders specified template (`templateFilePath`) in generator folder into string with provided `data` object. 
Additionally, you can pass a set of JS defined functions (`functions`) that will be available for use in the templates.

