# Sourced by the tools/pgo scripts; not meant to be run directly.

PGO_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(cd "$PGO_DIR/../.." && pwd)
TEST_APP="$REPO_ROOT/test-app"
PKG=com.tns.testapplication
ACTIVITY=$PKG/com.tns.NativeScriptActivity

die() {
  printf '%s\n' "$*" >&2
  exit 1
}

# adb reads stdin, so without the redirect a call inside a loop or a heredoc
# swallows the rest of the caller's input.
adbs() {
  adb ${SERIAL:+-s "$SERIAL"} "$@" </dev/null
}

require_device() {
  command -v adb >/dev/null || die "adb not found on PATH"
  local state
  state=$(adbs get-state 2>/dev/null) || die "no device (set ANDROID_SERIAL or pass -s when several are attached)"
  [ "$state" = device ] || die "device state is '$state'"
}

require_abi_on_device() {
  local abi=$1 abis
  abis=$(adbs shell getprop ro.product.cpu.abilist | tr -d '\r')
  case ",$abis," in
    *",$abi,"*) ;;
    *) die "device ABIs are '$abis'; it cannot run $abi code" ;;
  esac
}

# Reinstalls (uninstalling first when the signature differs) and AOT-compiles
# the app, so the measured runs are not also measuring the JIT warming up.
install_apk() {
  local apk=$1
  adbs install -r "$apk" >/dev/null || {
    adbs uninstall "$PKG" >/dev/null || true
    adbs install "$apk" >/dev/null
  }
  adbs shell cmd package compile -m speed -f "$PKG" >/dev/null
}

set_bench_props() {
  local enabled=$1 filter=${2:-}
  adbs shell setprop debug.ns.jnibench "$enabled"
  adbs shell setprop debug.ns.jnibench.filter "\"$filter\""
}

reset_bench_props() {
  set_bench_props 0 "" || true
}

file_hash() {
  cksum "$1" | awk '{print $1 "-" $2}'
}

# The NDK whose clang built the runtime: AGP compiles with android.ndkVersion,
# which is not necessarily the NDK that ANDROID_NDK_HOME points at.
ndk_dir() {
  local sdk=${ANDROID_HOME:-${ANDROID_SDK_ROOT:-}} version
  if [ -z "$sdk" ] && [ -f "$TEST_APP/local.properties" ]; then
    sdk=$(sed -n 's/^sdk\.dir=//p' "$TEST_APP/local.properties")
  fi
  [ -n "$sdk" ] || die "set ANDROID_HOME"
  version=${PGO_NDK_VERSION:-$(sed -n 's/^def defaultNdkVersion = "\(.*\)"/\1/p' "$TEST_APP/runtime/build.gradle")}
  [ -n "$version" ] || die "could not read defaultNdkVersion from test-app/runtime/build.gradle; set PGO_NDK_VERSION"
  [ -d "$sdk/ndk/$version" ] || die "NDK $version is not installed under $sdk/ndk"
  printf '%s\n' "$sdk/ndk/$version"
}

# llvm-profdata must come from the same LLVM release as the clang that built
# the instrumented runtime: .profraw is a version-specific format.
llvm_profdata() {
  if [ -n "${LLVM_PROFDATA:-}" ]; then
    printf '%s\n' "$LLVM_PROFDATA"
    return
  fi
  local ndk tool
  ndk=$(ndk_dir)
  tool=$(find "$ndk/toolchains/llvm/prebuilt" -maxdepth 3 -path '*/bin/llvm-profdata' | head -1)
  [ -n "$tool" ] || die "llvm-profdata not found in $ndk"
  printf '%s\n' "$tool"
}
