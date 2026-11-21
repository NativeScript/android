# Profile-guided optimization (PGO)

**Status: experimental.** No shipped build uses a profile. This page covers
the tooling for training a profile and building with it, how the spike was
measured and what it found, and a proposed CI design.

## What PGO does here

PGO is a two-step compile. First, clang builds an instrumented
`libNativeScript.so` (`-fprofile-generate`) that counts how often each branch,
call and basic block runs. That build then runs a representative workload. A
second, normal compile reads the counts back in (`-fprofile-use`). With real
counts in hand, the optimizer can:

- lay out hot paths as fall-through code and move cold code out of line;
- inline the calls that are hot and skip the ones that are not;
- promote hot indirect calls to guarded direct calls;
- order switch cases by frequency.

Only the runtime's own C++ gets profiled, meaning the sources listed in
`test-app/runtime/CMakeLists.txt`. V8 and libzip are linked from prebuilt
static archives, so their code is the same in every variant. The gains therefore
land where the runtime itself spends time: the JS↔Java bridge (argument
conversion, method and field dispatch, object wrapping) and startup code.

## What is in the tree

| Piece | Where | What it does |
| --- | --- | --- |
| `-PnsExtraCxxFlags=` / `-PnsExtraLinkFlags=` | `test-app/runtime/build.gradle`, `CMakeLists.txt` | Appended to `CMAKE_CXX_FLAGS` / the `NativeScript` target's `LINK_FLAGS`. Generic pass-through; PGO only needs the first, since CMake also passes the C++ flags to the link step, which links clang's profile runtime. |
| `__pgoDump(path)` | `test-app/runtime/src/main/cpp/PgoHooks.cpp` | Exists only in builds compiled with `-DNS_PGO_GENERATE`. Writes the counters gathered so far to `path`; returns 0 on success. |
| `__nsPgoDump(tag)` | `test-app/app/src/main/assets/app/boot.js` | Test-app helper. If `__pgoDump` exists, writes `ns-<tag>.profraw` to the app's external files dir and logs `PGO_DUMP <tag> rc=<n>`. It is called at the end of the Jasmine suite (`__JUnitSaveResults`) and of the JNI benchmarks (`JNIBENCH_DONE`). |
| `tools/pgo/` | repo root | Build, train, merge and measurement scripts, described below. |

Notes on these pieces:

- **Why the explicit dump.** Android kills app processes instead of letting
  them exit, so the profile runtime's at-exit write never runs. The training
  run has to call `__pgoDump` itself.
- **Why the external files dir.** The training APK is a release build. It
  is not debuggable, so `run-as` cannot read the app's data directory, but the
  external files dir is reachable with a plain `adb pull`.
- **Gradle scope.** The flag properties reach the runtime only when gradle
  runs inside `test-app/`. The root `build.gradle` builds the npm package
  through `Exec` tasks (`generateOptimizedRuntimeAar` and the others), and
  those do not forward the flags yet.

## Scripts

All scripts are repo-relative. Device scripts take `-s <serial>` or fall back
to `ANDROID_SERIAL`, then to the only attached device. Run any of them without
arguments to see its usage.

