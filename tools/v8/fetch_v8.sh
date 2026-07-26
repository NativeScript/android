#!/bin/bash
set -e
#
# Fetches and patches the V8 checkout that build_v8_source.sh builds from.
# Requires depot_tools on PATH.
#
# Usage: fetch_v8.sh [--v8-dir <path>]
#
# <path> is the directory that will *contain* the checkout; the V8 sources end
# up in <path>/v8 and the .gclient file in <path>.
#

V8_VERSION="14.9.207.39"
V8_PARENT="$HOME/projects/v8"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

usage() {
    cat <<EOF
Usage: $(basename "$0") [--v8-dir <path>]

  --v8-dir <path>  Directory to hold the checkout (default: $V8_PARENT).
                   Sources land in <path>/v8.
EOF
}

while [ $# -gt 0 ]; do
    case "$1" in
        --v8-dir)   V8_PARENT="$2"; shift 2 ;;
        --v8-dir=*) V8_PARENT="${1#*=}"; shift ;;
        -h|--help)  usage; exit 0 ;;
        *)          echo "Unknown argument: $1" >&2; usage >&2; exit 1 ;;
    esac
done

function checkpoint {
    echo ""
    echo "--------------------------------------------------------------------------------"
    echo "--- $(date +'%T') --- $1"
    echo "--------------------------------------------------------------------------------"
}

mkdir -p "$V8_PARENT"
cd "$V8_PARENT"

checkpoint "Fetching V8 $V8_VERSION"
gclient config --name v8 --unmanaged "https://chromium.googlesource.com/v8/v8.git"

# gclient config writes target_os = ['unix'] or nothing at all; the android
# entry is what pulls third_party/android_toolchain and android_platform.
if ! grep -q "target_os" .gclient; then
    echo "target_os = ['android']" >> .gclient
elif ! grep -q "'android'" .gclient; then
    sed -i.bak "s/target_os = \[/target_os = ['android', /" .gclient && rm -f .gclient.bak
fi

checkpoint "Syncing V8"
gclient sync --reset --with_branch_head --revision "$V8_VERSION" --delete_unversioned_trees

checkpoint "Patching V8"
# Restores WeakCallbackType::kFinalizer, removed upstream right after 10.3.22.
# ObjectManager relies on resurrecting finalizers for its dead-object pass.
git -C v8 apply "$SCRIPT_DIR/v8_resurrecting_finalizers.patch"

if [ "$(uname)" = "Darwin" ]; then
    checkpoint "Enabling the Android build on this macOS host"

    git -C v8/build apply "$SCRIPT_DIR/android_build_on_macos.patch"

    # android_toolchain_root is only read for the sysroot, which is
    # host-independent -- but the path is tagged with the host OS and CIPD only
    # ships the linux-x86_64 tree. Only matters when build_v8_source.sh is run
    # without --ndk-root, since it otherwise points at a released NDK.
    ln -sfn linux-x86_64 \
        "v8/third_party/android_toolchain/ndk/toolchains/llvm/prebuilt/darwin-x86_64"

    # The macOS clang package omits the Android compiler-rt builtins that the
    # Linux one bundles, and the link needs them.
    CLANG_VERSION=$(python3 v8/tools/clang/scripts/update.py --print-revision)
    CLANG_TARBALL="clang-$CLANG_VERSION.tar.xz"
    curl -fL -o "$CLANG_TARBALL" \
        "https://commondatastorage.googleapis.com/chromium-browser-clang/Linux_x64/$CLANG_TARBALL"
    CLANG_MAJOR=$(ls "v8/third_party/llvm-build/Release+Asserts/lib/clang" | head -1)
    tar xf "$CLANG_TARBALL" -C "v8/third_party/llvm-build/Release+Asserts/" \
        "lib/clang/$CLANG_MAJOR/lib/linux/"
    rm -f "$CLANG_TARBALL"
fi

checkpoint "Done -- now run build_v8_source.sh --v8-dir $V8_PARENT/v8"
