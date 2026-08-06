# Changelog

All notable changes to `mew` are documented here, grouped by release.
Format loosely follows [Keep a Changelog](https://keepachangelog.com/).

## [v2.0.0] - 2026-08-06

Phase 2 wrap-up: closes out the long-option / color / test-coverage arc
that started at v1.1.0. No behavior changes in this release itself —
docs, changelog, and version bump only.

### Changed
- README flags table now lists every long-option alias alongside its
  short flag, plus `--color[=WHEN]` and `NO_COLOR` behavior (#42, #49)
- `VERSION` bumped to `v2.0.0` (`src/util/usage.h`)

## [v1.6.0] - 2026-08-06

Test and CI coverage for everything added since v1.1.0 — no source changes.

### Added
- Test coverage for every long option, comparing against system `cat`
  where a portable short-flag equivalent exists, and against mew's own
  short flag otherwise (#46)
- Test coverage for `--color=always`/`never`/`auto` (piped and, via a
  `script`-allocated pseudo-terminal, on a real tty) and for `NO_COLOR`
  overriding `--color=always` (#47)
- CI step asserting `--help`/`--version` both exit `0` with non-empty
  output (#48)

## [v1.5.0] - 2026-08-06

Edge-case hardening around option parsing and file arguments.

### Fixed
- `mew .` (a directory argument) now reports a clean
  `mew: <name>: Is a directory` via an explicit `fstat`/`S_ISDIR` check
  in `open_operand`, instead of relying on `read()` failing with
  `EISDIR` by accident (#44)

### Verified
- `mew -- -n` correctly treats `-n` as a filename, not a flag —
  `getopt_long` already handled this correctly; added as an explicit
  regression test (#43)
- Mixed short/long flags in one invocation (e.g. `-n --show-ends`)
  behave identically to their all-short equivalent (#45)

## [v1.4.0] - 2026-08-06

Color output, driven by a new `--color` flag with terminal
auto-detection and `NO_COLOR` support.

### Added
- `--color[=WHEN]` flag (`auto` (default) | `always` | `never`) (#37)
- Colored non-printing character notation for `-v`/`-T`/`-E` (#38)
- `auto` mode detects a real terminal via `isatty(STDOUT_FILENO)` (#39)
- Colored line-number prefix for `-n`/`-b` (#40)
- `NO_COLOR` environment variable support — presence (any value)
  disables color unconditionally, overriding even `--color=always`,
  per the [NO_COLOR](https://no-color.org) convention (#41)
- README section documenting `--color` and `NO_COLOR` (#42)

## [v1.3.0] - 2026-08-05

Internal restructuring following the getopt_long migration — no
behavior changes, module boundaries only.

### Changed
- `t_opts` and flag constants moved into a dedicated `includes/options.h`,
  out of `parser.h` (#34)
- `print_help`/`print_version`/`usage_exit`/`usage_exit_long` extracted
  into `src/util/usage.c`/`usage.h`, leaving `error.c` to just
  `file_error` (#33)
- File-vs-stdin resolution (`-` vs. a real path, open/close) extracted
  into `src/files.c`/`files.h` (#35)
- `longopts[]` table kept `static` inside `parser.c` rather than split
  into a separate file — reconsidered from the original issue scope
  since the table is tightly coupled to the `switch` that consumes it (#32)
- Every file's includes audited: dead includes removed, implementation-only
  includes moved out of headers and into the `.c` files that use them (#36)

## [v1.2.0] - 2026-08-05

Help, version, and consistent usage-error wording.

### Added
- `-h`/`--help` flag (#28)
- `--version` flag (#29)

### Changed
- `VERSION` centralized as a single named constant (#30)
- stderr usage-error wording aligned with `--help`'s phrasing (#31)

## [v1.1.0] - 2026-08-05

Long-option aliases for every existing short flag, via `getopt_long`.

### Added
- `--number`, `--number-nonblank`, `--show-ends`, `--show-tabs`,
  `--squeeze-blank`, `--show-nonprinting`, `--show-all` as long aliases
  for `-n`, `-b`, `-E`, `-T`, `-s`, `-v`, `-A` (#21-#27)

### Changed
- Parsing migrated from `getopt(3)` to `getopt_long(3)`, keeping all
  short flags and POSIX "flags before operands" behavior working (#20)

## [v1.0.0] - 2026-08-05

First complete `mew`: a strict-C11, POSIX-only `cat` reimplementation —
no `stdio`, no `malloc`, just `open`/`read`/`write`/`close`.

### Added
- Full short-flag set: `-A`, `-b`, `-e`, `-E`, `-n`, `-s`, `-t`, `-T`,
  `-u`, `-v`, parsed via `getopt(3)` into a `t_opts` struct (#3-#10)
- `write_all` loop so partial writes never silently drop bytes; write
  failures propagate to a real exit code the same way read failures do
- `make test` running a POSIX shell suite against `cat`, in CI across
  Ubuntu/macOS and gcc/clang (#11, #12)
- Named read-buffer-size constant instead of a magic number (#1)
- `t_opts` struct introduced to carry flags through the call chain (#2)
