# Docs upkeep

If you change the CLI options, update `README.md`'s "CLI reference" section. If you change the
`generator.yml` schema, built-in JS/template functions, or the globals exposed to `main.js`, update
the corresponding page under [`docs/`](../README.md) instead - that's the user-facing source of
truth for writing a generator and easily drifts from code. `README.md`'s `## TODO` section tracks
known gaps/planned features; check it before assuming something missing is an oversight rather
than planned work.

Internal analysis/planning write-ups (design tradeoffs, a backlog item's own "why" and acceptance
criteria, anything meant for whoever picks the work back up rather than for someone writing a
generator) go under `docs/planning/`, not directly under `docs/`: the website's `include` list
(`website/docusaurus.config.ts`) only globs `docs/*.md` - not recursive - so a `docs/planning/*.md`
file never reaches the published site. `TODO-improvements.md` at the repo root is the running
backlog these documents typically feed into. Likewise, contributor/maintainer process docs (e.g. the
release procedure) live under `docs/develop/` and stay off the site for the same reason.
