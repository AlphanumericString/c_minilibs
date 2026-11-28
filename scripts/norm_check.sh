#!/bin/sh
# Check that every C/H file is clang-format compliant.
#
# usage: norm_check.sh <clang-format> [files...]
#
# Paths that no longer exist (deleted or renamed files) are ignored instead of
# making the rule error out, and files that were never added to git are checked
# too. Non-compliant files are rewritten in place so the caller can commit the
# result; their paths are printed on stdout and the script exits non-zero.

set -eu

clang_format=$1
shift

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT INT TERM

bad=
for file in "$@"; do
	[ -f "$file" ] || continue
	"$clang_format" "$file" | sed 's/^ \*\*\//**\//' >"$tmpdir/formatted"
	if ! cmp -s "$tmpdir/formatted" "$file"; then
		bad="$bad$file
"
		cat "$tmpdir/formatted" >"$file"
	fi
done

if [ -n "$bad" ]; then
	printf '%s' "$bad"
	exit 1
fi
