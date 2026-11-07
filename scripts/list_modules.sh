#!/bin/sh
# Print the module list declared in the Makefile's MODULES variable.
#
# usage: list_modules.sh [makefile]
#
# Single source of truth: the Makefile owns the module list, callers (e.g. the
# CI change detection) read it from here instead of duplicating it.

set -eu

makefile=${1:-Makefile}

value=$(sed -n 's/^MODULES[[:space:]]*:=[[:space:]]*//p' "$makefile" | tr -s '[:space:]' ' ')
printf '%s\n' "${value% }"
