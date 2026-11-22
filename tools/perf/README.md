# Runtime performance tooling

Scripts for A/B benchmarks of the Android runtime on a real device: build one benchmark
APK, run labelled rounds under a device lock, and turn the logs into a ratio table.
They use bash and python3 only. Run them with `bash`, even from zsh.

```bash
OUT=/tmp/ns-perf/my-experiment
# 1. one release APK that carries every label (variants 0, 1, 2 below)
bash tools/perf/build.sh --out "$OUT/apks" --label exp --variants --ccache
# 2. rounds on one device: base and its A/A copy base2 both run variant 0
bash tools/perf/run.sh --serial <adb-serial> --out "$OUT/runs" --apk "$OUT/apks/exp.apk" \
  --labels "base=0 base2=0 cache=1 nocache=2" --rounds 6 \
  --filter 'static.noop()|field.get int|callback.noArgs()'
# 3. the table
python3 tools/perf/table.py "$OUT/runs" --anchor base --aa base2
```

## The protocol, and why

Each rule below cost a wrong result at least once.

- **Release, optimized APK only.** The debug runtime is built `-O0` and debuggable apps
  run with CheckJNI. `build.sh` builds `:app:assembleRelease -Poptimized` signed with the
  debug keystore, because shipped apps use the optimized runtime.
- **Install, then `cmd package compile -m speed -f`, then throw away the first launch.**
  On some phones the first launch after an install runs about 1.6x slower from about
  3.5 s into the process, on every row. `run.sh` installs once per round (once per APK in a
  round, if labels use separate APKs), speed-compiles, runs one discarded launch
  (`--discard-secs`, default 8 s), and measures only relaunches.
- **Every label runs in every round, in rotated order.** Ratios are taken within a round
  (`table.py`), because absolute times swing between launches by core and frequency
  state. Rotation keeps a label from always running first or last.
- **An A/A label.** It is a second label with the anchor's exact configuration. Its
  ratio range is the noise floor, and `table.py --aa` compares every other range against it.
- **One device per experiment.** `run.sh` records the serial in `<out>/device.txt` and
  refuses to add rounds from another device to the same directory.
- **No builds while measuring.** A concurrent gradle build once inflated every row by
  50% (on an emulator, which shares the host's CPU). `launches.tsv` records the host's
  load average and the device's thermal status for every launch, so you can check.

## Run-time variants (`--variants`)

Building one APK per label is slow. It also leaves several APKs that each need an
install, a speed compile and a discarded launch every round. Instead, a benchmark build
can carry every label as a run-time branch:

- `build.sh --variants` passes `-PbenchVariants`, which sets the CMake option
  `NS_BENCH_VARIANTS` (off by default). The runtime then reads the system property
  `debug.ns.benchvariant` (an integer, default 0) once, when `libNativeScript.so` loads.
- C++: `#include "BenchVariant.h"`, then `if (tns::BenchVariant() == 1) { ... }`.
  The accessor is an inline read of a process global. Without the option it is
  `constexpr 0`, so the branches compile away and nothing reads the property.
- JS: `globalThis.__benchVariant`, a read-only number. It is `undefined` without the
  option, so use `(globalThis.__benchVariant | 0)`.
- Java can read the property itself (`getprop debug.ns.benchvariant`); the test app's
  `JniBench.getProp` shows how.
- `run.sh` sets the property before each launch. The runtime logs
  `NS_BENCH_VARIANT=<n>` at startup, and `run.sh` checks it against the label. A
  mismatch marks the launch `wrong-variant` and keeps its log as `*.log.bad`. A missing
  line aborts the run, because the APK was built without `--variants`.

**Convention.** Variant branches live in local, bench-only commits on top of the change
under test, and are never pushed. Variant 0 is the code as it would ship. Each other
number is one alternative, and the A/A label also uses 0.

**Caveat: one binary, one code layout.** Every variant shares the same compiled code.
An alternative whose effect depends on code layout, inlining or I-cache footprint
(adding a large function, moving a hot loop) can measure differently once it is the
only code path. The A/A pair catches noise, not layout. Confirm a layout-sensitive
result, or any result you will publish, with separate APKs: one `build.sh` per side, run
as `base=base.apk base2=base.apk new=new.apk`. Build both sides with the same `--ccache`
setting. A ccache build rewrites source paths (`__FILE__`), so it is not byte-identical
to a build without ccache.

## ccache (`--ccache`)

`--ccache` builds through ccache with settings that share cache entries across
checkouts. The runtime's existing switch accepts `-PuseCCache` (used by CI and
packaging), `-PuseCcache`, or `NS_USE_CCACHE=1` in the environment. `build.sh` also sets:

