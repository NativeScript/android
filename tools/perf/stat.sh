#!/usr/bin/env bash
# Two-scale hardware-counter runs: simpleperf stat on the app's JS thread while one row
# runs at two scales. The difference between the scales cancels startup and every other
# fixed cost, so statsum.py can report counts per operation of the row.
#
# usage: tools/perf/stat.sh --serial SERIAL --out DIR --labels "NAME=SPEC ..." --rows FILE
#                           [--apk FILE] [--repeat N] [--events LIST] [--bench NAME]
#                           [--lock PATH] [--no-wait] [--gap SECS] [--discard-secs N]
#
#   labels      as in run.sh (NAME=VARIANT with --apk, NAME=APK, NAME=APK:VARIANT)
#   --rows FILE one row per line: "<key> <scale1> <scale2> <filter>"; the filter is the rest
#               of the line and should select a single bench row
#   --repeat N  passes over every label (default 2), each under its own lock hold, with the
#               label order rotated
#   --events    default instructions:u,cpu-cycles:u; keep it to counters the CPU can count
#               at once (statsum.py flags multiplexed counts)
#
# The APK must be profileable (build.sh --profileable), which needs Android 10 (API 29) or
# later. Writes DIR/<label>.<key>.<scale>.p<pass>.{txt,bench}.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib/common.sh"

SERIAL= OUT= LABELS= APK= ROWS_FILE= REPEAT=2 EVENTS=instructions:u,cpu-cycles:u BENCH=jnibench
LOCK=${NS_PERF_LOCK:-} NO_WAIT=0 GAP=30 DISCARD_SECS=8 LAUNCH_TIMEOUT=600
while [ $# -gt 0 ]; do
  case $1 in
    --serial) SERIAL=$2; shift 2 ;;
    --out) OUT=$2; shift 2 ;;
    --labels) LABELS=$2; shift 2 ;;
    --apk) APK=$2; shift 2 ;;
    --rows) ROWS_FILE=$2; shift 2 ;;
    --repeat) REPEAT=$2; shift 2 ;;
    --events) EVENTS=$2; shift 2 ;;
    --bench) BENCH=$2; shift 2 ;;
    --lock) LOCK=$2; shift 2 ;;
    --no-wait) NO_WAIT=1; shift ;;
    --gap) GAP=$2; shift 2 ;;
    --discard-secs) DISCARD_SECS=$2; shift 2 ;;
    -h|--help) sed -n '2,22p' "$0"; exit 0 ;;
    *) die "unknown option $1" ;;
  esac
done
[ -n "$OUT" ] && [ -n "$LABELS" ] && [ -n "$ROWS_FILE" ] || die "--out, --labels and --rows are required"
require_serial
[ -n "$LOCK" ] || LOCK=$(default_lock_path)
PERF_CMD="stat $OUT"
sdk=$(ash getprop ro.build.version.sdk | tr -d '\r')
[ "${sdk:-0}" -ge 29 ] || die "$SERIAL runs API $sdk; profileable apps (and so simpleperf --app on a release APK) need API 29+"
ash command -v simpleperf >/dev/null 2>&1 || die "no simpleperf on $SERIAL (push the NDK's simpleperf/bin/android/<arch>/simpleperf to /data/local/tmp and put it on PATH)"

