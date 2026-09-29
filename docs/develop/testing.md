# Testing

Tests use Catch2 3, in `test/` (`common_test.cpp`, `generator_test.cpp`, `openapi_js_test.cpp`,
`openapi_test.cpp`, `parser_test.cpp`, `v2_reader_writer_test.cpp`, `v3_reader_writer_test.cpp`,
`vfs_test.cpp`), built as the `openapi-yagen-test` target alongside the main build. Test
resources (sample specs, generator files, a test zip) live in `test/resources/`.

```bash
cmake --build build -j --target openapi-yagen-test
./build/test/openapi-yagen-test
```

`generator_test.cpp` uses mocked `TemplateRenderer`/`FileReaderBackend` — follow that pattern
(don't hit the real filesystem or network) when adding tests for generator/JS-executor behavior.

Each generator is also a self-contained subproject under `generators/<name>/` — `README.md` +
`src/` (everything `-g` points at) + `test/` (that generator's own runtime test suite, if it has
one). `kotlin_ktor_client_generator/test/` and `kotlin_ktor_server_generator/test/` are
independent Kotlin Multiplatform (`jvm()` + `linuxX64()`) Gradle projects that regenerate from
their own kitchen-sink spec: `./gradlew test` (aliased to `jvmTest`) actually runs the generated
code under JUnit5 (`testApplication`/`MockEngine`), not just compiles it, while
`./gradlew compileKotlinLinuxX64` proves the same generated code also compiles under Kotlin/Native
(checked in CI - `.github/workflows/build.yml`'s `kotlin-native-compile-check` job) - see
`generators/README.md` for the convention and how to run one in isolation.

## Trying it end-to-end

`generators/run.sh` runs the CLI against the example generator and the test petstore spec:
```bash
./build/cli/openapi-yagen g -o generators/out -g generators/sample_cpp_models_generator/src \
    -c test/resources/petstore.yaml -v "namespace=OpenAPI"
```
(the checked-in `generators/run.sh` assumes an installed `openapi-yagen` on PATH and
`clang-format` post-processing; adjust the path to your local build binary.)
