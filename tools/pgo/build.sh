#!/usr/bin/env bash
# Builds the test app as an optimized, release-signed APK for one ABI, with a
# plain, an instrumented or a profile-optimized runtime.
#
# usage: tools/pgo/build.sh [-a abi] -o outdir [--generate | --use file.profdata] [-- extra gradle args]
#
#   -a abi        ABI to build (default arm64-v8a). Profiles are per ABI.
#   -o outdir     receives app.apk, libNativeScript.so (unstripped) and BUILD_INFO.
#   --generate    instrumented runtime (-fprofile-generate) exposing __pgoDump().
#   --use FILE    runtime optimized with FILE, a profile merged for the same ABI.
#
# Release signing defaults to the debug keystore; override with KS_PATH,
# KS_PASSWORD, KS_ALIAS and KS_ALIAS_PASSWORD.
set -euo pipefail
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"

ABI=arm64-v8a
OUT=""
MODE=none
PROFILE=""
while [ $# -gt 0 ]; do
  case $1 in
    -a) ABI=$2; shift 2 ;;
    -o) OUT=$2; shift 2 ;;
    --generate) MODE=generate; shift ;;
    --use) MODE=use; PROFILE=$2; shift 2 ;;
    --) shift; break ;;
    *) die "unknown argument: $1" ;;
  esac
done
[ -n "$OUT" ] || die "usage: build.sh [-a abi] -o outdir [--generate | --use file.profdata] [-- gradle args]"

case $MODE in
  none) CXX_FLAGS="" ;;
  generate) CXX_FLAGS="-fprofile-generate -DNS_PGO_GENERATE" ;;
  use)
    [ -f "$PROFILE" ] || die "profile not found: $PROFILE"
    PROFILE=$(cd "$(dirname "$PROFILE")" && pwd)/$(basename "$PROFILE")
    case $PROFILE in *" "*) die "the profile path must not contain spaces" ;; esac
    # The profile always lags the sources a little; functions it no longer
    # matches are compiled without it, so these warnings are expected noise.
    CXX_FLAGS="-fprofile-use=$PROFILE -Wno-profile-instr-unprofiled -Wno-profile-instr-out-of-date -Wno-profile-instr-missing"
    ;;
esac

GRADLE_ARGS=(
  :app:assembleRelease -Poptimized -Prelease "-Pabis=$ABI"
  "-PksPath=${KS_PATH:-$HOME/.android/debug.keystore}"
  "-PksPassword=${KS_PASSWORD:-android}"
  "-Palias=${KS_ALIAS:-androiddebugkey}"
  "-Ppassword=${KS_ALIAS_PASSWORD:-android}"
  --console=plain
)
if [ -n "$CXX_FLAGS" ]; then
  GRADLE_ARGS+=("-PnsExtraCxxFlags=$CXX_FLAGS")
fi

mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)

cd "$TEST_APP"
# buildMetadata only registers its inputs when this dir exists at configuration
# time, so a leftover one makes a release build reuse stale metadata.
rm -rf app/build app/src/main/assets/metadata
# Each flag set links into its own cxx/<config>/<hash>/ dir; removing the old
# unstripped libraries leaves this build's as the only one to copy.
if [ -d runtime/build/intermediates/cxx ]; then
  find runtime/build/intermediates/cxx -path "*/obj/$ABI/libNativeScript.so" -delete
fi
nice -n 19 ./gradlew "${GRADLE_ARGS[@]}" "$@"

APK=$(find app/build/outputs/apk/release -name '*.apk' | head -1)
[ -n "$APK" ] || die "no release APK was produced"
cp "$APK" "$OUT/app.apk"
SO=$(find runtime/build/intermediates/cxx -path "*/obj/$ABI/libNativeScript.so")
[ -n "$SO" ] && [ "$(printf '%s\n' "$SO" | wc -l)" -eq 1 ] || die "expected one unstripped libNativeScript.so for $ABI, found: $SO"
cp "$SO" "$OUT/libNativeScript.so"
{
  printf 'commit=%s\n' "$(git -C "$REPO_ROOT" rev-parse HEAD)"
  printf 'abi=%s\nmode=%s\nprofile=%s\ncxx_flags=%s\n' "$ABI" "$MODE" "$PROFILE" "$CXX_FLAGS"
} >"$OUT/BUILD_INFO"
echo "built $MODE $ABI -> $OUT"
