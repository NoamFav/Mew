<div align="center">

<!-- Animated Banner -->
<img src="https://capsule-render.vercel.app/api?type=venom&height=280&color=gradient&customColorList=4&text=MEW&fontSize=130&fontColor=fff&animation=twinkling&desc=A%20cat%20clone%20built%20the%20right%20way%20%E2%80%94%20POSIX%20%C2%B7%20strict%20C11%20%C2%B7%20zero%20deps&descSize=22&descAlignY=72&stroke=FFFFFF&strokeWidth=1" alt="mew Banner" />

<!-- Animated Typing -->
<img src="https://readme-typing-svg.herokuapp.com?font=Fira+Code&size=22&pause=1000&color=FF6B35&center=true&vCenter=true&multiline=true&repeat=true&width=900&height=80&lines=Read+files%2C+concatenate%2C+print+to+stdout+%E2%80%94+just+like+cat;Built+in+C11+%C2%B7+Strict+flags+%C2%B7+ASAN+%2B+UBSAN+in+debug+mode" alt="Typing SVG" />

<br>

<!-- Badges -->
[![C11](https://img.shields.io/badge/C11-00599C?style=for-the-badge&logo=c&logoColor=white&labelColor=0D1117)](https://en.cppreference.com/w/c/11)
[![POSIX](https://img.shields.io/badge/POSIX-zero%20deps-brightgreen?style=for-the-badge&labelColor=0D1117)]()
[![CI](https://img.shields.io/github/actions/workflow/status/NoamFav/Minette/ci.yml?style=for-the-badge&label=CI&logo=github-actions&logoColor=white&labelColor=0D1117)](https://github.com/NoamFav/Minette/actions)
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
**mew** (`minette`) is a `cat` reimplementation in C11. No `printf`, no `stdlib` — just four syscalls: `open`, `read`, `write`, `close`.

It handles:
- One or more **file arguments**
- **Standard input** — when called with no args, or with `-` as a filename
- Proper **error reporting** on stderr, prefixed with the program name
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
- **No stdio buffering** — `read()`/`write()` go straight to the kernel; no `fopen`, no `printf`
- **Strict compilation** — `-Wall -Wextra -Werror` plus shadow, VLA, format checks

</td>
</tr>
</table>

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

<!-- Quickstart -->
<div align="center">
  <img src="https://readme-typing-svg.herokuapp.com?font=Orbitron&size=28&pause=1000&color=00D9FF&center=true&width=800&lines=%E2%9C%A8+QUICKSTART+%E2%9C%A8" alt="Quickstart" />
</div>

<br>

```sh
# Clone
git clone https://github.com/NoamFav/Minette.git && cd Minette

# Build (debug — ASAN + UBSAN enabled)
make

# Or build for release
make release

# Run
./mew file1.txt file2.txt
./mew -                     # read from stdin
echo "hello" | ./mew
```

> [!NOTE]
> The debug build links **AddressSanitizer** and **UndefinedBehaviorSanitizer**.
> Any memory error or undefined behaviour will abort with a detailed message —
> this is intentional and very useful while developing.

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
make clean    # remove build/ directory
make fclean   # clean + remove the mew binary
```

<details>
<summary><b>📁 Project Layout</b></summary>
<br>

```
Minette/
├── main.c              Entry point — calls minette()
├── includes/
│   └── minette.h       All declarations in one header
├── src/
│   ├── minette.c       Argument loop — delegates to display_file()
│   ├── display_file.c  Core I/O — read() loop + write_all()
│   └── helpers.c       getlen(), file_error() for stderr output
├── build/
│   ├── debug/          Object files for debug build
│   └── release/        Object files for release build
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
