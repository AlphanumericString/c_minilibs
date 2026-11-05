#!/bin/sh
# Remove a module and every artifact it owns.
#
# usage: remove_module.sh <module_name> [makefile]
#
# Removes:
#   - the implementation directory  ./<module_name>
#   - the associated tests          ./tests/<module_name>_tests
#   - the associated examples       ./examples/<module_name>
#   - the amalgamated single header ./single/<module_name>.h
# and, when a Makefile is given, the module from its MODULES list
# (which also drives the doxygen INPUT via `make documentation`).

set -eu

name=${1:?missing module name}
makefile=${2:-}

rm -rf -- "$name" "tests/${name}_tests" "examples/${name}" "single/${name}.h"

if [ -n "$makefile" ] && [ -f "$makefile" ]; then
	NAME="$name" perl -i -pe '
		if (/^(MODULES[ \t]*:=[ \t]*)(.*)$/) {
			my ($head, $rest) = ($1, $2);
			my %seen;
			my @mods = grep { $_ ne $ENV{NAME} && !$seen{$_}++ } split /\s+/, $rest;
			$_ = "$head@mods\n";
		}
	' "$makefile"
fi
