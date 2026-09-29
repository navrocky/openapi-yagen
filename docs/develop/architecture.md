# Architecture

Entry point: `cli/main.cpp` → `cli/commands/generate_command.cpp` (`GenerateCommand`) wires
together the pieces below and calls `Generator::OpenApiGenerator::generate()`.

`lib/` is split into independent static libraries (see each `lib/*/CMakeLists.txt` for exact
sources/deps):

- `lib/common` — shared value type `Node` (a `std::variant`-based JSON/YAML-like tree, see
  `node.h`), `NodeWalker` for structured parsing, string helpers, functional-style STL pipe
  helpers (`std_tools.h`, e.g. `x | mapToVector(...)`, `x | firstOrThrow()`), YAML/JSON parsing.
- `lib/logger` — small logging facade (`Logger`, `LogLevel`) + console backend.
- `lib/filesystem` — abstracts reading generator files from a directory, a zip archive, or a
  remote HTTP(S) URL (`FileReaderBackend` implementations), plus output file writing and
  post-processing via external tools.
- `lib/js` — wraps QuickJS (`JS::Executor`) to run a generator's `main.js` as an ES module, with
  `tools.h/.cpp` converting between `Node` and `JSValue`.
- `lib/templates` — wraps Inja (`Templates::InjaTemplateRenderer`) for `.j2` template rendering.
- `lib/openapi` — a version-agnostic canonical OpenAPI object model (`Document`/`Schema`/... in
  `document.h`/`schema.h`/`info.h`/`security.h`) plus per-version-family readers/writers that
  convert a raw `Node` to/from it: `v3/` (`reader.cpp`/`writer.cpp`) covers OpenAPI 3.0/3.1/3.2 as
  one family (they differ only in a handful of fields/dialect quirks), `v2/` covers Swagger/OpenAPI
  2.0 separately since it's a structurally different format (`host`/`basePath`/`schemes` instead of
  `servers`, `definitions` instead of `components.schemas`, body/formData parameters instead of
  `requestBody`, ...). `version_convert.cpp`'s `convertVersion(node, from, to)` composes
  `<family>::Read` + `<family>::Write` to convert between any two supported versions via this IR as
  the hub; `resolve.cpp` resolves every `$ref` in the raw `Node` tree (not the typed model) before
  it reaches `lib/generator`. Reading a spec into the typed model also doubles as its structural
  validation - a malformed spec fails fast with a clear error naming the missing/misshapen field,
  no separate JSON-schema validation step. See
  [`docs/generator-format.md`](../generator-format.md#spec-versions-and-conversion) for the
  user-facing version/conversion behavior this enables.
- `lib/generator` — ties it together: `OpenApiGenerator::generate()` reads `generator.yml`
  (`generator_metadata.{h,cpp}`), reads the OpenAPI spec into a `Node`, converts it to the
  generator's declared `openApiVersion` if needed (`convertToGeneratorVersion`, via `lib/openapi`),
  resolves all `$ref`s, and executes `main.js` with globals injected (`schema`, `vars`,
  `renderTemplate`, `renderTemplateToString`, and the common functions in `functions.cpp`: `dump`,
  `toCamelCase`, `toPascalCase`, `toSnakeCase`, `toScreamingSnakeCase`) via
  `openapi_js_bridge.{h,cpp}`'s raw-passthrough + typed-field overlay pattern. A generator's
  `openApiVersion` may only resolve to 3.0/3.1/3.2 - 2.0 works only as a spec *input* (converted up
  before reaching the JS bridge), never as a generation target, since the bridge's raw shape
  assumes OAS 3.x (content maps, nested `schema` keys, ...).

`cli/` — CLI11-based argument parsing and command dispatch: `generate`/`g`
(`commands/generate_command.cpp`) runs the pipeline above, `convert` (`commands/convert_command.cpp`)
exposes `OpenApi::convertVersion` standalone, independent of any generator. `cli/config.h.in` is
configured by
CMake with `APP_VERSION`, taken from `git describe --tags --always` (root `CMakeLists.txt`): an
exact tag reads as `1.2.3`, N commits past a tag as `1.2.3-N-gHASH`. Requires the build's checkout
to have full history and tags fetched (see `.github/workflows/build.yml`'s `fetch-depth: 0`) and
`git` available on `PATH` at configure time (the musl/uClibc Docker build images install it) -
falls back to `"unknown"` if git or a repo isn't found. Exposed on the CLI via `-v/--version`
(`CLI::App::set_version_flag` in `cli/main.cpp`) and in the `--help` banner.

`generators/sample_cpp_models_generator/` is a working example generator (used by
`generators/run.sh` and referenced by `test/`) — a good template to look at when writing or
debugging a generator, or when changing what globals/functions are exposed to `main.js`.
