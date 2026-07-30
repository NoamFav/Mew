# Contributing to minette

Welcome! This guide covers everything you need to contribute to this project — from setting up your fork to getting your PR merged. If this is your first time collaborating on GitHub, you're in the right place.

## Table of Contents

1. [The Big Picture](#the-big-picture)
2. [Fork & Clone](#fork--clone)
3. [Stay in Sync with Upstream](#stay-in-sync-with-upstream)
4. [Create a Branch](#create-a-branch)
5. [Build & Test Locally](#build--test-locally)
6. [Write Your Commits](#write-your-commits)
7. [Rebase Before You Push](#rebase-before-you-push)
8. [Open a Pull Request](#open-a-pull-request)
9. [Review Process](#review-process)
10. [Code Style](#code-style)

---

## The Big Picture

GitHub collaboration follows this pattern:

```
[upstream: NoamFav/Minette]   ← the "real" repo — everyone's work ends up here
          ↕  sync
[your fork: you/Minette]      ← your personal copy of the repo on GitHub
          ↕  push / pull
[your machine: ~/Minette]     ← where you actually write code
```

You **never push directly to upstream**. You push to *your fork*, then open a **Pull Request** (PR) to propose your changes. A maintainer reviews and merges it.

---

## Fork & Clone

### 1. Fork the repo

Go to [https://github.com/NoamFav/Minette](https://github.com/NoamFav/Minette) and click **Fork** in the top-right corner. This creates `your-username/Minette` on GitHub — your own personal copy.

### 2. Clone your fork to your machine

```sh
git clone https://github.com/YOUR_USERNAME/Minette.git
cd Minette
```

### 3. Add the upstream remote

This connects your local repo to the original project so you can pull in updates:

```sh
git remote add upstream https://github.com/NoamFav/Minette.git
```

Verify both remotes are set:

```sh
git remote -v
# origin    https://github.com/YOUR_USERNAME/Minette.git (fetch)
# origin    https://github.com/YOUR_USERNAME/Minette.git (push)
# upstream  https://github.com/NoamFav/Minette.git (fetch)
# upstream  https://github.com/NoamFav/Minette.git (push)
```

---

## Stay in Sync with Upstream

Before starting any new work, always pull the latest changes from the main project. This prevents merge conflicts later.

```sh
git fetch upstream
git checkout main
git merge upstream/main
git push origin main          # keep your fork's main up to date too
```

Make this a habit. Do it every time you sit down to work on a new branch.

---

## Create a Branch

**Never work directly on `main`.** Create a dedicated branch for every feature or fix:

```sh
# for new features
git checkout -b feat/short-description

# for bug fixes
git checkout -b fix/short-description
```

Examples:

```sh
git checkout -b feat/flag-n-line-numbers
git checkout -b fix/partial-write-on-stdin
git checkout -b docs/improve-readme
```

Keep branch names lowercase, hyphenated, and descriptive.

---

## Build & Test Locally

Always build and test before committing:

```sh
make           # debug build — AddressSanitizer + UBSanitizer enabled
make release   # release build — -O2, no sanitizers
make re        # full rebuild from scratch
```

Run your binary manually against different inputs:

```sh
./mew file.txt
./mew file1.txt file2.txt
./mew -                        # stdin
echo "hello world" | ./mew
./mew /nonexistent             # should print an error to stderr
```

> The debug build links **AddressSanitizer** and **UndefinedBehaviorSanitizer**. If your code has a memory bug or undefined behaviour, it will abort with a detailed message pointing straight to the problem. Do not disable these.

---

## Write Your Commits

Keep commits small and focused — one logical change per commit. Avoid mixing unrelated changes in a single commit.

**Message format:**

```
<type>(<scope>): <short description>

Optional longer explanation for non-obvious changes.
```

**Types:**

| Type | Use for |
|------|---------|
| `feat` | new feature or behaviour |
| `fix` | bug fix |
| `refactor` | restructuring code without changing behaviour |
| `docs` | documentation only |
| `style` | formatting, whitespace (no logic change) |
| `build` | Makefile, compiler flags |
| `test` | adding or updating tests |

**Examples:**

```
feat(flags): add -n flag for line numbering
fix(display): handle partial write on interrupted syscall
refactor(helpers): extract getlen to a shared utility
docs(contributing): add branch naming conventions
build(makefile): add lint target with cppcheck
```

**Rules:**
- Subject line ≤ 72 characters
- Use imperative mood: "add", not "added" or "adds"
- No trailing period on the subject line

---

## Rebase Before You Push

This repo enforces **linear history** — no merge commits allowed. That means instead of merging `main` into your branch to pick up new changes, you **rebase** your branch on top of `main`.

### What's the difference?

```
# Merge (creates an ugly merge commit — not allowed here)
main:       A ── B ── C ── M
                           ↑ merge commit
your branch:        D ── E

# Rebase (replays your commits on top of main — clean, linear)
main:       A ── B ── C
                       └── D' ── E'   ← your commits, replayed on top
```

### How to rebase

Before opening a PR, always do this:

```sh
# 1. Pull the latest main from upstream
git fetch upstream
git checkout main
git merge upstream/main

# 2. Switch back to your branch and rebase
git checkout feat/your-feature-name
git rebase main
```

### If there are conflicts

Git will pause and tell you which file has a conflict. Open it, look for the conflict markers, and resolve it:

```
<<<<<<< HEAD
int current_main_version = 1;
=======
int your_version = 2;
>>>>>>> feat/your-feature-name
```

Edit the file to keep what's correct, then:

```sh
git add <the-file-you-fixed>
git rebase --continue
```

Repeat for each conflicting commit until the rebase finishes.

### Force-push after rebasing

Because rebase rewrites your commits, you need to force-push to your fork:

```sh
git push --force-with-lease origin feat/your-feature-name
```

> Use `--force-with-lease` not `--force` — it refuses to push if someone else has pushed to the same branch in the meantime, which protects you from accidentally overwriting their work.

> **Never rebase or force-push a branch that someone else is actively reviewing.** Add new commits instead, and rebase only once the review is done.

---

## Open a Pull Request

Once your branch is ready:

```sh
git push origin feat/your-feature-name
```

Go to [https://github.com/NoamFav/Minette](https://github.com/NoamFav/Minette) — GitHub will show a banner prompting you to open a PR. Click **Compare & pull request**.

Fill in:

- **Title** — follow the same format as commits: `feat(flags): add -n line numbering`
- **Description** — explain what the PR does, why it matters, and any edge cases you tested
- Reference related issues with `Closes #N` if applicable

### Before submitting, double-check:

- [ ] Code compiles cleanly with no warnings: `make re`
- [ ] Tested manually with files, stdin, `-`, and missing files
- [ ] No debug prints left in the code
- [ ] Commit messages follow the format above

---

## Review Process

1. A maintainer will review your PR and may leave comments or request changes
2. Address feedback by adding **new commits** to the same branch — do not rewrite history while a review is open
3. Once all discussions are resolved and the PR is approved, a maintainer merges it
4. **Do not merge your own PR** — wait for approval

Reviews exist to maintain code quality and catch bugs, not to judge you. Every contributor gets reviewed the same way.

---

## Code Style

This project targets **C11** with strict compiler flags. Your code must compile with zero warnings under:

```
-std=c11 -Wall -Wextra -Werror -Wshadow -Wswitch-enum \
-Wformat=2 -Wwrite-strings -Wvla -Wmissing-prototypes
```

### Rules

- **No `printf` or `stdlib.h`** — use `write()`, `read()`, `open()`, `close()` only
- **No global variables**
- **Declare variables at the top** of their scope block, before any statements
- **Errors go to stderr** (fd 2), never stdout
- **Functions stay short and single-purpose** — if a function is getting long, split it
- **Return values matter** — check them; propagate errors to the caller
- **Match the existing style** — when in doubt, look at the surrounding code and copy it exactly

### What is allowed

- `string.h` — `strlen`, `memcpy`, `memmove`, `strerror` and friends are fine
- `stdlib.h` — `exit()` only; no dynamic allocation
- `getopt` (from `unistd.h`) — use it if you're adding flag support

### What is not allowed

- `stdio.h` — `fopen`, `printf`, `fgets` and friends are off-limits; use `read()` / `write()` directly; the whole point of this project is learning unbuffered I/O
- `malloc` / `free` — `cat` doesn't need a heap; if your design requires dynamic allocation, reconsider the design
- VLAs — disabled by `-Wvla`

---

## Questions?

Open an issue on [GitHub](https://github.com/NoamFav/Minette/issues) or ping us on Discord.