| Script | Purpose |
| --- | --- |
| `build.sh` | Builds an optimized, release-signed test app for one ABI with a plain (default), instrumented (`--generate`) or profile-optimized (`--use file.profdata`) runtime. Writes `app.apk`, the unstripped `libNativeScript.so` and `BUILD_INFO` to `-o`. |
| `train.sh` | Installs an instrumented APK and runs the JNI benchmarks (skip them with `--no-bench`) and then the full suite, each as a fresh launch. Pulls the raw profiles and merges them into `<outdir>/<abi>.profdata`. |
| `merge.sh` | `llvm-profdata merge` with the right `llvm-profdata` (see [Merge](#3-merge)). |
| `bench.sh` | One launch of the JNI benchmarks, saving the `JNIBENCH` rows plus the device's thermal and cpufreq state. |
| `ab.sh` | Interleaved comparison of several APKs over N rounds. Each install is followed by a discarded launch, then the measured relaunch. |
| `summarize.py` | `ab` mode: per-row medians and deltas from `ab.sh` output. `startup` mode: cold-start medians from `startup.sh`. |
| `startup.sh` | Cold-start `TotalTime` (`am start -W`) of several APKs, interleaved. |

## Workflow

Each step below works on one ABI. Repeat the whole sequence for every ABI you
want a profile for.

### 1. Build an instrumented runtime

```sh
tools/pgo/build.sh -a arm64-v8a -o out/gen --generate
```

This runs the following:

```sh
cd test-app
rm -rf app/build app/src/main/assets/metadata
./gradlew :app:assembleRelease -Poptimized -Prelease -Pabis=arm64-v8a \
  "-PnsExtraCxxFlags=-fprofile-generate -DNS_PGO_GENERATE" \
  -PksPath=... -PksPassword=... -Palias=... -Ppassword=...
```

Train on the optimized release runtime: it is the one that ships. A debug
runtime is built at `-O0` and runs under CheckJNI, so its counts would describe
different code. The instrumented `.so` is about 2 MB larger (stripped, in
the APK). You can recognize it by its `__llvm_prf_*` sections
(`llvm-readelf -S`), and `train.sh` checks for them.

### 2. Train

```sh
tools/pgo/train.sh -s <serial> -a arm64-v8a -o out/train out/gen/app.apk
```

`train.sh` refuses an APK whose `lib/<abi>/libNativeScript.so` is not
instrumented, and a device that cannot run `<abi>`. It then:

1. installs the APK and AOT-compiles it;
2. launches it with `debug.ns.jnibench=1` and waits for `PGO_DUMP bench`;
3. launches it again as the normal test app and waits for `PGO_DUMP suite`;
4. pulls `ns-bench.profraw` and `ns-suite.profraw`, then merges them.

The suite run in release mode reports some failures, 29 of 1773 specs in the
spike. Release builds refuse remote/HTTP module loading, and the failing
specs exercise it. The failures do not affect what the profile covers.

On an SM-G780G the suite took about 136 s, and the benchmark run a few minutes.

`--no-bench` trains on the suite alone. In the spike, a suite-only profile
performed about as well as suite + benchmarks, even on the benchmark rows (see
[Results](#results)). So the profile is not overfit to the benchmark, and the
benchmark run is optional.

### 3. Merge

`train.sh` runs this step for you. To merge by hand:

```sh
tools/pgo/merge.sh out/arm64-v8a.profdata out/train/ns-bench.profraw out/train/ns-suite.profraw
```

`.profraw` is a format tied to the LLVM version that wrote it, so
`llvm-profdata` must come from the NDK that compiled the runtime.
`merge.sh` uses `android.ndkVersion` from `test-app/runtime/build.gradle`,
which is what AGP compiles with. That is not necessarily the NDK that
`ANDROID_NDK_HOME` points at. To override it, set `PGO_NDK_VERSION` or
`LLVM_PROFDATA`.

### 4. Build with the profile

```sh
tools/pgo/build.sh -a arm64-v8a -o out/pgo --use out/arm64-v8a.profdata
```

This passes:

```
-PnsExtraCxxFlags=-fprofile-use=/abs/path/arm64-v8a.profdata
  -Wno-profile-instr-unprofiled -Wno-profile-instr-out-of-date -Wno-profile-instr-missing
```

A profile always lags the sources a little. Clang matches functions by name
plus a hash of their control flow. A function whose hash no longer matches is
compiled as if there were no profile, and a source file with no data at all
gets the same treatment. Nothing breaks, but clang warns once per translation
unit:

| Flag | Silences |
| --- | --- |
| `-Wno-profile-instr-out-of-date` | "of N functions, M have mismatched data that will be ignored", for functions edited since training |
| `-Wno-profile-instr-unprofiled` | "no profile data available for file", for files added since training |
| `-Wno-profile-instr-missing` | "M have no data", for functions that never ran in training. Off by default, listed for completeness. |

These flags only silence the warnings. Staleness itself still has to be made
visible somewhere, which is part of the [CI design](#proposed-ci-design).

**Profiles are per ABI.** One gradle invocation compiles every ABI in
`-Pabis` with the same flags, so a single `-fprofile-use=` path works only for
a single-ABI build. A multi-ABI release build needs CMake to pick the profile
per ABI. That is not implemented yet; see the CI design.

## Measuring correctly

Most of the effects are 5–10%, so the measuring method matters more than
usual.

- **Benchmark only optimized release APKs.** These are `-Poptimized -Prelease`
  builds, which is what `build.sh` produces.
- **AOT-compile after every install** (`cmd package compile -m speed -f`), so
  the Java side is not still in the JIT.
- **Discard the first launch after an install.** On the SM-G780G the first
  launch after an install ran about 1.6× slower across the board. That would
  swamp the effect being measured. Compare relaunch against relaunch only;
  `ab.sh` saves the first launch separately as `first-<label>.<round>.txt`.
- **Interleave variants within each round** (A B C, A B C, ...). Thermal
  state and background activity drift over a session, and interleaving
  spreads that drift over every variant. The spike ran 6 rounds.
- **Run an A/A.** Run `ab.sh` with the same APK under two labels to get the
  noise floor. On the SM-G780G, single-call rows (static, instance and field
  access) moved by about ±1% between identical APKs. The construction,
  callback and contention rows are bimodal and moved by up to ±9%. Do not read
  effects into those rows.
- **Keep the host and device quiet.** Do not build on the host while
  measuring. `bench.sh` appends the thermal status and the cpufreq caps from
  before, during and after each run, so a throttled run can be spotted and
  dropped.

Compare with:

```sh
tools/pgo/summarize.py ab out/ab none pgo
```

## Results

The spike was built on main at `9a514a7a` plus this tooling. Measurements:

- Device: SM-G780G, Android 13, arm64-v8a.
- Build: optimized release, AOT-compiled.
- Method: 6 interleaved rounds, relaunches only. Each cell is the median over
  the 6 launches of each launch's median, in ns per call.

Three variants were compared:

- **none**: no profile;
- **bench+suite**: profile trained on the JNI benchmarks and the suite;
- **suite**: profile trained on the suite only.

| Row | none | bench+suite | suite |
| --- | ---: | ---: | ---: |
| `static.noop()` | 104.2 | 95.5 (−8.3%) | 94.6 (−9.2%) |
| `static.add(int,int)` | 186.4 | 172.0 (−7.7%) | 172.1 (−7.7%) |
| `instance.noop()` | 238.2 | 224.2 (−5.8%) | 226.6 (−4.9%) |
| `instance.getInt()` | 237.8 | 223.9 (−5.9%) | 225.8 (−5.1%) |
| `instance.echo(String)` | 678.9 | 640.0 (−5.7%) | 644.8 (−5.0%) |
| `instance.takeObject(Object)` | 512.8 | 482.9 (−5.8%) | 479.4 (−6.5%) |
| `instance.takeObjects(Object x3)` | 1070.5 | 1010.2 (−5.6%) | 992.5 (−7.3%) |
| `static.bufferCapacity(ArrayBuffer)` | 444.1 | 409.9 (−7.7%) | 404.8 (−8.9%) |
| `field.get int` | 162.6 | 151.2 (−7.0%) | 150.6 (−7.3%) |
| `field.get Object` | 331.2 | 319.6 (−3.5%) | 322.4 (−2.7%) |
| `static field get int [small class]` | 71.0 | 65.5 (−7.7%) | 67.8 (−4.5%) |

Other results:

- **Construction and callback rows look much better, but cannot be trusted.**
  `new java.lang.Object()` showed −9..−20%, a Java-created extended instance
  −17..−21%, and `callback.noArgs()` −60%. These rows are bimodal, and their
  A/A gaps reach ±9%.
- **Cold start is neutral.** Median of 15 launches (`startup.sh`): 182 ms
  none, 178 ms bench+suite, 181 ms suite.
- **Size changes are small either way.** The spike measured the `.so`
  growing by 30–85 KB. A rebuild of this branch with the same bench+suite
  profile came out 32 KB *smaller* than the plain build (stripped `.so` in the
  APK).
- **Not overfit.** The suite-only profile is about as good as bench+suite,
  including on the benchmark rows it never trained on.

## Maintenance cost

- **One profile per ABI, four ABIs.** Each profile needs a training run on
  hardware or an emulator that executes that ABI. On CI's x86_64 runners,
  x86/x86_64 emulators are cheap. arm64-v8a and armeabi-v7a need ARM runners
  or a device farm. These are the ABIs that matter most for real devices.
- **Profiles go stale.** Each change to runtime C++ invalidates the profile
  data of the functions it touches. Those functions quietly lose the benefit
  and nothing fails, so the gain decays over time instead of breaking
  visibly.
- **Release output depends on a binary artifact.** Reproducing a release
  build requires the exact profiles it used, so they must be kept with the
  release.
- **CI time.** Training costs an extra native build per ABI, an emulator or
  device boot, and about 2–3 minutes for the suite.
- **Workload bias.** The suite and the benchmarks are not real apps. Code
  that real apps use heavily but the suite barely touches is treated as cold.

## Proposed CI design

This is the direction proposed for discussion, not a commitment.

**Release builds (non-`next`: tags and manual runs with a version) must
train.** In `npm_release.yml`, a matrix job per ABI:

1. builds the instrumented test app for that ABI;
2. boots an emulator or device for that ABI and runs `train.sh`;
3. uploads `<abi>.profdata` as a workflow artifact.

The build job waits for all four, builds the runtime with them, and attaches
them to the GitHub release (for example `pgo-profiles-<version>.zip`), so the
release build can be reproduced.

**`next` (main pushes) and PR builds never train.** They download the
profiles attached to the latest release and build with those. With no
profiles available, they build without PGO. A slightly stale profile still
helps everything it still matches.

**Plumbing still needed:**

- Per-ABI profile selection in CMake. For example, `-PnsPgoProfileDir=<dir>`
  would use `<dir>/${ANDROID_ABI}.profdata` when that file exists and add the
  `-Wno-profile-instr-*` flags.
- Forwarding of that property through the root `build.gradle`'s
  `getAssembleReleaseBuildArguments`.

**Where profiles live:** as release assets, not in the repo. They are about
1 MB of binary per ABI, every release would replace them, and keeping them in
git would only add churn.

**Surfacing staleness:** a `next` build could compile once without
`-Wno-profile-instr-out-of-date` and report, in the job summary, the share of
functions whose profile data no longer matches. When that share grows large,
it is time for a release, or for a manual training run.

## Open questions

- Should training run on emulators or devices, per ABI? Where do the arm64-v8a
  and armeabi-v7a runs happen: ARM runners, a device farm, or armeabi-v7a in
  32-bit mode on an arm64 device? Could an x86_64 profile be reused for
  arm64-v8a? IR-level hashes match wherever the source has no ABI-specific
  code, but this is untested.
- Is the CI time per ABI (about 2–3 minutes of suite plus an instrumented
  build and an emulator boot) acceptable on every release?
- Should profiles be stored as release assets or in the repo?
- How should staleness be surfaced, and at what point should it block a
  release?
- Could V8 ever be included? That would mean instrumented V8 builds in the V8
  build scripts, and a profile that matches the exact V8 revision. The payoff
  could be larger, but so is the cost.
