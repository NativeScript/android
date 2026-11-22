# Shared helpers for tools/perf. Sourced by bash scripts only.

PKG=com.tns.testapplication
ACTIVITY=com.tns.NativeScriptActivity
PERF_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
REPO_ROOT="$(cd "$PERF_DIR/../.." && pwd)"

log() { printf '%s %s\n' "$(date +%T)" "$*"; }
die() { printf 'error: %s\n' "$*" >&2; exit 1; }

# ---- device -----------------------------------------------------------------

# Every adb call goes through this: -s pins the device, and </dev/null keeps adb from
# eating the caller's stdin (a heredoc or a while-read loop).
A() { adb -s "$SERIAL" "$@" </dev/null; }
ash() { adb -s "$SERIAL" shell "$@" </dev/null; }

# Devices the scripts refuse to touch. emulator-5554 is the default emulator most
# people keep for other work; NS_PERF_PROTECTED_SERIALS= (empty) allows it.
PROTECTED_SERIALS="${NS_PERF_PROTECTED_SERIALS-emulator-5554}"

require_serial() {
  [ -n "${SERIAL:-}" ] || die "no device: pass --serial (see 'adb devices -l'); a run never picks one for you"
  local s
  for s in $PROTECTED_SERIALS; do
    [ "$SERIAL" != "$s" ] || die "$SERIAL is protected (NS_PERF_PROTECTED_SERIALS); pick another device"
  done
  local state
  state=$(adb -s "$SERIAL" get-state </dev/null 2>/dev/null || true)
  [ "$state" = device ] || die "$SERIAL is not attached and ready (state: '${state:-absent}')"
}

device_desc() {
  printf '%s %s Android %s (API %s)' "$SERIAL" "$(ash getprop ro.product.model | tr -d '\r')" \
    "$(ash getprop ro.build.version.release | tr -d '\r')" "$(ash getprop ro.build.version.sdk | tr -d '\r')"
}

# Property values reach the device shell as one single-quoted word.
setprop_q() {
  local name=$1 value=$2
  case "$value" in *"'"*) die "property value for $name contains a single quote: $value" ;; esac
  [ "${#value}" -le 91 ] || die "property value for $name is longer than 91 bytes: $value"
  ash setprop "$name" "'$value'"
}

# ---- lock -------------------------------------------------------------------
#
# A lock is a directory created with mkdir (atomic). The holder writes an "owner" file
# inside it and removes the file before rmdir. LOCK_HELD is 1 only between a successful
# mkdir and the matching rmdir, and every cleanup path checks it first, so a process that
# is still waiting (or was interrupted while waiting) never stops the app, clears
# properties or removes someone else's lock.

LOCK_HELD=0

default_lock_path() {
  printf '%s/phone-%s.lock' "${NS_PERF_LOCK_DIR:-$HOME/.cache/ns-perf/locks}" "$SERIAL"
}

lock_acquire() {
  local poll=${LOCK_POLL_SECS:-60}
  mkdir -p "$(dirname "$LOCK")"
  until mkdir "$LOCK" 2>/dev/null; do
    if [ "${NO_WAIT:-0}" = 1 ]; then die "lock busy: $LOCK ($(lock_owner))"; fi
    log "lock busy: $LOCK ($(lock_owner)), retrying in ${poll}s"
    sleep "$poll"
  done
  LOCK_HELD=1
  printf 'pid=%s host=%s since=%s cmd=tools/perf %s\n' "$$" "$(hostname -s)" "$(date '+%F %T')" "${PERF_CMD:-}" > "$LOCK/owner" || true
  log "lock acquired: $LOCK"
}

lock_owner() {
  if [ -f "$LOCK/owner" ]; then tr -d '\n' < "$LOCK/owner"; else printf 'owner unknown, created %s' "$(stat -f '%Sm' "$LOCK" 2>/dev/null || stat -c '%y' "$LOCK" 2>/dev/null || echo '?')"; fi
}

lock_release() {
  [ "$LOCK_HELD" = 1 ] || return 0
  rm -f "$LOCK/owner"
  rmdir "$LOCK" 2>/dev/null && log "lock released: $LOCK"
  LOCK_HELD=0
}

# ---- bench properties -------------------------------------------------------

# A bench named X is switched on by debug.ns.X=1, selected with debug.ns.X.filter and
# .scale, logs "<TAG> {json}" result lines and ends with a "<TAG>_DONE" line, where TAG
# is X in upper case (jnibench -> JNIBENCH).
bench_props_clear() {
  local p
  for p in "debug.ns.$BENCH.filter" "debug.ns.$BENCH.scale" "debug.ns.$BENCH.rounds" debug.ns.benchvariant; do
    ash setprop "$p" "''" >/dev/null 2>&1 || true
  done
  ash setprop "debug.ns.$BENCH" 0 >/dev/null 2>&1 || true
}

# Leaves the device as it was found. Only ever called while LOCK_HELD=1.
device_cleanup() {
  [ "$LOCK_HELD" = 1 ] || return 0
  ash am force-stop "$PKG" >/dev/null 2>&1 || true
  bench_props_clear
}

# One launch of the bench. Waits for "<TAG>_DONE", the process dying or the timeout,
# then writes every bench line plus the runtime's variant echo to $1.
# Prints: "<status> <seconds>" where status is done|died|timeout.
bench_launch() {
  local out=$1 timeout=${LAUNCH_TIMEOUT:-300}
  local tag
  tag=$(printf '%s' "$BENCH" | tr '[:lower:]' '[:upper:]')
  A logcat -c
  local t0=$SECONDS status=timeout
  ash am start -n "$PKG/$ACTIVITY" >/dev/null
  while [ $((SECONDS - t0)) -lt "$timeout" ]; do
    sleep 2
    if A logcat -d -s JS:I | grep -q "${tag}_DONE"; then status=done; break; fi
    if ! ash pidof "$PKG" >/dev/null 2>&1; then
      sleep 1
      A logcat -d -s JS:I | grep -q "${tag}_DONE" && status=done || status=died
      break
    fi
  done
  A logcat -d | grep -E "${tag}|NS_BENCH_VARIANT=|FATAL EXCEPTION|TypeError|ReferenceError" > "$out" || true
  ash am force-stop "$PKG" >/dev/null
  printf '%s %s\n' "$status" "$((SECONDS - t0))"
}

# The variant the runtime reported in a launch log, or "none" for a build without
# NS_BENCH_VARIANTS.
launch_variant() {
  local v
  v=$(grep -o 'NS_BENCH_VARIANT=[-0-9]*' "$1" | head -1 | cut -d= -f2)
  printf '%s' "${v:-none}"
}

thermal_status() {
  ash dumpsys thermalservice 2>/dev/null | grep -o 'Thermal Status: [0-9]*' | head -1 | grep -o '[0-9]*$' || printf '?'
}

# Host load matters for emulators, which share the host's CPU, and shows when a build
# ran during a measurement.
host_load() {
  uptime | sed -E 's/.*load averages?: *//' | tr -d ',' | cut -d' ' -f1
}
