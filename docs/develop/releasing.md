# Changelog and releases

A generic process for maintaining `CHANGELOG.md` and cutting a release. It is
project-agnostic; everything project-specific (the changelog script's name,
what the release workflow builds and publishes, who the users are, what counts
as a breaking change) lives in the project's `AGENTS.md`, in its "Releasing"
section. Read both.

A release is an annotated `x.y.z` git tag (plain semver, no `v` prefix).
Pushing it triggers the project's CI release workflow, which builds the
release artifacts and publishes them on a GitHub Release for that tag. The
application's own version string is normally derived from the same tag via
`git describe`, so the tag is the single source of truth for the version.

Never create or push a tag unless explicitly asked to make a release.

## Changelog

`CHANGELOG.md` is seeded from git by the project's changelog script: it
inserts a `# X.X.X (<date>)` heading on top of the file with one `- ` bullet
per commit subject since the last version in the file. `X.X.X` is a
placeholder, replaced with the real version when releasing. The script
refuses to run while an `X.X.X` section is still present.

### Tidying up the changelog

Edit only the section for commits since the last tagged release
(`git tag -l`) — typically the topmost, still-unreleased `# X.X.X` heading.
For each entry in that section:

- Drop entries of no interest to the project's users (see `AGENTS.md` for who
  they are): routine `README`/`AGENTS.md` touch-ups, internal
  test/CI/build-tooling churn, dependency and base image bumps, typo fixes,
  reverted or rolled-back intermediate steps, and other commits that don't
  change user-visible behavior, a public interface (CLI flags, APIs, file
  formats, configuration), or the install/release process.
- Refactors and source-tree reorganizations (moving files, renaming internal
  classes or folders, splitting modules) are not interesting on their own —
  drop them, unless they change something users see or pass in (a path, a
  name, a flag, the look or behavior); then describe that change instead.
- Rephrase entries that describe an implementation step (an internal class or
  function name, a source file path, a commit-scope prefix) into a plain
  description of the resulting user-visible capability.
- Consolidate a run of incremental commits that build up one feature ("Add X",
  "Extend X", "Document X") into a single bullet describing the feature's end
  state.
- Multiple commits touching the same component collapse into one bullet
  naming that component once, not one bullet per commit.
- Keep bug fixes, new features, new public interfaces, and
  packaging/install/release changes — these are what users read the changelog
  for. Still aim to leave only what matters most: fold minor, low-signal
  entries into a closely related bullet instead of listing each on its own.

Preserve the formatting: one heading per release (`# <x.y.z> (<date>)`,
`# X.X.X (<date>)` while unreleased), one `- ` bullet per entry, in English.
Do not touch already-released sections.

Order entries in three groups: new features first, then changes to existing
behavior (including docs/process changes), then bug fixes last. Within a
group, keep the original newest-first order.

## Making a release

1. Pull fresh tags: `git fetch --tags --force origin` (a tag may have been
   created or moved on GitHub).
2. Run the project's changelog script (see `AGENTS.md`).
3. Tidy up the new section (see above).
4. Pick the version from the last tag (`git tag -l`), following semver:
   - only bug fixes → patch (`1.0.0` → `1.0.1`);
   - anything new → minor (`1.0.0` → `1.1.0`);
   - a change that breaks existing users — a removed or renamed public
     interface, or a behavior change existing users must adapt to (see
     `AGENTS.md` for what that means for this project) → major
     (`1.0.0` → `2.0.0`). While still on `0.x`, a breaking change bumps
     minor instead.

   **Stop and ask the user which version to release** — don't pick it
   silently. Propose the version the rules above point to and explain which
   changelog entries make it a patch, minor, or major bump. Use whichever
   version the user approves or names instead, and only then replace `X.X.X`
   in the heading with it.
5. Commit only `CHANGELOG.md`, as `Release v<x.y.z>`.
6. Create an annotated tag on that commit whose message is exactly that
   release's changelog bullets (no heading) — one unwrapped line per bullet,
   written to a file first rather than passed via `-m` — then push the commit
   and the tag:

   ```bash
   git tag -a <x.y.z> -F <message-file>
   git push origin HEAD <x.y.z>
   ```
