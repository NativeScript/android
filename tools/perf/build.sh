#!/usr/bin/env bash
# Builds the release (optimized, arm64) test-app APK used for benchmarks.
#
# usage: tools/perf/build.sh --out DIR [--label NAME] [--variants] [--ccache]
#                            [--profileable] [--regular] [--abi ABI] [--repo DIR] [--keep-build]
#
#   --out DIR       where <label>.apk, <label>.syms/ and <label>.build.txt go
#   --label NAME    APK name (default: bench)
#   --variants      -PbenchVariants: the runtime reads debug.ns.benchvariant (one APK, many labels)
#   --ccache        compile through ccache, with settings that share hits across checkouts
#   --profileable   add <profileable android:shell="true"/> for this build only (stat.sh needs it)
#   --regular       the regular runtime instead of -Poptimized (shipped apps use optimized)
#   --abi ABI       default arm64-v8a
#   --repo DIR      checkout to build (default: the one this script is in)
#   --keep-build    keep test-app/app/build afterwards (deleted by default to save disk)
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib/common.sh"

OUT= LABEL=bench VARIANTS=0 CCACHE=0 PROFILEABLE=0 OPTIMIZED=1 ABI=arm64-v8a KEEP_BUILD=0
REPO=$REPO_ROOT
while [ $# -gt 0 ]; do
  case $1 in
    --out) OUT=$2; shift 2 ;;
    --label) LABEL=$2; shift 2 ;;
    --variants) VARIANTS=1; shift ;;
    --ccache) CCACHE=1; shift ;;
    --profileable) PROFILEABLE=1; shift ;;
    --regular) OPTIMIZED=0; shift ;;
    --abi) ABI=$2; shift 2 ;;
    --repo) REPO=$(cd "$2" && pwd); shift 2 ;;
    --keep-build) KEEP_BUILD=1; shift ;;
    -h|--help) sed -n '2,17p' "$0"; exit 0 ;;
    *) die "unknown option $1" ;;
  esac
done
[ -n "$OUT" ] || die "--out is required"
mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)
TA=$REPO/test-app
[ -x "$TA/gradlew" ] || die "$TA/gradlew not found"
KS=$HOME/.android/debug.keystore
[ -f "$KS" ] || die "$KS is missing (any Android Studio install creates it)"

MANIFEST=$TA/app/src/main/AndroidManifest.xml
restore_manifest() {
  [ "$PROFILEABLE" = 1 ] || return 0
  git -C "$REPO" checkout -q -- "$MANIFEST" && log "restored $(basename "$MANIFEST")"
}
if [ "$PROFILEABLE" = 1 ]; then
  git -C "$REPO" diff --quiet -- "$MANIFEST" || die "$MANIFEST has local changes; --profileable rewrites and restores it"
  grep -q '<profileable' "$MANIFEST" && die "$MANIFEST already declares <profileable>"
  trap restore_manifest EXIT
  python3 -I - "$MANIFEST" <<'PY'
import re, sys
p = sys.argv[1]
s = open(p).read()
s, n = re.subn(r'(<application\b[^>]*>)', r'\1\n        <profileable android:shell="true" />', s, count=1)
assert n == 1, "no <application> element"
open(p, "w").write(s)
PY
fi

ARGS=(:app:assembleRelease "-Pabis=$ABI" -Prelease "-PksPath=$KS" -PksPassword=android
      -Palias=androiddebugkey -Ppassword=android --console=plain)
[ "$OPTIMIZED" = 1 ] && ARGS+=(-Poptimized)
[ "$VARIANTS" = 1 ] && ARGS+=(-PbenchVariants)

CC_BEFORE=
if [ "$CCACHE" = 1 ]; then
  command -v ccache >/dev/null || die "ccache not found on PATH"
  ARGS+=(-PuseCCache)
  # A dedicated cache: a shared default one is evicted by unrelated projects.
  export CCACHE_DIR=${NS_PERF_CCACHE_DIR:-${CCACHE_DIR:-$HOME/Library/Caches/ns-android-ccache}}
  export CCACHE_MAXSIZE=${NS_PERF_CCACHE_MAXSIZE:-${CCACHE_MAXSIZE:-3G}}
  # CMake passes absolute source and -I paths, and -g makes ccache hash the working
  # directory. Rewriting paths under the checkout and not hashing the directory lets
  # two checkouts of the same sources share entries.
  export CCACHE_BASEDIR=$REPO
  export CCACHE_NOHASHDIR=1
  mkdir -p "$CCACHE_DIR"
  CC_BEFORE=$(ccache --print-stats 2>/dev/null || true)
