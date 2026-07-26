#!/bin/bash
set -e
#
# Builds libv8_monolith.a for every Android ABI the runtime ships and copies the
# result into test-app/runtime/src/main/libs/<abi>/, then re-vendors the public
# headers into test-app/runtime/src/main/cpp/include/.
#
# Requires a V8 checkout synced with target_os including "android" (see
# fetch_v8.sh) and depot_tools on PATH.
#
# Usage: build_v8_source.sh [--v8-dir <path>] [--abi <abi> ...] [-- <ninja args>]
#

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
V8_DIR="$HOME/projects/v8/v8"
NDK_ROOT="${ANDROID_HOME:-$HOME/Library/Android/sdk}/ndk/29.0.14206865"
ABIS=()
NINJA_ARGS=()

usage() {
    cat <<EOF
Usage: $(basename "$0") [--v8-dir <path>] [--abi <abi>]... [-- <ninja args>]

  --v8-dir <path>  V8 checkout to build from (default: $V8_DIR)
  --ndk-root <p>   NDK to build against; must match the one the runtime is
                   built with (default: $NDK_ROOT)
  --abi <abi>      Build only this ABI; repeatable. One of:
                   armeabi-v7a, arm64-v8a, x86, x86_64
                   (default: all four)
  -- <ninja args>  Everything after -- is forwarded to ninja (e.g. -j 8)
EOF
}

while [ $# -gt 0 ]; do
    case "$1" in
        --v8-dir)     V8_DIR="$2"; shift 2 ;;
        --ndk-root)   NDK_ROOT="$2"; shift 2 ;;
        --ndk-root=*) NDK_ROOT="${1#*=}"; shift ;;
        --v8-dir=*) V8_DIR="${1#*=}"; shift ;;
        --abi)      ABIS+=("$2"); shift 2 ;;
        --abi=*)    ABIS+=("${1#*=}"); shift ;;
        --)         shift; NINJA_ARGS=("$@"); break ;;
        -h|--help)  usage; exit 0 ;;
        *)          echo "Unknown argument: $1" >&2; usage >&2; exit 1 ;;
    esac
done

if [ ${#ABIS[@]} -eq 0 ]; then
    ABIS=(arm64-v8a armeabi-v7a x86_64 x86)
fi

function checkpoint {
    echo ""
    echo "--------------------------------------------------------------------------------"
    echo "--- $(date +'%T') --- $1"
    echo "--------------------------------------------------------------------------------"
}

# Android ABI name -> gn target_cpu
function gn_cpu_for_abi {
    case "$1" in
        armeabi-v7a) echo "arm"   ;;
        arm64-v8a)   echo "arm64" ;;
        x86)         echo "x86"   ;;
        x86_64)      echo "x64"   ;;
        *) echo "Invalid ABI: $1" >&2; exit 1 ;;
    esac
}

# Matches the 10.3 build this replaces: JIT and WebAssembly on, i18n off.
#
# v8_enable_sandbox is pinned off rather than left to default. It would
# otherwise follow pointer compression (on for 64-bit) and change the object
# layout the runtime compiles against, which is a far larger change than a
# version bump.
#
# use_thin_lto is pinned off because is_official_build now turns it on, and a
# ThinLTO build emits LLVM bitcode rather than object code -- the resulting
# archive cannot be linked by the app's NDK.
#
# The array buffer internal field counts default to 0 in 14.9 and defaulted to 2
# in 10.3. ObjectManager::Link stores its JSInstanceInfo in internal field 0 of
# the buffer object itself when marshalling an ArrayBuffer/TypedArray to a Java
# NIO buffer, so 0 fields makes every such conversion throw "Trying to link
# invalid 'this' to a Java object". v8-array-buffer.h still falls back to 2 when
# the macro is undefined, which is what the runtime compiles against.
#
# The allocator shim interposes on malloc through linker --wrap flags that the
# embedder would have to pass too. PartitionAlloc itself stays on -- V8 depends
# on the target -- it just no longer replaces malloc.
#
# Temporal is implemented in Rust; enabling it requires linking a Rust sysroot
# that this build does not provide.
#
# android_ndk_root is pinned to the NDK the runtime itself is built with. V8's
# bundled NDK is newer, and its libc++ exports symbols (std::__ndk1::__hash_memory
# and friends) that the runtime's libc++_static.a does not, which only surfaces
# at link time.
GN_ARGS_BASE="
    target_os=\"android\"
    is_component_build=false
    is_debug=false
    is_official_build=true
    chrome_pgo_phase=0
    treat_warnings_as_errors=false
    symbol_level=0
    use_thin_lto=false
    default_min_sdk_version=21

    use_custom_libcxx=false
    icu_use_data_file=false

    use_allocator_shim=false
    use_partition_alloc_as_malloc=false

    v8_array_buffer_internal_field_count=2
    v8_array_buffer_view_internal_field_count=2

    v8_monolithic=true
    v8_static_library=true
    v8_use_external_startup_data=false

    v8_enable_i18n_support=false
    v8_enable_webassembly=true
    v8_enable_sandbox=false
    v8_enable_temporal_support=false
    v8_enable_v8_checks=false
    v8_enable_debugging_features=false
    v8_control_flow_integrity=false