NAMES=() L_APK=() L_VAR=()
for spec in $LABELS; do
  name=${spec%%=*}; rest=${spec#*=}
  [[ $name =~ ^[A-Za-z0-9.+-]+$ ]] || die "bad label name '$name'"
  if [[ $rest =~ ^-?[0-9]+$ ]]; then
    [ -n "$APK" ] || die "label $name gives only a variant; pass --apk"
    apk=$APK; var=$rest
  elif [[ $rest =~ ^(.+):(-?[0-9]+)$ ]]; then apk=${BASH_REMATCH[1]}; var=${BASH_REMATCH[2]}
  else apk=$rest; var=; fi
  [ -f "$apk" ] || die "label $name: no APK at $apk"
  NAMES+=("$name"); L_APK+=("$apk"); L_VAR+=("$var")
done
K=${#NAMES[@]}

R_KEY=() R_S1=() R_S2=() R_FILTER=()
while read -r key s1 s2 filter || [ -n "${key:-}" ]; do
  case ${key:-} in ''|'#'*) continue ;; esac
  [ -n "$filter" ] || die "row '$key' has no filter"
  R_KEY+=("$key"); R_S1+=("$s1"); R_S2+=("$s2"); R_FILTER+=("$filter")
done < "$ROWS_FILE"
[ "${#R_KEY[@]}" -gt 0 ] || die "$ROWS_FILE has no rows"

mkdir -p "$OUT"
if [ -f "$OUT/device.txt" ]; then
  [ "$(head -1 "$OUT/device.txt" | cut -d' ' -f1)" = "$SERIAL" ] || die "$OUT holds runs from another device"
else
  device_desc > "$OUT/device.txt"; printf '\n' >> "$OUT/device.txt"
fi
TAG=$(printf '%s' "$BENCH" | tr '[:lower:]' '[:upper:]')
SP_OUT=/data/local/tmp/ns-perf-stat.txt

on_exit() {
  if [ "$LOCK_HELD" = 1 ]; then ash pkill -INT simpleperf >/dev/null 2>&1 || true; fi
  device_cleanup; lock_release
}
trap on_exit EXIT
trap 'exit 130' INT TERM

props() {
  setprop_q debug.ns.benchvariant "$1"
  ash setprop "debug.ns.$BENCH" 1
  setprop_q "debug.ns.$BENCH.filter" "$2"
  setprop_q "debug.ns.$BENCH.scale" "$3"
}

for p in $(seq 1 "$REPEAT"); do
  lock_acquire
  installed=
  for j in $(seq 0 $((K - 1))); do
    i=$(( (j + p - 1) % K ))
    name=${NAMES[$i]}; apk=${L_APK[$i]}; var=${L_VAR[$i]}
    if [ "$apk" != "$installed" ]; then
      A install -r "$apk" >/dev/null || die "install failed: $apk"
      ash cmd package compile -m speed -f "$PKG" >/dev/null
      props "$var" "${R_FILTER[0]}" "${R_S1[0]}"
      ash am start -n "$PKG/$ACTIVITY" >/dev/null
      sleep "$DISCARD_SECS"
      ash am force-stop "$PKG" >/dev/null
      sleep 1
      installed=$apk
    fi
    for ri in "${!R_KEY[@]}"; do
      for sc in "${R_S1[$ri]}" "${R_S2[$ri]}"; do
        f=$OUT/$name.${R_KEY[$ri]}.$sc.p$p
        props "$var" "${R_FILTER[$ri]}" "$sc"
        ash rm -f "$SP_OUT"
        A logcat -c
        adb -s "$SERIAL" shell simpleperf stat --app "$PKG" --per-thread -e "$EVENTS" --duration "$LAUNCH_TIMEOUT" -o "$SP_OUT" </dev/null > "$f.sp.log" 2>&1 &
        sp=$!
        sleep 2
        t0=$SECONDS; status=timeout
        ash am start -n "$PKG/$ACTIVITY" >/dev/null
        while [ $((SECONDS - t0)) -lt "$LAUNCH_TIMEOUT" ]; do
          sleep 2
          if A logcat -d -s JS:I | grep -q "${TAG}_DONE"; then status=done; break; fi
        done
        ash am force-stop "$PKG" >/dev/null
        sleep 1
        ash pkill -INT simpleperf >/dev/null 2>&1 || true
        wait "$sp" || true
        A logcat -d | grep -E "$TAG |NS_BENCH_VARIANT=" > "$f.bench" || true
        ash cat "$SP_OUT" > "$f.txt" || true
        got=$(launch_variant "$f.bench")
        if [ -n "$var" ] && [ "$got" != "$var" ]; then
          die "$name: the runtime reported variant '$got', wanted $var"
        fi
        [ "$status" = done ] || { mv "$f.txt" "$f.txt.bad"; }
        log "p$p $name ${R_KEY[$ri]} x$sc $status $((SECONDS - t0))s"
      done
    done
  done
  device_cleanup
  lock_release
  [ "$p" -lt "$REPEAT" ] && sleep "$GAP"
done
log "done; tools/perf/statsum.py $OUT --anchor ${NAMES[0]}"
