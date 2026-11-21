#!/usr/bin/env bash
# Merges raw profiles from one ABI into the indexed profile that
# build.sh --use consumes.
#
# usage: tools/pgo/merge.sh out.profdata in.profraw [in.profraw ...]
#
# Uses llvm-profdata from the NDK the runtime is built with (android.ndkVersion
# in test-app/runtime/build.gradle); set LLVM_PROFDATA or PGO_NDK_VERSION to
# override.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

[ $# -ge 2 ] || die "usage: merge.sh out.profdata in.profraw [in.profraw ...]"
OUT=$1
shift
for f in "$@"; do
  [ -s "$f" ] || die "missing or empty raw profile: $f"
done

TOOL=$(llvm_profdata)
"$TOOL" merge -o "$OUT" "$@"
"$TOOL" show "$OUT"
