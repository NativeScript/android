#!/usr/bin/env bash
# One launch of the JNI benchmarks (debug.ns.jnibench) with its JNIBENCH rows
# saved to a file, followed by a line with the device's thermal and cpufreq
# state before, during and after the run.
#
# usage: tools/pgo/bench.sh [-s serial] [-f filter] [--no-install] app.apk outfile
#
#   -f filter      only run benchmarks whose name contains filter.
#   --no-install   relaunch the installed app instead of reinstalling it.
#
# The first launch after an install is not representative (it ran ~1.6x slower
# on one test phone), so compare relaunches with relaunches: ab.sh discards
# the launch that follows each install.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

SERIAL=${ANDROID_SERIAL:-}
FILTER=""
INSTALL=1
while [ $# -gt 0 ]; do
  case $1 in
    -s) SERIAL=$2; shift 2 ;;
    -f) FILTER=$2; shift 2 ;;
    --no-install) INSTALL=0; shift ;;
    -*) die "unknown argument: $1" ;;
    *) break ;;
  esac
done
[ $# -eq 2 ] || die "usage: bench.sh [-s serial] [-f filter] [--no-install] app.apk outfile"
APK=$1
OUT=$2

require_device
trap reset_bench_props EXIT

state() {
  adbs shell 'dumpsys thermalservice 2>/dev/null | grep -m1 "Thermal Status"; echo maxfreq $(cat /sys/devices/system/cpu/cpufreq/policy*/scaling_max_freq 2>/dev/null)' | tr '\r\n' '  '
}

if [ "$INSTALL" = 1 ]; then
  install_apk "$APK"
fi
adbs shell am force-stop "$PKG"
set_bench_props 1 "$FILTER"
adbs logcat -c
BEFORE=$(state)
adbs shell am start -n "$ACTIVITY" >/dev/null
waited=0
MID=""
until adbs logcat -d -s JS:I | grep -F JNIBENCH_DONE >/dev/null; do
  sleep 3
  waited=$((waited + 3))
  [ "$waited" = 15 ] && MID=$(state)
  [ "$waited" -le 900 ] || { echo "TIMEOUT" >&2; break; }
done
AFTER=$(state)
adbs logcat -d -s JS:I | grep -F 'JNIBENCH ' >"$OUT" || true
echo "STATE before: $BEFORE | mid: $MID | after: $AFTER | secs=$waited" >>"$OUT"
adbs shell am force-stop "$PKG"
echo "$(grep -c 'JNIBENCH ' "$OUT" || true) rows -> $OUT"
