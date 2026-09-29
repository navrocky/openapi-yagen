# AGENTS.md

`openapi-yagen` is a C++20 CLI that generates code from an OpenAPI spec via pluggable generators
(`main.js` + Inja `.j2` templates + `generator.yml`). User-facing reference: `README.md` and
`docs/`. Development details live in `docs/develop/` - read the relevant page before working in
that area.

## Where to look

| Task | Read |
|---|---|
| Understand the code layout (`lib/*`, `cli/`, spec reading/conversion, JS bridge, `--version`) | [`docs/develop/architecture.md`](docs/develop/architecture.md) |
| Change C++ engine code | [`docs/develop/code-conventions.md`](docs/develop/code-conventions.md) |
| Add or change a generator under `generators/` | [`docs/develop/generator-conventions.md`](docs/develop/generator-conventions.md), `generators/README.md` |
| Build (Conan/CMake, Docker static builds, CI caching) | [`docs/develop/building.md`](docs/develop/building.md) |
| Tests (Catch2, per-generator test projects, end-to-end run) | [`docs/develop/testing.md`](docs/develop/testing.md) |
| Change CLI options, `generator.yml`, JS/template built-ins; write planning docs | [`docs/develop/docs-upkeep.md`](docs/develop/docs-upkeep.md) |
| Tidy up `CHANGELOG.md` / make a release | [`docs/develop/releasing.md`](docs/develop/releasing.md) + "Releasing" below |

## Must-know rules

- **Error/log tags**: every new `throw`/log message gets a new unique `<xxxxxxxx>` hex tag,
  produced by actually running `openssl rand -hex 4` (never hand-typed), grepped for uniqueness
  first. Details in `code-conventions.md`.
- Build is `-Werror`; run `./reformat_sources.sh` (clang-format) before committing C++ changes.
- Quick build & test:
  ```bash
  conan install . -s compiler.cppstd=20 --output-folder=build --build=missing
  cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
  cmake --build build -j && ./build/test/openapi-yagen-test
  ```
- Generated code comments and generator READMEs must not reference this repo's internals
  (`AGENTS.md`, `.j2` names, other generators) - see `generator-conventions.md`.
- `docs/develop/` and `docs/planning/` are internal and not published on the website (it only
  globs `docs/*.md`).

## Releasing

`docs/develop/releasing.md` is the generic procedure; this project's specifics:

- **Changelog script**: `./update_changelog_from_git.sh`.
- **Release workflow**: an `x.y.z` tag push runs `.github/workflows/build.yml` - static Linux
  amd64/arm64 and macOS binaries on a GitHub Release, Docker image
  `ghcr.io/openapi-yagen/openapi-yagen:<x.y.z>` and `:latest`. `--version` comes from the tag via
  `git describe`.
- **Users**: people running the CLI and generator authors - changes to CLI flags,
  `generator.yml`, JS/template built-ins, built-in generators and the code they emit,
  install/release options. A renamed generator directory or `builtin:` name is user-visible.
- **Breaking change** (major): a removed/renamed CLI flag, generator variable, or JS/template
  built-in, or generated code whose public API changes incompatibly for an existing spec.