fi

# buildMetadata only registers its javac input when assets/metadata is absent at
# configuration time; a stale copy makes new Java classes come out undefined in JS.
rm -rf "$TA/app/build" "$TA/app/src/main/assets/metadata"

BUILD_LOG=$OUT/$LABEL.build.log
log "building $LABEL from $REPO ($(git -C "$REPO" rev-parse --short HEAD)) -> $OUT/$LABEL.apk"
T0=$SECONDS
if ! (cd "$TA" && nice -n 19 ./gradlew "${ARGS[@]}" > "$BUILD_LOG" 2>&1); then
  grep -E 'error:|FAILED|What went wrong' -A3 "$BUILD_LOG" | head -40 >&2
  die "build failed, log: $BUILD_LOG"
fi
WALL=$((SECONDS - T0))

APK=$TA/app/build/outputs/apk/release/app-release.apk
[ -f "$APK" ] || die "no APK at $APK"
cp "$APK" "$OUT/$LABEL.apk"
CXX_CFG=RelWithDebInfo
SO=$(ls -t "$TA"/runtime/build/intermediates/cxx/$CXX_CFG/*/obj/"$ABI"/libNativeScript.so 2>/dev/null | head -1 || true)
if [ -n "$SO" ]; then
  mkdir -p "$OUT/$LABEL.syms"
  cp "$SO" "$OUT/$LABEL.syms/libNativeScript.so"
fi

CC_LINE=off
if [ "$CCACHE" = 1 ]; then
  CC_AFTER=$(ccache --print-stats 2>/dev/null || true)
  CC_LINE=$(python3 -I - "$CC_BEFORE" "$CC_AFTER" <<'PY'
import sys
def parse(t):
    d = {}
    for line in t.splitlines():
        k, _, v = line.partition("\t")
        if v.strip().isdigit(): d[k] = int(v)
    return d
a, b = parse(sys.argv[1]), parse(sys.argv[2])
def delta(*keys): return sum(b.get(k, 0) - a.get(k, 0) for k in keys)
hits = delta("direct_cache_hit", "preprocessed_cache_hit")
miss = delta("cache_miss")
total = hits + miss
print("hits=%d misses=%d hit_rate=%s" % (hits, miss, ("%.0f%%" % (100.0 * hits / total)) if total else "n/a"))
PY
)
fi

LIBSHA=$(unzip -p "$OUT/$LABEL.apk" "lib/$ABI/libNativeScript.so" | shasum | cut -c1-12)
DIRTY=$(git -C "$REPO" status --porcelain --untracked-files=no | wc -l | tr -d ' ')
{
  printf 'label=%s\n' "$LABEL"
  printf 'repo=%s\n' "$REPO"
  printf 'commit=%s\n' "$(git -C "$REPO" rev-parse HEAD)"
  printf 'dirty_files=%s\n' "$DIRTY"
  [ "$DIRTY" = 0 ] || printf 'dirty_diff_sha=%s\n' "$(git -C "$REPO" diff HEAD | shasum | cut -c1-12)"
  printf 'flags=%s\n' "${ARGS[*]}"
  printf 'variants=%s profileable=%s optimized=%s abi=%s\n' "$VARIANTS" "$PROFILEABLE" "$OPTIMIZED" "$ABI"
  printf 'libNativeScript_sha=%s\n' "$LIBSHA"
  printf 'wall_secs=%s\n' "$WALL"
  printf 'ccache=%s\n' "$CC_LINE"
  printf 'built=%s\n' "$(date '+%F %T')"
} > "$OUT/$LABEL.build.txt"

[ "$KEEP_BUILD" = 1 ] || rm -rf "$TA/app/build"
log "built $LABEL in ${WALL}s, libNativeScript sha $LIBSHA, ccache: $CC_LINE"
