#!/usr/bin/env bash
# A/B rounds of a bench on one device. See tools/perf/README.md for the protocol.
#
# usage: tools/perf/run.sh --serial SERIAL --out DIR --labels "NAME=SPEC ..." [options]
#
#   label SPEC   VARIANT           a variant of --apk (needs a --variants build)
#                APK               a separate APK
#                APK:VARIANT       a variant of a separate APK
#   --apk FILE       APK for labels that give only a variant
#   --rounds N       rounds to run (default 5); --first-round K numbers them from K (default 1)
#   --bench NAME     bench switched on by debug.ns.NAME (default jnibench)
#   --filter F       rows to run; '|' separates names (default: every row)
#   --groups FILE    one filter per line instead; each line is a separate launch per label
#   --scale N        debug.ns.<bench>.scale
#   --lock PATH      device lock directory (default: $NS_PERF_LOCK, else
#                    ${NS_PERF_LOCK_DIR:-~/.cache/ns-perf/locks}/phone-<serial>.lock)
#   --no-wait        fail instead of waiting when the lock is busy
#   --gap SECS       pause between rounds, lock released (default 30)
#   --discard-secs N how long the discarded first launch after an install runs (default 8)
#   --no-discard     measure the first launch after an install too (first-launch check on a
#                    new device); launches.tsv marks it with first=1
#   --timeout SECS   per measured launch (default 300)
#
# Writes DIR/<label>_r<round>.g<group>.log per launch, DIR/launches.tsv and DIR/rounds.tsv.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/lib/common.sh"

SERIAL= OUT= LABELS= APK= ROUNDS=5 FIRST=1 BENCH=jnibench FILTER= GROUPS_FILE= SCALE=
LOCK=${NS_PERF_LOCK:-} NO_WAIT=0 GAP=30 DISCARD_SECS=8 NO_DISCARD=0 LAUNCH_TIMEOUT=300
while [ $# -gt 0 ]; do
  case $1 in
    --serial) SERIAL=$2; shift 2 ;;
    --out) OUT=$2; shift 2 ;;
    --labels) LABELS=$2; shift 2 ;;
    --apk) APK=$2; shift 2 ;;
    --rounds) ROUNDS=$2; shift 2 ;;
    --first-round) FIRST=$2; shift 2 ;;
    --bench) BENCH=$2; shift 2 ;;
    --filter) FILTER=$2; shift 2 ;;
    --groups) GROUPS_FILE=$2; shift 2 ;;
    --scale) SCALE=$2; shift 2 ;;
    --lock) LOCK=$2; shift 2 ;;
    --no-wait) NO_WAIT=1; shift ;;
    --gap) GAP=$2; shift 2 ;;
    --discard-secs) DISCARD_SECS=$2; shift 2 ;;
    --no-discard) NO_DISCARD=1; shift ;;
    --timeout) LAUNCH_TIMEOUT=$2; shift 2 ;;
    -h|--help) sed -n '2,27p' "$0"; exit 0 ;;
    *) die "unknown option $1" ;;
  esac
done
[ -n "$OUT" ] || die "--out is required"
[ -n "$LABELS" ] || die "--labels is required"
require_serial
[ -n "$LOCK" ] || LOCK=$(default_lock_path)
PERF_CMD="run $OUT"

# ---- labels -----------------------------------------------------------------
NAMES=() L_APK=() L_VAR=()
for spec in $LABELS; do
  name=${spec%%=*}; rest=${spec#*=}
  [ "$name" != "$spec" ] && [ -n "$rest" ] || die "label '$spec' is not NAME=SPEC"
  [[ $name =~ ^[A-Za-z0-9.+-]+$ ]] || die "label name '$name' may only use letters, digits, '.', '+' and '-'"
  if [[ $rest =~ ^-?[0-9]+$ ]]; then
    [ -n "$APK" ] || die "label $name gives only a variant; pass --apk"
    apk=$APK; var=$rest
  elif [[ $rest =~ ^(.+):(-?[0-9]+)$ ]]; then
    apk=${BASH_REMATCH[1]}; var=${BASH_REMATCH[2]}
  else
    apk=$rest; var=
  fi
  [ -f "$apk" ] || die "label $name: no APK at $apk"
  NAMES+=("$name"); L_APK+=("$(cd "$(dirname "$apk")" && pwd)/$(basename "$apk")"); L_VAR+=("$var")
done
K=${#NAMES[@]}
[ "$K" -ge 2 ] || die "an A/B needs at least two labels (include an A/A copy of the base)"

GROUPS_LIST=()
if [ -n "$GROUPS_FILE" ]; then
  while IFS= read -r line || [ -n "$line" ]; do
    case $line in ''|'#'*) continue ;; esac
    GROUPS_LIST+=("$line")
  done < "$GROUPS_FILE"
  [ "${#GROUPS_LIST[@]}" -gt 0 ] || die "$GROUPS_FILE has no groups"
else
  GROUPS_LIST=("$FILTER")
fi

mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)
# One experiment, one device: appending rounds from another device would mix them.
if [ -f "$OUT/device.txt" ]; then
  prev=$(head -1 "$OUT/device.txt" | cut -d' ' -f1)
  [ "$prev" = "$SERIAL" ] || die "$OUT already holds rounds from $prev; use a new --out for $SERIAL"