| variable | value | why |
|---|---|---|
| `CCACHE_DIR` | `$NS_PERF_CCACHE_DIR`, else `~/Library/Caches/ns-android-ccache` | a dedicated cache, so unrelated projects don't evict runtime objects |
| `CCACHE_MAXSIZE` | `$NS_PERF_CCACHE_MAXSIZE`, else `3G` | one arm64 configuration takes about 25 MB, so 3 GB holds many |
| `CCACHE_BASEDIR` | the checkout root | CMake passes absolute source and `-I` paths. ccache rewrites them relative to the build directory, which is the same in every checkout |
| `CCACHE_NOHASHDIR` | `1` | `-g` makes ccache hash the working directory by default, which differs per checkout |

Measured on an M-series Mac with a host load average of about 8 (arm64,
`-Poptimized --variants`, whole `build.sh` wall time):

| build | wall time | ccache |
|---|---:|---|
| first build in a fresh worktree, empty cache | 28 s | 79 misses |
| first build in a second worktree (different path), same sources | 18 s | 79/79 hits |
| same worktree, native build state deleted, no ccache | 22 s | — |
| same worktree, native build state deleted, warm cache | 12 s | 79/79 hits |

The rest of a build (gradle, Java, dex, packaging) is not cached, and a build that only
changes JS or Java reuses the native objects anyway. ccache pays off in fresh worktrees,
after switching a CMake flag (`-Poptimized`, `--variants`), each of which recompiles
everything, and on a busy host, where native compilation slows the most.
`ccache -s` with the same `CCACHE_DIR` shows totals.

## Device lock and device selection

Several sessions share each device, so every command that touches one holds a lock
for its whole session. `run.sh` takes it per round, so other sessions can interleave
between rounds.

- **The lock** is a directory created with `mkdir`. It is
  `$NS_PERF_LOCK`, or `--lock PATH`, or else
  `${NS_PERF_LOCK_DIR:-~/.cache/ns-perf/locks}/phone-<serial>.lock`. To share an
  existing lock with other tools, point `NS_PERF_LOCK` (or `--lock`) at the same path.
  The holder writes an `owner` file inside the lock, and waiters print it.
- **Cleanup happens only while the lock is held.** That covers force-stopping the app
  and clearing the `debug.ns.*` properties. A process that is still waiting, or that is
  interrupted while waiting, touches nothing. A stale lock is never broken
  automatically: check `owner` and remove it by hand.
- **`--serial` is required.** The scripts never pick a device for you. Serials in
  `NS_PERF_PROTECTED_SERIALS` (default `emulator-5554`) are refused outright.

## Commands

### `build.sh`

```
build.sh --out DIR [--label NAME] [--variants] [--ccache] [--profileable] [--regular]
         [--abi ABI] [--repo DIR] [--keep-build]
```

This builds `DIR/<label>.apk`, plus the unstripped `DIR/<label>.syms/libNativeScript.so` for
simpleperf symbols. It writes `DIR/<label>.build.txt` with the commit, whether the tree
was dirty, the flags, the `.so` hash, the wall time and the ccache hits. It runs gradle
under `nice -n 19`. It deletes `app/build` and `assets/metadata` first (stale metadata
hides new Java classes from JS), and deletes `app/build` again afterwards to save disk.
`--profileable` adds `<profileable android:shell="true"/>` to the test app manifest for
this one build and restores the file afterwards.

### `run.sh`

```
run.sh --serial S --out DIR --labels "NAME=SPEC ..." [--apk FILE] [--rounds N]
       [--first-round K] [--bench NAME] [--filter F | --groups FILE] [--scale N]
       [--lock PATH] [--no-wait] [--gap SECS] [--discard-secs N] [--no-discard] [--timeout SECS]
```

- A label spec is `VARIANT` (a variant of `--apk`), `APK` or `APK:VARIANT`.
- `--groups FILE` runs one launch per line and label; each line is a filter. Use it to keep
  rows that disturb each other (GC-heavy rows, large heaps) out of the same process.
- `--first-round K` appends more rounds to an existing directory.
- Output:
  - `DIR/<label>_r<round>.g<group>.log`: the bench lines of one launch.
  - `launches.tsv`: per launch, the status, seconds, row count, thermal status, host load
    average, and whether it was the first launch after an install.
  - `rounds.tsv`: per round, the lock wait, the round time, installs and launches.
  - `run.txt`: the APK hashes and filters used.

### `table.py`

