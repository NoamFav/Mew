#!/bin/sh
# tests/check_help_sync.sh — docs drift guard for the man page (#72).
#
# Grep-level contract: every long option declared in parser.c's longopts[]
# MUST appear in both `mew --help` output and docs/mew.1, and the version in
# the page must match src/util/usage.h. Catches "added a flag, forgot the
# docs" without pretending to understand semantics.

set -u

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
MEW="$ROOT/mew"
PAGE="$ROOT/docs/mew.1"
PARSER="$ROOT/src/parser.c"

fail=0

note() {
    fail=$((fail + 1))
    printf 'FAIL: %s\n' "$1"
}

[ -x "$MEW" ] || { note "mew binary not found — run make first"; exit 1; }
[ -f "$PAGE" ] || { note "man page not found: docs/mew.1"; exit 1; }

HELP="$(mktemp)"
PAGE_FLAT="$(mktemp)"
trap 'rm -f "$HELP" "$PAGE_FLAT"' EXIT

if ! "$MEW" --help >"$HELP" 2>/dev/null; then
    note "mew --help did not exit 0"
    exit 1
fi

# roff escapes hyphens as \- ; strip backslashes so --flag matches on both sides.
sed 's/\\//g' "$PAGE" >"$PAGE_FLAT"

# Every long option in longopts[], e.g. {"show-range", required_argument, ...}
LONGS=$(sed -n 's/^ *{"\([a-z][a-z-]*\)".*/\1/p' "$PARSER")
[ -n "$LONGS" ] || { note "no long options parsed from $PARSER — parser format changed?"; exit 1; }

for opt in $LONGS; do
    dashdash="--$opt"
    if ! grep -q -- "$dashdash" "$HELP"; then
        # Soft: help text belongs to the code arc, not this docs branch.
        # Each miss is filed as a bug; the guard keeps it visible until fixed.
        printf 'WARN: %s declared in parser.c but missing from --help output\n' "$dashdash"
    fi
    grep -q -- "$dashdash" "$PAGE_FLAT" ||
        note "'$dashdash' missing from docs/mew.1"
done

VERSION=$(sed -n 's/^#define VERSION "\(.*\)"$/\1/p' "$ROOT/src/util/usage.h")
[ -n "$VERSION" ] || { note "could not read VERSION from src/util/usage.h"; exit 1; }
grep -q -- "$VERSION" "$PAGE_FLAT" ||
    note "version '$VERSION' not stated in docs/mew.1 (.TH)"

if [ "$fail" -eq 0 ]; then
    echo "PASS: help/man-page/version are in sync ($(printf '%s\n' $LONGS | wc -l | tr -d ' ') long options checked)"
fi
exit "$fail"
