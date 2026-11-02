#!/bin/sh
# Amalgamate a module into a single stb-style header.
#
# usage: amalgamate.sh <module_dir> <output_header> <module_name>
#
# Quoted includes of project files (and angle includes that resolve to a
# project file) are inlined recursively, so the resulting header is self
# contained apart from real system headers. The module declaration is always
# available, while the implementation is only exposed when
# C_MINI__<MODULE>_IMPLEMENTATION is defined, mirroring the STB single-file
# convention.
#
# System headers pulled in by the implementation sources are collected and
# emitted once, right after the implementation ifdef, instead of being
# scattered through the generated file.
#
# Only the module sources are emitted: the first source that includes a given
# header pulls it in with whatever internal macro it defines, exactly like a
# normal translation unit. Static symbols declared in a source file are given
# a file unique prefix to avoid collisions once every source shares a single
# translation unit.

set -eu

mod_dir=${1:?missing module dir}
out=${2:?missing output header}
name=${3:?missing module name}

name_upper=$(printf '%s' "$name" | tr '[:lower:]' '[:upper:]')
guard="C_MINI__${name_upper}_SINGLE_H"
impl="C_MINI__${name_upper}_IMPLEMENTATION"

work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
seen="$work/seen"
sysinc="$work/sysinc"
: >"$seen"
: >"$sysinc"

resolve() {
	local base=$1 inc=$2 cand
	for cand in "$base/$inc" "$inc"; do
		if [ -f "$cand" ]; then
			printf '%s\n' "$cand"
			return 0
		fi
	done
	return 1
}

# emit_header <file> <base_dir>
# Print a project header, recursively inlining the project headers it includes.
# System headers are kept in place since the declarations that follow may need
# them.
emit_header() {
	local f=$1 base=${2:-$(dirname "$1")} line inc resolved
	grep -qxF -- "$f" "$seen" 2>/dev/null && return 0
	printf '%s\n' "$f" >>"$seen"
	while IFS= read -r line || [ -n "$line" ]; do
		case $line in
		'#include "'*'"')
			inc=${line#*\"}
			inc=${inc%\"*}
			if resolved=$(resolve "$base" "$inc"); then
				emit_header "$resolved"
			else
				printf '%s\n' "$line"
			fi
			;;
		'#include <'*'>')
			inc=${line#*<}
			inc=${inc%>*}
			if resolved=$(resolve "$base" "$inc"); then
				emit_header "$resolved"
			else
				printf '%s\n' "$line"
			fi
			;;
		*)
			printf '%s\n' "$line"
			;;
		esac
	done <"$f"
}

# pull_in_headers <source_file> <base_dir>
# Emit (once) every project header a source includes, without emitting any of
# the source body. This puts all declarations before the implementation ifdef.
pull_in_headers() {
	local f=$1 base=${2:-$(dirname "$1")} line inc resolved
	while IFS= read -r line || [ -n "$line" ]; do
		case $line in
		'#include "'*'"')
			inc=${line#*\"}
			inc=${inc%\"*}
			if resolved=$(resolve "$base" "$inc"); then
				emit_header "$resolved"
			fi
			;;
		'#include <'*'>')
			inc=${line#*<}
			inc=${inc%>*}
			if resolved=$(resolve "$base" "$inc"); then
				emit_header "$resolved"
			fi
			;;
		esac
	done <"$f"
}

# collect_sys <source_file> <base_dir>
# Gather every system header an implementation source includes. Includes that
# resolve to a project file are skipped since they are already emitted in the
# declaration section.
collect_sys() {
	local f=$1 base=${2:-$(dirname "$1")} line inc
	while IFS= read -r line || [ -n "$line" ]; do
		case $line in
		'#include "'*'"')
			inc=${line#*\"}
			inc=${inc%\"*}
			if ! resolve "$base" "$inc" >/dev/null; then
				printf '%s\n' "$line" >>"$sysinc"
			fi
			;;
		'#include <'*'>')
			inc=${line#*<}
			inc=${inc%>*}
			if ! resolve "$base" "$inc" >/dev/null; then
				printf '%s\n' "$line" >>"$sysinc"
			fi
			;;
		esac
	done <"$f"
}

# emit_body <source_file>
# Print a source without its includes, which are handled separately.
emit_body() {
	local f=$1 line
	while IFS= read -r line || [ -n "$line" ]; do
		case $line in
		'#include "'*'"' | '#include <'*'>')
			;;
		*)
			printf '%s\n' "$line"
			;;
		esac
	done <"$f"
}

# prefix the static symbols declared in a single source file so that two
# sources can be concatenated without colliding.
prefix_statics() {
	local f=$1 stem
	stem=$(basename "$f" .c)
	perl -0777 -pe '
		my @n;
		while (/static\s+(?:[A-Za-z_]\w*\s+|\*\s*)*?\b([A-Za-z_]\w*)\s*(?=[\(\[;=])/g) {
			push @n, $1;
		}
		my %seen; my @u = grep { !$seen{$_}++ && !/^__/ } @n;
		for my $n (@u) { s/\b\Q$n\E\b/'"${stem}__"'$n/g; }
	' "$f"
}

sources=$(find "$mod_dir" -name '*.c' | sort)

# Expose the internal sections of any header that a source would normally
# guard with a per-translation-unit macro, now that everything shares a
# single translation unit.
internals=$(grep -rhoE '#[[:space:]]*define[[:space:]]+C_MINI__[A-Z0-9_]*INTERNAL[A-Z0-9_]*' \
	"$mod_dir" 2>/dev/null | sed -E 's/#[[:space:]]*define[[:space:]]+//' | sort -u)

for c in $sources; do
	collect_sys "$c" "$(dirname "$c")"
done

{
	printf '#ifndef %s\n#define %s\n\n' "$guard" "$guard"
	printf '/* Amalgamated single-file build of the %s module. */\n' "$name"
	printf '/* Define %s before including to get the implementation. */\n\n' "$impl"

	for macro in $internals; do
		printf '#ifndef %s\n#define %s\n#endif\n' "$macro" "$macro"
	done
	printf '\n'

	# Declarations first, so they are available whether or not the
	# implementation is requested.
	for c in $sources; do
		pull_in_headers "$c" "$(dirname "$c")"
	done

	printf '\n#ifdef %s\n\n' "$impl"

	if [ -s "$sysinc" ]; then
		awk '!seen[$0]++' "$sysinc"
		printf '\n'
	fi

	for c in $sources; do
		tmpc="$work/$(printf '%s' "$c" | tr '/.' '__').c"
		prefix_statics "$c" >"$tmpc"
		emit_body "$tmpc"
	done

	printf '\n#endif /* %s */\n\n' "$impl"
	printf '#endif /* %s */\n' "$guard"
} >"$out"
