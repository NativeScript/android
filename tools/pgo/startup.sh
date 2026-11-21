#!/usr/bin/env bash
# Cold-start time (am start -W TotalTime) of each APK, interleaved per round,
# three launches per APK per round.
#
# usage: tools/pgo/startup.sh [-s serial] -n rounds -o outfile label=app.apk [label=app.apk ...]
#
# Output lines are "<label> <round> <launch> <ms>"; summarize.py startup prints
# the median per label.
# The app boots into the JNI benchmark harness with a filter that matches no
# benchmark, so a launch measures runtime startup without running the suite.
# An APK is reinstalled only when it differs from the one installed last, and
# the launch right after each install is discarded as a warm-up.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

SERIAL=${ANDROID_SERIAL:-}
ROUNDS=""
OUT=""
while [ $# -gt 0 ]; do
  case $1 in
    -s) SERIAL=$2; shift 2 ;;
    -n) ROUNDS=$2; shift 2 ;;
    -o) OUT=$2; shift 2 ;;
    -*) die "unknown argument: $1" ;;
    *) break ;;
  esac
done
[ -n "$ROUNDS" ] && [ -n "$OUT" ] && [ $# -ge 1 ] || die "usage: startup.sh [-s serial] -n rounds -o outfile label=app.apk ..."

require_device
trap reset_bench_props EXIT
: >"$OUT"
set_bench_props 1 zzz_no_such_benchmark
INSTALLED=""

launch_ms() {
  adbs shell am start -W -S -n "$ACTIVITY" | tr -d '\r' | awk '/TotalTime/ {print $2}'
}

for round in $(seq 1 "$ROUNDS"); do
  for spec in "$@"; do
    label=${spec%%=*}
    apk=${spec#*=}
    sum=$(file_hash "$apk")
    if [ "$INSTALLED" != "$sum" ]; then
      install_apk "$apk"
      INSTALLED=$sum
      launch_ms >/dev/null
      sleep 3
    fi
    for launch in 1 2 3; do
      adbs shell am force-stop "$PKG"
      sleep 1
      echo "$label $round $launch $(launch_ms)" >>"$OUT"
      sleep 2
    done
  done
done
adbs shell am force-stop "$PKG"
echo "STATE $(adbs shell 'dumpsys thermalservice 2>/dev/null | grep -m1 "Thermal Status"' | tr -d '\r')" >>"$OUT"
python3 "$PGO_DIR/summarize.py" startup "$OUT"