else
  device_desc > "$OUT/device.txt"; printf '\n' >> "$OUT/device.txt"
fi
[ -f "$OUT/launches.tsv" ] || printf 'round\tlabel\tgroup\tvariant\tstatus\tsecs\trows\tthermal\thost_load\tfirst\tapk\n' > "$OUT/launches.tsv"
[ -f "$OUT/rounds.tsv" ] || printf 'round\tlock_wait_secs\tround_secs\tinstalls\tlaunches\n' > "$OUT/rounds.tsv"
{
  printf '%s run.sh rounds %s..%s bench=%s scale=%s lock=%s\n' "$(date '+%F %T')" "$FIRST" "$((FIRST + ROUNDS - 1))" "$BENCH" "${SCALE:-1}" "$LOCK"
  for i in "${!NAMES[@]}"; do
    printf '  %s apk=%s sha=%s variant=%s\n' "${NAMES[$i]}" "${L_APK[$i]}" "$(shasum "${L_APK[$i]}" | cut -c1-12)" "${L_VAR[$i]:-none}"
  done
  for g in "${!GROUPS_LIST[@]}"; do printf '  g%s filter=%s\n' "$g" "${GROUPS_LIST[$g]}"; done
} >> "$OUT/run.txt"

TAG=$(printf '%s' "$BENCH" | tr '[:lower:]' '[:upper:]')

on_exit() { device_cleanup; lock_release; }
trap on_exit EXIT
trap 'exit 130' INT TERM

set_bench_props() {
  local variant=$1 filter=$2
  setprop_q debug.ns.benchvariant "$variant"
  ash setprop "debug.ns.$BENCH" 1
  setprop_q "debug.ns.$BENCH.filter" "$filter"
  setprop_q "debug.ns.$BENCH.scale" "${SCALE:-}"
  local got
  got=$(ash getprop "debug.ns.$BENCH.filter" | tr -d '\r')
  [ "$got" = "$filter" ] || die "filter stored as '$got', wanted '$filter'"
}

install_and_discard() {
  local apk=$1 variant=$2
  A install -r "$apk" >/dev/null || die "install failed: $apk"
  ash cmd package compile -m speed -f "$PKG" >/dev/null
  [ "$NO_DISCARD" = 1 ] && return 0
  # The first launch after an install can run much slower than later ones on some
  # devices (README: first-launch check), so it is never measured.
  set_bench_props "$variant" "${GROUPS_LIST[0]}"
  ash am start -n "$PKG/$ACTIVITY" >/dev/null
  sleep "$DISCARD_SECS"
  ash am force-stop "$PKG" >/dev/null
  sleep 1
}

for r in $(seq "$FIRST" $((FIRST + ROUNDS - 1))); do
  tw=$SECONDS
  lock_acquire
  t0=$SECONDS; installed=; installs=0; launches=0; first=0
  log "round $r on $(device_desc)"
  for j in $(seq 0 $((K - 1))); do
    i=$(( (j + r - 1) % K ))
    name=${NAMES[$i]}; apk=${L_APK[$i]}; var=${L_VAR[$i]}
    if [ "$apk" != "$installed" ]; then
      install_and_discard "$apk" "$var"
      installed=$apk; installs=$((installs + 1)); first=$NO_DISCARD
    fi
    for g in "${!GROUPS_LIST[@]}"; do
      set_bench_props "$var" "${GROUPS_LIST[$g]}"
      th=$(thermal_status); ld=$(host_load)
      f=$OUT/${name}_r$r.g$g.log
      read -r status secs < <(bench_launch "$f")
      launches=$((launches + 1))
      rows=$(grep -c "$TAG {" "$f" || true)
      got=$(launch_variant "$f")
      if [ -n "$var" ] && [ "$got" = none ]; then
        die "$name: the runtime did not report a variant; was $(basename "$apk") built with --variants?"
      fi
      if [ -n "$var" ] && [ "$got" != "$var" ]; then
        mv "$f" "$f.bad"; status="wrong-variant($got)"
      elif [ "$status" != done ]; then
        mv "$f" "$f.bad"
      fi
      printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$r" "$name" "$g" "${var:-none}" "$status" "$secs" "$rows" "$th" "$ld" "$first" "$(basename "$apk")" >> "$OUT/launches.tsv"
      log "r$r $name g$g variant=${var:-none} $status ${secs}s rows=$rows thermal=$th host_load=$ld$([ "$first" = 1 ] && printf " first-after-install")"
      first=0
    done
  done
  device_cleanup
  lock_release
  printf '%s\t%s\t%s\t%s\t%s\n' "$r" "$((t0 - tw))" "$((SECONDS - t0))" "$installs" "$launches" >> "$OUT/rounds.tsv"
  log "round $r done in $((SECONDS - t0))s ($installs install(s), $launches launches)"
  [ "$r" -lt $((FIRST + ROUNDS - 1)) ] && sleep "$GAP"
done
log "all rounds done; tools/perf/table.py $OUT --anchor ${NAMES[0]}"
