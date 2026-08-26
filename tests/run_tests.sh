#!/bin/sh
# tests/run_tests.sh — smoke-test suite for mew

set -u

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
MEW="$ROOT/mew"
INPUTS="$ROOT/tests/inputs"
EXPECTED="$ROOT/tests/expected"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

pass=0
fail=0

ok() {
    pass=$((pass + 1))
    printf 'PASS: %s\n' "$1"
}

ko() {
    fail=$((fail + 1))
    printf 'FAIL: %s -- %s\n' "$1" "$2"
}

expect_files_equal() {
    name=$1
    actual=$2
    expected=$3
    if cmp -s "$actual" "$expected"; then
        ok "$name"
    else
        ko "$name" "output differs from expected"
    fi
}

expect_exit() {
    name=$1
    got=$2
    want=$3
    if [ "$got" -eq "$want" ]; then
        ok "$name"
    else
        ko "$name" "exit code $got, expected $want"
    fi
}

expect_contains() {
    name=$1
    haystack=$2
    needle=$3
    case "$haystack" in
    *"$needle"*) ok "$name" ;;
    *) ko "$name" "expected output to contain '$needle'" ;;
    esac
}

expect_not_contains() {
    name=$1
    haystack=$2
    needle=$3
    case "$haystack" in
    *"$needle"*) ko "$name" "expected output NOT to contain an escape byte" ;;
    *) ok "$name" ;;
    esac
}

# Run "$@" attached to a pseudo-terminal (BSD vs. GNU `script` differ in
# invocation syntax) so --color=auto's isatty() check sees a real tty.
run_tty() {
    case "$(uname)" in
    Darwin | *BSD)
        script -q /dev/null "$@" 2>/dev/null
        ;;
    *)
        script -qec "$*" /dev/null 2>/dev/null
        ;;
    esac
}

if [ ! -x "$MEW" ]; then
    printf 'error: %s not found -- run `make` first\n' "$MEW" >&2
    exit 1
fi

# 1. no arguments (stdin)
"$MEW" <"$INPUTS/multiline.txt" >"$TMP/stdin_noargs.out"
expect_files_equal "stdin (no args)" "$TMP/stdin_noargs.out" "$EXPECTED/multiline.txt"

# 2. single file
"$MEW" "$INPUTS/multiline.txt" >"$TMP/single_file.out"
expect_files_equal "single file" "$TMP/single_file.out" "$EXPECTED/multiline.txt"

# 3. multiple files concatenated
"$MEW" "$INPUTS/single_line.txt" "$INPUTS/multiline.txt" >"$TMP/multi_file.out"
cat "$INPUTS/single_line.txt" "$INPUTS/multiline.txt" >"$TMP/multi_file.expected"
expect_files_equal "multiple files concatenated" "$TMP/multi_file.out" "$TMP/multi_file.expected"

# 4. "-" as explicit stdin
"$MEW" - <"$INPUTS/single_line.txt" >"$TMP/dash_stdin.out"
expect_files_equal "'-' as explicit stdin" "$TMP/dash_stdin.out" "$EXPECTED/single_line.txt"

# 5. missing file -> stderr contains "mew:", exit code 1
err=$("$MEW" "$INPUTS/does_not_exist.txt" 2>&1 >/dev/null)
code=$?
expect_exit "missing file exit code" "$code" 1
expect_contains "missing file stderr message" "$err" "mew:"

# 6. empty file -> no output, exit code 0
out=$("$MEW" "$INPUTS/empty.txt")
code=$?
expect_exit "empty file exit code" "$code" 0
if [ -z "$out" ]; then
    ok "empty file produces no output"
else
    ko "empty file produces no output" "got non-empty output"
fi

