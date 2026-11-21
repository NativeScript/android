#!/usr/bin/env bash
# Runs an instrumented test app (build.sh --generate) on a device or emulator,
# collects its raw profiles and merges them into outdir/<abi>.profdata.
#
# usage: tools/pgo/train.sh [-s serial] [-a abi] -o outdir [--no-bench] instrumented.apk
#
#   -s serial     adb serial (default: ANDROID_SERIAL, or the only device).
#   -a abi        ABI the APK was built for (default arm64-v8a); the device must run it.
#   -o outdir     receives ns-<run>.profraw, logcat-<run>.txt and <abi>.profdata.
#   --no-bench    train on the test suite only, without the JNI benchmarks.
#
# Each run is a fresh launch that writes its counters through __pgoDump() when
# it finishes: the JNI benchmarks (debug.ns.jnibench=1) and the full Jasmine
# suite. A release-mode suite run reports some failures (release builds refuse
# remote module loading); they do not affect what the profile covers.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

SERIAL=${ANDROID_SERIAL:-}
ABI=arm64-v8a
OUT=""
BENCH=1
while [ $# -gt 0 ]; do
  case $1 in
    -s) SERIAL=$2; shift 2 ;;
    -a) ABI=$2; shift 2 ;;
    -o) OUT=$2; shift 2 ;;
    --no-bench) BENCH=0; shift ;;
    -*) die "unknown argument: $1" ;;
    *) break ;;
  esac
done
[ $# -eq 1 ] && [ -n "$OUT" ] || die "usage: train.sh [-s serial] [-a abi] -o outdir [--no-bench] instrumented.apk"
APK=$1
[ -f "$APK" ] || die "APK not found: $APK"
unzip -p "$APK" "lib/$ABI/libNativeScript.so" | grep -a -F __llvm_prf_cnts >/dev/null ||
  die "$APK has no instrumented lib/$ABI/libNativeScript.so; build it with build.sh --generate -a $ABI"
mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)

require_device
require_abi_on_device "$ABI"
EXT=/sdcard/Android/data/$PKG/files
trap reset_bench_props EXIT

install_apk "$APK"
adbs shell "rm -f $EXT/ns-*.profraw" || true

# $1 = debug.ns.jnibench value, $2 = run name, $3 = timeout in seconds
run() {
  local bench=$1 name=$2 timeout=$3 waited=0
  adbs shell am force-stop "$PKG"
  set_bench_props "$bench" ""
  adbs logcat -c
  adbs shell am start -n "$ACTIVITY" >/dev/null
  # grep without -q reads all of its input: an early exit would SIGPIPE adb,
  # and pipefail would turn the match into a failure.
  until adbs logcat -d | grep -F "PGO_DUMP $name" >/dev/null; do
    sleep 5
    waited=$((waited + 5))
    [ "$waited" -le "$timeout" ] || { adbs logcat -d >"$OUT/logcat-$name.txt"; die "timed out waiting for the $name run; see $OUT/logcat-$name.txt"; }
  done
  adbs logcat -d >"$OUT/logcat-$name.txt"
  adbs shell am force-stop "$PKG"
  grep -E "PGO_DUMP|Runtime Tests: .*specs" "$OUT/logcat-$name.txt" | tail -2 || true
  grep -q "PGO_DUMP $name rc=0" "$OUT/logcat-$name.txt" || die "__pgoDump failed for $name; is the APK instrumented?"
  adbs pull "$EXT/ns-$name.profraw" "$OUT/ns-$name.profraw" >/dev/null
}

RAW=()
if [ "$BENCH" = 1 ]; then
  run 1 bench 900
  RAW+=("$OUT/ns-bench.profraw")
fi
run 0 suite 1800
RAW+=("$OUT/ns-suite.profraw")

"$PGO_DIR/merge.sh" "$OUT/$ABI.profdata" "${RAW[@]}"
echo "profile: $OUT/$ABI.profdata"
