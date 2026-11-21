#!/usr/bin/env bash
# Interleaved A/B (or A/B/C...) comparison of JNI benchmark runs.
#
# usage: tools/pgo/ab.sh [-s serial] [-f filter] -o outdir [--from N] --to M label=app.apk [label=app.apk ...]
#
# For each round N..M and each label in turn: reinstall and AOT-compile the
# APK, launch once and keep that run only as outdir/first-<label>.<round>.txt,
# then relaunch and keep the measured run as outdir/<label>.<round>.txt.
# Rounds can be split across invocations (--from 3 --to 4) to fit the device
# around other work. Compare with summarize.py outdir labelA labelB.
#
# Pass the same APK under two labels (base=a.apk base2=a.apk) for an A/A run:
# the spread it shows is the noise floor for every other comparison.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

SERIAL=${ANDROID_SERIAL:-}
FILTER=""
OUT=""
FROM=1
TO=""
while [ $# -gt 0 ]; do
  case $1 in
    -s) SERIAL=$2; shift 2 ;;
    -f) FILTER=$2; shift 2 ;;
    -o) OUT=$2; shift 2 ;;
    --from) FROM=$2; shift 2 ;;
    --to) TO=$2; shift 2 ;;
    -*) die "unknown argument: $1" ;;
    *) break ;;
  esac
done
[ -n "$OUT" ] && [ -n "$TO" ] && [ $# -ge 1 ] || die "usage: ab.sh [-s serial] [-f filter] -o outdir [--from N] --to M label=app.apk ..."
for spec in "$@"; do
  case $spec in
    *=*) [ -f "${spec#*=}" ] || die "APK not found: ${spec#*=}" ;;
    *) die "expected label=app.apk, got: $spec" ;;
  esac
done
mkdir -p "$OUT"
DEVICE_ARGS=()
if [ -n "$SERIAL" ]; then
  DEVICE_ARGS=(-s "$SERIAL")
fi

for round in $(seq "$FROM" "$TO"); do
  for spec in "$@"; do
    label=${spec%%=*}
    apk=${spec#*=}
    "$PGO_DIR/bench.sh" "${DEVICE_ARGS[@]}" -f "$FILTER" "$apk" "$OUT/first-$label.$round.txt" >/dev/null
    "$PGO_DIR/bench.sh" "${DEVICE_ARGS[@]}" -f "$FILTER" --no-install "$apk" "$OUT/$label.$round.txt" >/dev/null
    echo "round $round $label: $(tail -1 "$OUT/$label.$round.txt")"
  done
done
