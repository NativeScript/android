#!/bin/bash
set -e
#
# Builds the 32-bit Android ABIs (armeabi-v7a, x86) in a linux/amd64 container.
#
# These cannot be built on macOS: mksnapshot has to run V8's simulator for a
# 32-bit target, and v8config.h only permits that from an ia32 host ("Target
# architecture arm is only supported on arm and ia32 host"). A linux/amd64 host
# builds mksnapshot as a 32-bit x86 binary, which is the supported path.
#
# The V8 checkout lives in a named docker volume so gclient sync is not paying
# bind-mount overhead for millions of files; only this repo is bind-mounted, so
# the resulting libraries land straight in test-app/runtime/src/main/libs/.
#
# Usage: build_v8_linux_docker.sh [--abi <abi>]... [--volume <name>]
#

REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
VOLUME="v8linux"
ABIS=()

usage() {
    cat <<EOF
Usage: $(basename "$0") [--abi <abi>]... [--volume <name>]

  --abi <abi>      Repeatable. armeabi-v7a and/or x86 (default: both).
  --volume <name>  Docker volume holding the V8 checkout (default: $VOLUME).
                   Reused across runs so a rebuild does not re-sync.
EOF
}

while [ $# -gt 0 ]; do
    case "$1" in
        --abi)      ABIS+=("$2"); shift 2 ;;
        --abi=*)    ABIS+=("${1#*=}"); shift ;;
        --volume)   VOLUME="$2"; shift 2 ;;
        --volume=*) VOLUME="${1#*=}"; shift ;;
        -h|--help)  usage; exit 0 ;;
        *)          echo "Unknown argument: $1" >&2; usage >&2; exit 1 ;;
    esac
done

if [ ${#ABIS[@]} -eq 0 ]; then
    ABIS=(armeabi-v7a x86)
fi

ABI_ARGS=""
for ABI in "${ABIS[@]}"; do
    case "$ABI" in
        armeabi-v7a|x86) ABI_ARGS="$ABI_ARGS --abi $ABI" ;;
        *) echo "This script only builds the 32-bit ABIs; got '$ABI'." >&2; exit 1 ;;
    esac
done

# Keep in step with test-app/runtime/build.gradle's defaultNdkVersion -- V8 and
# the runtime must be built against the same NDK, see docs/V8_14_MIGRATION.md.
NDK_RELEASE="r29"

docker volume create "$VOLUME" > /dev/null

exec docker run --rm -i --platform linux/amd64 \
    -v "$VOLUME:/v8" \
    -v "$REPO_ROOT:/repo" \
    -e "ABI_ARGS=$ABI_ARGS" \
    -e "NDK_RELEASE=$NDK_RELEASE" \
    debian:bookworm bash -s <<'INNER'
set -e

echo "### installing host packages"
export DEBIAN_FRONTEND=noninteractive
apt-get update -qq
apt-get install -y -qq --no-install-recommends \
    git python3 python3-setuptools curl ca-certificates xz-utils zip unzip \
    rsync file lsb-release build-essential pkg-config > /dev/null

if [ ! -d /depot_tools ]; then
    echo "### fetching depot_tools"
    git clone -q --depth 1 \
        https://chromium.googlesource.com/chromium/tools/depot_tools.git /depot_tools
fi
export PATH="/depot_tools:$PATH"
export DEPOT_TOOLS_UPDATE=1

if [ ! -d "/opt/android-ndk-$NDK_RELEASE" ]; then
    echo "### fetching Android NDK $NDK_RELEASE"
    curl -fsSL -o /tmp/ndk.zip \
        "https://dl.google.com/android/repository/android-ndk-$NDK_RELEASE-linux.zip"
    unzip -q /tmp/ndk.zip -d /opt
    rm -f /tmp/ndk.zip
fi

echo "### syncing V8"
bash /repo/tools/v8/fetch_v8.sh --v8-dir /v8

echo "### building"
bash /repo/tools/v8/build_v8_source.sh \
    --v8-dir /v8/v8 \
    --ndk-root "/opt/android-ndk-$NDK_RELEASE" \
    $ABI_ARGS

echo "### done"
ls -la /repo/test-app/runtime/src/main/libs/*/libv8_monolith.a
INNER
