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

total=$((pass + fail))
printf '\ntests passed: %d/%d\n' "$pass" "$total"

[ "$fail" -eq 0 ]