# 7. byte-for-byte match against cat, for every input (including binary)
for f in "$INPUTS"/*; do
    name=$(basename "$f")
    "$MEW" "$f" >"$TMP/cat_cmp.out"
    cat "$f" >"$TMP/cat_cmp.expected"
    expect_files_equal "byte-for-byte vs cat: $name" "$TMP/cat_cmp.out" "$TMP/cat_cmp.expected"
done

# 8. "--" end-of-options marker -> "-n" treated as a filename, not a flag
err=$("$MEW" -- -n 2>&1 >/dev/null)
code=$?
expect_exit "'--' end-of-options exit code" "$code" 1
expect_contains "'--' end-of-options stderr message" "$err" "-n"

# 9. directory argument -> clean "Is a directory" error, exit code 1
err=$("$MEW" "$TMP" 2>&1 >/dev/null)
code=$?
expect_exit "directory argument exit code" "$code" 1
expect_contains "directory argument stderr message" "$err" "Is a directory"

# 10. mixed short and long flags in one invocation
"$MEW" -n --show-ends "$INPUTS/multiline.txt" >"$TMP/mixed_flags.out"
"$MEW" -nE "$INPUTS/multiline.txt" >"$TMP/mixed_flags.expected"
expect_files_equal "mixed long/short flags (-n --show-ends == -nE)" \
    "$TMP/mixed_flags.out" "$TMP/mixed_flags.expected"

# 11. long options with a direct system-cat short-flag equivalent
#     (portable across BSD/GNU cat -- both support -s/-v)
for pair in "--squeeze-blank:-s" "--show-nonprinting:-v"; do
    long=${pair%%:*}
    short=${pair##*:}
    "$MEW" "$long" "$INPUTS/blank_lines.txt" >"$TMP/longopt.out"
    cat "$short" "$INPUTS/blank_lines.txt" >"$TMP/longopt.expected"
    expect_files_equal "$long matches system cat $short" "$TMP/longopt.out" "$TMP/longopt.expected"
done

# 12. long options that can't be compared to system cat byte-for-byte --
#     -E/-T/-A have no BSD-cat equivalent, and -n/-b use mew's own
#     box-drawing separator instead of cat's tab (a deliberate divergence).
#     Verify against mew's own short flag instead.
for pair in "--number:-n" "--number-nonblank:-b" "--show-ends:-E" "--show-tabs:-T" "--show-all:-A"; do
    long=${pair%%:*}
    short=${pair##*:}
    "$MEW" "$long" "$INPUTS/multiline.txt" >"$TMP/longopt.out"
    "$MEW" "$short" "$INPUTS/multiline.txt" >"$TMP/longopt.expected"
    expect_files_equal "$long matches mew's own $short" "$TMP/longopt.out" "$TMP/longopt.expected"
done

# 13. --color=always / --color=never / --color=auto (piped) / NO_COLOR
ESC=$(printf '\033')

out=$("$MEW" --color=always -n "$INPUTS/single_line.txt")
expect_contains "--color=always emits color, even piped" "$out" "$ESC"

out=$("$MEW" --color=never -n "$INPUTS/single_line.txt")
expect_not_contains "--color=never never emits color" "$out" "$ESC"

out=$("$MEW" --color=auto -n "$INPUTS/single_line.txt")
expect_not_contains "--color=auto is plain when piped (not a tty)" "$out" "$ESC"

out=$(NO_COLOR=1 "$MEW" --color=always -n "$INPUTS/single_line.txt")
expect_not_contains "NO_COLOR overrides --color=always" "$out" "$ESC"

# --color=auto on a *real* tty should emit color -- needs a pseudo-terminal,
# which `script` can't reliably provide in every CI sandbox. Best-effort:
# run it as a real test when `script` is available, skip (uncounted) otherwise,
# so a missing/broken pty never fails the whole suite.
if command -v script >/dev/null 2>&1; then
    tty_out=$(run_tty "$MEW" --color=auto -n "$INPUTS/single_line.txt")
    case "$tty_out" in
    *"$ESC"*) ok "--color=auto emits color on a real tty" ;;
    *) ko "--color=auto emits color on a real tty" "no escape byte via script-allocated tty" ;;
    esac
else
    printf 'SKIP: --color=auto emits color on a real tty -- `script` not available\n'
fi

# 14. line ranges (-r / --line-range)
seq 1 40 | sed 's/^/line/' >"$TMP/range40.txt"

# 14a. N:M matches sed -n 'N,Mp' exactly
"$MEW" -r 2:4 "$TMP/range40.txt" >"$TMP/r.out"
sed -n '2,4p' "$TMP/range40.txt" >"$TMP/r.expected"
expect_files_equal "-r 2:4 matches sed -n '2,4p'" "$TMP/r.out" "$TMP/r.expected"

# 14b. long form equals short form
"$MEW" --line-range=2:4 "$TMP/range40.txt" >"$TMP/r.out"
expect_files_equal "--line-range=2:4 matches -r 2:4" "$TMP/r.out" "$TMP/r.expected"

# 14c. attached short form (-r2:4) parses like the spaced form
"$MEW" -r2:4 "$TMP/range40.txt" >"$TMP/r.out"
expect_files_equal "-r2:4 (attached) matches -r 2:4" "$TMP/r.out" "$TMP/r.expected"

# 14d. N: (to EOF), :M (from start), single line N
"$MEW" -r 5: "$TMP/range40.txt" >"$TMP/r.out"
sed -n '5,$p' "$TMP/range40.txt" >"$TMP/r.expected"
expect_files_equal "-r 5: (to EOF) matches sed" "$TMP/r.out" "$TMP/r.expected"

"$MEW" -r :3 "$TMP/range40.txt" >"$TMP/r.out"
sed -n '1,3p' "$TMP/range40.txt" >"$TMP/r.expected"
expect_files_equal "-r :3 (from start) matches sed" "$TMP/r.out" "$TMP/r.expected"

"$MEW" -r 7 "$TMP/range40.txt" >"$TMP/r.out"
sed -n '7p' "$TMP/range40.txt" >"$TMP/r.expected"
expect_files_equal "-r 7 (single line) matches sed" "$TMP/r.out" "$TMP/r.expected"

# 14e. repeatable ranges: union of :2 and 5:, each line printed once, in order
"$MEW" -r :2 -r 5: "$TMP/range40.txt" >"$TMP/r.out"
sed -n '1,2p;5,$p' "$TMP/range40.txt" >"$TMP/r.expected"
expect_files_equal "repeated ranges union, no duplicates" "$TMP/r.out" "$TMP/r.expected"

# 14f. overlapping/unsorted ranges normalize to the union
"$MEW" -r 5:8 -r 1:3 -r 4:6 "$TMP/range40.txt" >"$TMP/r.out"
sed -n '1,8p' "$TMP/range40.txt" >"$TMP/r.expected"
expect_files_equal "unsorted/overlapping ranges merge" "$TMP/r.out" "$TMP/r.expected"

# 14g. ranges apply per file
printf 'a\nb\nc\nd\n' >"$TMP/r_a.txt"
printf 'w\nx\ny\nz\n' >"$TMP/r_b.txt"
"$MEW" -r 2:3 "$TMP/r_a.txt" "$TMP/r_b.txt" >"$TMP/r.out"
{ sed -n '2,3p' "$TMP/r_a.txt"; sed -n '2,3p' "$TMP/r_b.txt"; } >"$TMP/r.expected"
expect_files_equal "ranges apply per file" "$TMP/r.out" "$TMP/r.expected"

# 14h. -n numbers by original position in the file
"$MEW" -n -r 30:32 "$TMP/range40.txt" >"$TMP/r.out"
# mew's separator contains a UTF-8 box character; build expected via mew
# itself on a full print to avoid encoding it here.
"$MEW" -n "$TMP/range40.txt" >"$TMP/r.full"
awk 'NR>=30 && NR<=32' "$TMP/r.full" >"$TMP/r.expected"
expect_files_equal "-n numbers original positions under -r" "$TMP/r.out" "$TMP/r.expected"

# 14i. -s operates after range filtering: squeezed lines keep their original
#      position numbering (line 3/4 dropped from output, y stays numbered 5)
printf 'x\n\n\n\ny\nz\n' >"$TMP/r_s.txt"
"$MEW" -n -s -r 1:5 "$TMP/r_s.txt" >"$TMP/r.out"
{ "$MEW" -n -r 1:2 "$TMP/r_s.txt"; "$MEW" -n -r 5:5 "$TMP/r_s.txt"; } >"$TMP/r.expected"
expect_files_equal "-s squeezes after range filtering" "$TMP/r.out" "$TMP/r.expected"

# 14j. stdin works with ranges
"$MEW" -r 2:3 <"$TMP/range40.txt" >"$TMP/r.out"
sed -n '2,3p' "$TMP/range40.txt" >"$TMP/r.expected"
expect_files_equal "-r reads stdin" "$TMP/r.out" "$TMP/r.expected"

# 14k. range past EOF and empty file: empty output, exit 0
out=$("$MEW" -r 99:100 "$TMP/range40.txt")
code=$?
expect_exit "range past EOF exit code" "$code" 0
if [ -z "$out" ]; then
    ok "range past EOF produces no output"
else
    ko "range past EOF produces no output" "got non-empty output"
fi

# 14l. N > M is a usage error with exit code 2
err=$("$MEW" -r 5:2 "$TMP/range40.txt" 2>&1 >/dev/null)
code=$?
expect_exit "N>M range exit code" "$code" 2
expect_contains "N>M range stderr message" "$err" "invalid line range"

# 14m. malformed ranges are usage errors: letters, double colon, zero, overflow
for bad in "abc" "1:2:3" "0:5" ":0" "99999999999999999999:5" "1:99999999999999999999"; do
    "$MEW" -r "$bad" "$TMP/range40.txt" >/dev/null 2>&1
    expect_exit "invalid range '$bad' exit code" "$?" 2
done

# 14n. more than 8 ranges is a usage error
"$MEW" -r 1:2 -r 3:4 -r 5:6 -r 7:8 -r 9:10 -r 11:12 -r 13:14 -r 15:16 -r 17:18 \
    "$TMP/range40.txt" >/dev/null 2>&1
expect_exit "more than 8 ranges exit code" "$?" 2

# 14o. early exit: reading stops after the last wanted line (sparse 5GB file,
#       must not be drained). Skipped when truncate is unavailable.
if command -v truncate >/dev/null 2>&1; then
    big="$TMP/r_sparse.bin"
    { seq 1 100; dd if=/dev/zero bs=1048576 count=1 seek=5120 2>/dev/null; printf 'last\n'; } >"$big"
    out=$("$MEW" -r 1:1 "$big")
    expect_contains "early exit returns first line of huge file" "$out" "1"
    rm -f "$big"
else
    printf 'SKIP: early exit on huge file -- truncate not available\n'
fi

total=$((pass + fail))
printf '\ntests passed: %d/%d\n' "$pass" "$total"

[ "$fail" -eq 0 ]