```
table.py DIR[,DIR...] --anchor LABEL [--aa LABEL] [--labels A,B] [--metric median|min|norm]
         [--rows SUBSTR] [--md]
```

The table shows the absolute medians and launch counts. Under them, for each label, it
shows the within-round ratio to the anchor as median [min-max] over rounds. Flags:

- `=` means the label's ratio range overlaps the A/A range, so it is indistinguishable
  from noise.
- `*` means the ranges do not overlap.
- `B` means the per-launch values look bimodal, so compare medians with care.

`--metric norm` divides by the row's reference timing (callback rows time a reference
callback in the same round). `--md` prints Markdown for a PR body.

### `stat.sh` and `statsum.py`

```
stat.sh --serial S --out DIR --labels "..." [--apk FILE] --rows FILE [--repeat N] [--events LIST]
statsum.py DIR --anchor LABEL [--md]
```

These count user-space instructions and cycles on the JS thread with `simpleperf stat`.
Each row in `FILE` (`<key> <scale1> <scale2> <filter>`) runs at two scales. The difference
cancels startup and every other fixed cost, and `statsum.py` divides it by the
difference in the `ops` the bench reports for the row. The result is counts per
operation of the row. Instruction counts resolve differences of a few instructions per
op, far below what wall-clock times can show on a phone. Cycle counts are noisier.

`stat.sh` needs an APK built with `--profileable`, on Android 10 (API 29) or later, and
`simpleperf` on the device (`adb shell simpleperf --version`; if it is missing, push the
NDK's `simpleperf/bin/android/<arch>/simpleperf`). Keep the events to what the CPU counts at
once; `statsum.py` marks multiplexed counts `mux`.

Example rows file:

```
add   4 20 static.add(int,int)
field 4 20 field.get int
cb    4 20 callback.noArgs()
```

## Benchmark rows

The JNI bridge micro-benchmarks from #2092 are the default bench
(`--bench jnibench`). They live in
`test-app/app/src/main/assets/app/tests/benchmarks/jniBenchmarks.js`, with Java targets
in `test-app/app/src/main/java/com/tns/tests/JniBench*.java`. They cover calls, field
access, object creation and lifecycle, callbacks and contention.

A bench named `X` follows this contract, so a branch can add its own without changing
the harness:

- `debug.ns.X=1` makes `boot.js` run the bench instead of the Jasmine suite.
- `debug.ns.X.filter` holds `|`-separated substrings that select rows by name.
  `debug.ns.X.scale` multiplies the iterations.
- Each row logs one `X_UPPER {json}` line through `console.log`, where `X_UPPER` is `X` in
  upper case. The JSON has at least `name` and `median` (ns per op), and optionally
  `min`, `ref` (a reference timing from the same rounds) and `ops`. `ops` is every
  operation the row executed in the launch, including warm-up and reference calls, and
  `stat.sh` needs it.
- After the last row, the bench logs `X_UPPER_DONE`.

To add rows to `jnibench`, add a `measureJs` or `measureCallback` entry to
`buildBenchmarks()`. Keep each operation trivial, so the bridge dominates the
measurement.

**GC pitfall.** If a row times `gc()`, cross into Java once just before it, for example
with a trivial static call. Some runtime state is released at the next JS-to-Java
crossing. Without that crossing, the timed `gc()` also pays for releasing work left by
earlier rows.

## A new device

The scripts are device-agnostic, but check these before trusting numbers from a new
device:

1. **Serial:** `adb devices -l`. Pass it as `--serial`, and give the device its own
   lock, which is the default `phone-<serial>.lock`.
2. **First-launch behaviour:** run four or more rounds of `--labels "a=0 b=0" --no-discard`.
   Then the first launch after each install is measured, and `launches.tsv` marks it
   `first=1`. Rotation alternates which label gets it, so the `b`/`a` ratio in
   `table.py` swings round by round if first launches are slow. On the phone where this
   was found, the slowdown starts about 3.5 s into the process, so pick rows that run
   well past that. Keep the default discard
   either way; it is cheap insurance.
3. **simpleperf:** `adb shell simpleperf --version`, and `getprop ro.build.version.sdk`.
   Below API 29 (for example a Galaxy S8 on Android 9), apps cannot be profileable, so
   `stat.sh` does not work with a release APK. Use wall-clock rounds only, or a rooted
   device.
4. **Thermal and governor:** watch the thermal column of `launches.tsv`. Older phones
   throttle sooner, so lengthen `--gap` if later launches drift slower.

Each experiment stays on one device. Two devices can run two independent experiments
at once, one per lane, each with its own lock and output directory.