"

pushd "$V8_DIR" > /dev/null

for ABI in "${ABIS[@]}"; do
    CPU=$(gn_cpu_for_abi "$ABI")

    # mksnapshot has to run V8's simulator for a 32-bit target, which v8config.h
    # only allows from a 32-bit x86 host. Linux x64 builds mksnapshot as an x86
    # binary for exactly this; macOS cannot.
    if [ "$(uname)" != "Linux" ] && { [ "$CPU" = "arm" ] || [ "$CPU" = "x86" ]; }; then
        echo "Skipping $ABI: 32-bit snapshots require a Linux x64 host." >&2
        continue
    fi
    OUTFOLDER="out.gn/android-$CPU-release"

    checkpoint "Building V8 for $ABI ($CPU) in $OUTFOLDER"

    # ninja never deletes outputs orphaned by a config change, so reusing an
    # output directory across V8 versions silently keeps stale objects.
    rm -rf "$OUTFOLDER"
    gn gen "$OUTFOLDER" --args="${GN_ARGS_BASE} target_cpu=\"$CPU\" v8_target_cpu=\"$CPU\" android_ndk_root=\"$NDK_ROOT\""

    echo "Started building v8: $(date)"
    ninja "${NINJA_ARGS[@]}" -C "$OUTFOLDER" v8_monolith
    echo "Finished building v8: $(date)"

    DEST="$REPO_ROOT/test-app/runtime/src/main/libs/$ABI"
    mkdir -p "$DEST"
    cp "$OUTFOLDER/obj/libv8_monolith.a" "$DEST/libv8_monolith.a"
    echo "Installed $DEST/libv8_monolith.a ($(du -h "$DEST/libv8_monolith.a" | cut -f1))"
done

checkpoint "Vendoring headers"

INCLUDE_DEST="$REPO_ROOT/test-app/runtime/src/main/cpp/include"
# zip.h/zipconf.h belong to libzip, not V8; inspector/ holds the generated
# protocol headers copied from the build output below.
rsync -a --delete \
    --exclude='zip.h' --exclude='zipconf.h' --exclude='inspector/' \
    "$V8_DIR/include/" "$INCLUDE_DEST/"

# The generated protocol headers are architecture-independent, but only a
# configuration that actually built has them -- a directory left behind by a
# failed run would silently yield a partial tree.
GEN_DIR=$(ls -d out.gn/android-*-release/gen 2>/dev/null | while read -r d; do
    [ -f "$d/src/inspector/protocol/Runtime.h" ] && echo "$d" && break
done)
if [ -z "$GEN_DIR" ]; then
    echo "No build output with generated inspector protocol sources." >&2
    exit 1
fi
cp "$GEN_DIR/include/inspector/"*.h "$INCLUDE_DEST/inspector/"

# The inspector glue compiles against V8 internals, which are not part of the
# public headers. Keeping that copy on an older V8 than libv8_monolith.a is an
# ABI mismatch, so it is refreshed from the same tree.
python3 "$SCRIPT_DIR/vendor_inspector_sources.py" \
    --v8-dir "$V8_DIR" --gen-dir "$V8_DIR/$GEN_DIR"

popd > /dev/null

checkpoint "Done"
grep -E "V8_(MAJOR_VERSION|MINOR_VERSION|BUILD_NUMBER|PATCH_LEVEL)" "$INCLUDE_DEST/v8-version.h"
