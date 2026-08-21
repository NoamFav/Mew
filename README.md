<div align="center">

<!-- Animated Banner -->
<img src="https://capsule-render.vercel.app/api?type=venom&height=280&color=gradient&customColorList=4&text=MEW&fontSize=130&fontColor=fff&animation=twinkling&desc=A%20cat%20clone%20built%20the%20right%20way%20%E2%80%94%20POSIX%20%C2%B7%20strict%20C11%20%C2%B7%20zero%20deps&descSize=22&descAlignY=72&stroke=FFFFFF&strokeWidth=1" alt="mew Banner" />

<!-- Animated Typing -->
<img src="https://readme-typing-svg.herokuapp.com?font=Fira+Code&size=22&pause=1000&color=FF6B35&center=true&vCenter=true&multiline=true&repeat=true&width=900&height=80&lines=Read+files%2C+concatenate%2C+print+to+stdout+%E2%80%94+just+like+cat;Built+in+C11+%C2%B7+Strict+flags+%C2%B7+ASAN+%2B+UBSAN+in+debug+mode" alt="Typing SVG" />

<br>

<!-- Badges -->
[![C11](https://img.shields.io/badge/C11-00599C?style=for-the-badge&logo=c&logoColor=white&labelColor=0D1117)](https://en.cppreference.com/w/c/11)
[![POSIX](https://img.shields.io/badge/POSIX-zero%20deps-brightgreen?style=for-the-badge&labelColor=0D1117)]()
[![CI](https://img.shields.io/github/actions/workflow/status/NoamFav/Mew/ci.yml?style=for-the-badge&label=CI&logo=github-actions&logoColor=white&labelColor=0D1117)](https://github.com/NoamFav/Mew/actions)
[![License](https://img.shields.io/badge/Apache%202.0-D22128?style=for-the-badge&logo=opensourceinitiative&logoColor=white&labelColor=0D1117)](./LICENSE.md)
[![42](https://img.shields.io/badge/42-school-000000?style=for-the-badge&labelColor=0D1117)]()

</div>

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- What is mew -->
<div align="center">
  <img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=28&pause=1000&color=FF6B35&center=true&width=800&lines=%F0%9F%90%B1+WHAT+IS+MEW+%3F" alt="What is mew" />
</div>

<br>

<table align="center">
<tr>
<td width="50%" valign="top">

### The Project
**mew** is a `cat` reimplementation in C11. No `printf`, no `stdlib` beyond `exit`/`getenv` — just four syscalls at its core: `open`, `read`, `write`, `close`.

It handles:
- One or more **file arguments**
- **Standard input** — when called with no args, or with `-` as a filename
- **Short and long flags** — parsed with `getopt_long(3)` into a `t_opts` struct
- **Colorized output** — `--color[=WHEN]`, terminal auto-detection, `NO_COLOR` support
- Proper **error reporting** on stderr, prefixed with the program name — including a clean message for directory arguments
- **Partial writes** — a `write_all` loop ensures every byte reaches the output

</td>
<td width="50%" valign="top">

### Why Build This?
Building `cat` from scratch forces you to deal with real POSIX I/O:

```
 file argument → open() → fd
        │
        ▼
    read() in a loop
        │
        ▼
    write_all()    ← handles partial writes
        │
        ▼
    close(fd)

 error anywhere? → stderr with strerror(errno)
```

No shortcuts. No buffering from the C library. Real I/O.

</td>
</tr>
</table>

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- Features -->
<div align="center">
  <img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=28&pause=1000&color=FF69B4&center=true&width=800&lines=%F0%9F%9A%80+FEATURES+%F0%9F%9A%80" alt="Features" />
</div>

<br>

<table align="center">
<tr>
<td width="50%" valign="top">

#### Input Handling
- **Files** — pass any number of paths as arguments
- **Stdin** — `mew` with no arguments reads from stdin
- **`-` as filename** — explicit stdin in the middle of a file list
- **Mixed** — `mew file1 - file2` works as expected

</td>
<td width="50%" valign="top">

#### Correctness
- **Partial writes handled** — `write_all` loops until every byte is flushed
- **Errors to stderr** — `mew: <filename>: <reason>` format, exit code 1
- **Write failures caught too** — a closed fd or full disk fails the same way a read error does, instead of silently exiting 0
- **Directory arguments** — detected up front via `fstat`, reported as `mew: <name>: Is a directory` instead of misbehaving on a directory fd
- **No stdio buffering** — `read()`/`write()` go straight to the kernel; no `fopen`, no `printf`
- **Strict compilation** — `-Wall -Wextra -Werror` plus shadow, VLA, format checks, clean on GCC and Clang
- **Tested** — `make test` runs a byte-for-byte suite against `cat` plus flag, edge-case, and color coverage, in CI across Ubuntu/macOS and gcc/clang

</td>
</tr>
</table>

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- Flags -->
<div align="center">
  <img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=28&pause=1000&color=00D9FF&center=true&width=800&lines=%F0%9F%9A%A9+FLAGS+%F0%9F%9A%A9" alt="Flags" />
</div>

<br>

`src/parser.c` parses these with `getopt_long(3)` into a `t_opts` struct that's threaded through `display_loop()` and `display_file()`. Every flag below accepts either form — `mew -n file` and `mew --number file` are equivalent:

| Short | Long | Meaning |
|-------|------|---------|
| `-A` | `--show-all` | Equivalent to `-vET` |
| `-b` | `--number-nonblank` | Number nonempty output lines, overrides `-n` |
| `-e` | — | Equivalent to `-vE` |
| `-E` | `--show-ends` | Display `$` at the end of each line |
| `-h` | `--help` | Display help and exit |
| `-n` | `--number` | Number all output lines |
| `-s` | `--squeeze-blank` | Suppress repeated empty output lines |
| `-t` | — | Equivalent to `-vT` |
| `-T` | `--show-tabs` | Display TAB characters as `^I` |
| `-u` | — | Accepted and ignored (POSIX compatibility) |
| `-v` | `--show-nonprinting` | Show non-printing characters using `^` and `M-` notation |
| `-V` | `--version` | Output version information and exit |
| — | `--color[=WHEN]` | Colorize mew's own output — see [Color Output](#color-output) below |

An unrecognized flag prints a `usage:` message to stderr and exits with status 1. `--` ends option parsing explicitly — everything after it is treated as a filename, even if it looks like a flag (`mew -- -n` reads a file literally named `-n`).

`-v` (and the non-printing part of `-e`/`-t`/`-A`) covers the full byte
range, not just ASCII control characters:

| Byte range | Rendered as |
|------------|-------------|
| `0x00`–`0x1F` (excluding `\t`/`\n`) | `^@`–`^_` |
| `0x7F` (DEL) | `^?` |
| `0x80`–`0x9F` | `M-^@`–`M-^_` |
| `0xA0`–`0xFE` | `M-` + the literal stripped byte |
| `0xFF` | `M-^?` |

#### Color Output

`--color[=WHEN]` controls ANSI color on mew's own output: line numbers
(`-n`/`-b`), non-printing notation (`-v`/`-T`/`-E`), and mew's
`--help`/`--version`/error messages. Plain file content is never
colorized — only what mew itself renders around it.

| `WHEN` | Behavior |
|--------|----------|
| `auto` (default) | Colored only when stdout is a real terminal |
| `always` | Always colored, even when piped or redirected |
| `never` | Never colored |

`--color` with no `=WHEN` behaves like `--color=auto`.

```sh
./mew -n file.txt              # colored only if stdout is a terminal
./mew -n file.txt | less -R    # --color=always needed to survive the pipe
./mew --color=always -n file.txt | less -R
./mew --color=never -n file.txt   # force plain, e.g. for scripts
```

> [!NOTE]
> `NO_COLOR` overrides everything above, including `--color=always` —
> if it's set to *anything* (even an empty string), mew never emits
> color. This follows the [NO_COLOR](https://no-color.org) convention:
> presence of the variable is what matters, not its value.

> [!NOTE]
> `parser.c` targets `#define _POSIX_C_SOURCE 200809L` (POSIX.1-2008)
> explicitly, rather than relying on whatever a given libc exposes by
> default — this is what makes `getopt`/`optarg`/`optind` visible under
> strict `-std=c11`. Its optstring is also prefixed with `+`, forcing
> POSIX's non-permuting "options before operands" behavior instead of
> glibc's GNU-style flag reordering. Concretely: `mew -n file.txt` works,
> `mew file.txt -n` doesn't — on every platform mew targets, not just some.

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- Quickstart -->
<div align="center">
  <img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=28&pause=1000&color=00D9FF&center=true&width=800&lines=%E2%9C%A8+QUICKSTART+%E2%9C%A8" alt="Quickstart" />
</div>

<br>

```sh
# Clone
git clone https://github.com/NoamFav/Mew.git && cd Mew

# Build (debug — ASAN + UBSAN enabled)
make

# Or build for release
make release

# Run
./mew file1.txt file2.txt
./mew -                     # read from stdin
echo "hello" | ./mew
./mew -n file1.txt          # with flags, e.g. number output lines
./mew --number-nonblank file1.txt   # long-option form works everywhere -n/-b/etc. do
./mew --help                        # usage + full flag list, exit 0
./mew --version                     # e.g. "mew: v2.0.0", exit 0
```

> [!NOTE]
> The debug build links **AddressSanitizer** and **UndefinedBehaviorSanitizer**.
> Any memory error or undefined behaviour will abort with a detailed message —
> this is intentional and very useful while developing.

> [!IMPORTANT]
> `mew` calls POSIX syscalls directly (`open`/`read`/`write`/`close`,
> `getopt`) instead of going through portable C `stdio` — it needs a real
> POSIX environment to build and run:
> - **Linux, macOS, the BSDs** — native, no extra setup
> - **Windows** — only via **WSL2** (a real Linux kernel); raw `cmd.exe`
>   and PowerShell don't implement the POSIX interface `mew` depends on,
>   so it won't build there

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- Development -->
<div align="center">
  <img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=28&pause=1000&color=6A5ACD&center=true&width=800&lines=%F0%9F%9B%A0%EF%B8%8F+DEVELOPMENT+%F0%9F%9B%A0%EF%B8%8F" alt="Development" />
</div>

<br>

```sh
make          # debug build — ASAN + UBSAN, -g3
make release  # release build — -O2 -DNDEBUG, no sanitizers
make re       # fclean + all (full rebuild)
make test     # build + run tests/run_tests.sh + help/man-page sync guard
make clean    # remove build/ directory
make fclean   # clean + remove the mew binary
make install  # install binary + man page (PREFIX=/usr/local by default)
make uninstall
make man      # preview docs/mew.1 without installing
```

#### Installing

```sh
make install            # PREFIX=/usr/local by default
make install PREFIX=$HOME/.local DESTDIR=""   # or anywhere you like
```

Both `PREFIX` and `DESTDIR` are honored (staging/packaging safe): the binary
lands in `$DESTDIR$PREFIX/bin/mew` (0755) and this manual page in
`$DESTDIR$PREFIX/share/man/man1/mew.1` (0644). After installing,
`man mew` gives you the full offline reference — every option, exit status,
environment variable and example, kept honest by a CI guard that fails if a
flag ever ships without documentation (`tests/check_help_sync.sh`).

<details>
<summary><b>📁 Project Layout</b></summary>
<br>

```
Mew/
├── main.c                  Entry point — calls mew()
├── includes/
│   ├── mew.h               mew() prototype
│   └── options.h           t_opts struct + OPT_COLOR/OPT_RANGE, COLOR_* enum
├── src/
│   ├── mew.c               Argument loop — delegates to files.c + display_file()
│   ├── files.c / files.h   open_operand()/close_operand() — "-" vs. real path
│   ├── parser.c            getopt_long() flag parsing → t_opts
│   ├── parser.h            opts_parser() prototype
│   ├── out/
│   │   ├── display_file.c  Core I/O — read() loop, drives rendering + flush
│   │   ├── display_file.h  display_file() prototype + t_linestate
│   │   ├── render.c        Special-character + color rendering (-T/-E/-v/-n)
│   │   ├── render.h        is_special() / emit_special() / emit_prefix()
│   │   ├── outbuf.c        Buffered writer — ob_append() / ob_flush()
│   │   └── outbuf.h
│   └── util/
│       ├── error.c         file_error() — runtime I/O error reporting
│       ├── usage.c         print_help()/print_version()/usage_exit[_long]()
│       ├── colors.c        col_on()/col_off()/put_c()/iscolor() — NO_COLOR + isatty
│       ├── io.c            write_all()
│       ├── num.c           count_digits(), num_to_buf()
│       └── str.c           getlen(), nf_memcpy(), nf_strcmp()
├── tests/
│   ├── run_tests.sh        POSIX shell test suite
│   ├── inputs/             Fixture files (text + binary)
│   └── expected/           Expected output per fixture
├── tools/
│   └── genh.sh             Generates a header from a .c file (cproto)
├── build/
│   ├── debug/              Object files for debug build
│   └── release/            Object files for release build
└── Makefile
```

</details>

<details>
<summary><b>⚙️ Compiler Flags</b></summary>
<br>

```
Common   -std=c11 -Wall -Wextra -Werror -Wshadow -Wswitch-enum
         -Wformat=2 -Wwrite-strings -Wvla -Wmissing-prototypes

Debug    -g3 -gdwarf-4 -O0 -fno-omit-frame-pointer
         -fsanitize=address,undefined -fno-sanitize-recover=all

Release  -O2 -DNDEBUG
```

</details>

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- Contributing -->
<div align="center">
  <img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=28&pause=1000&color=FF69B4&center=true&width=800&lines=%F0%9F%A4%9D+CONTRIBUTING+%F0%9F%A4%9D" alt="Contributing" />
</div>

<br>

<div align="center">

This is a collaborative 42 project — contributions are welcome and expected.
New to GitHub collaboration? The [CONTRIBUTING.md](./CONTRIBUTING.md) covers everything from forking to getting your PR merged, step by step.

See [CHANGELOG.md](./CHANGELOG.md) for what changed in each release.

</div>

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- Footer -->
<div align="center">

<img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=22&pause=1000&color=FF6B35&center=true&width=800&lines=Thanks+for+stopping+by!;Feel+free+to+%E2%AD%90+the+repo;Let%27s+build+something+amazing." alt="Footer typing" />

<br>

Made with ♥ by [NoamFav](https://github.com/NoamFav) and 42 contributors

<img src="https://capsule-render.vercel.app/api?type=waving&height=120&color=gradient&customColorList=4&section=footer" />

</div>
