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
CONTAINER="v8-android-32bit"
ABIS=()
ATTACH=0

usage() {
    cat <<EOF
Usage: $(basename "$0") [--abi <abi>]... [--volume <name>]

  --abi <abi>      Repeatable. armeabi-v7a and/or x86 (default: both).
  --volume <name>  Docker volume holding the V8 checkout (default: $VOLUME).
                   Reused across runs so a rebuild does not re-sync.
  --attach         Do not start anything; just follow the logs of a build that
                   is already running.
EOF
}

while [ $# -gt 0 ]; do
    case "$1" in
        --abi)      ABIS+=("$2"); shift 2 ;;
        --abi=*)    ABIS+=("${1#*=}"); shift ;;
        --volume)   VOLUME="$2"; shift 2 ;;
        --volume=*) VOLUME="${1#*=}"; shift ;;
        --attach)   ATTACH=1; shift ;;
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

if [ "$ATTACH" = "1" ]; then
    exec docker logs -f "$CONTAINER"
fi

docker volume create "$VOLUME" > /dev/null
docker rm -f "$CONTAINER" > /dev/null 2>&1 || true

# Detached on purpose: the build runs for hours, and with stdin attached the
# container dies whenever the invoking shell does. Logs are followed separately
# below, so interrupting this script leaves the build running -- re-run it with
# --attach to pick the stream back up.
SCRIPT_TMP="${TMPDIR:-/tmp}/v8-android-32bit-build.sh"
cat > "$SCRIPT_TMP" <<'INNER'
set -e

echo "### installing host packages"
export DEBIAN_FRONTEND=noninteractive
# i386 multiarch is required, not optional: building a 32-bit Android target
# makes mksnapshot and the bytecode-builtins generator 32-bit x86 host binaries,
# and without the i386 runtime they link fine and then fail to execute.
dpkg --add-architecture i386
apt-get update -qq
apt-get install -y -qq --no-install-recommends \
    git python3 python3-setuptools curl ca-certificates xz-utils zip unzip \
    rsync file lsb-release build-essential pkg-config \
    libc6:i386 libstdc++6:i386 libatomic1:i386 zlib1g:i386 > /dev/null

if [ ! -d /v8/depot_tools ]; then
    echo "### fetching depot_tools"
    git clone -q --depth 1 \
        https://chromium.googlesource.com/chromium/tools/depot_tools.git /v8/depot_tools
fi
export PATH="/v8/depot_tools:$PATH"
export DEPOT_TOOLS_UPDATE=1

if [ ! -d "/v8/android-ndk-$NDK_RELEASE" ]; then
    echo "### fetching Android NDK $NDK_RELEASE"
    curl -fsSL -o /tmp/ndk.zip \
        "https://dl.google.com/android/repository/android-ndk-$NDK_RELEASE-linux.zip"
    unzip -q /tmp/ndk.zip -d /v8
    rm -f /tmp/ndk.zip
fi

echo "### syncing V8"
bash /repo/tools/v8/fetch_v8.sh --v8-dir /v8

echo "### building"
bash /repo/tools/v8/build_v8_source.sh \
    --v8-dir /v8/v8 \
    --ndk-root "/v8/android-ndk-$NDK_RELEASE" \
    $ABI_ARGS

echo "### done"
ls -la /repo/test-app/runtime/src/main/libs/*/libv8_monolith.a
INNER

docker run -d --name "$CONTAINER" --platform linux/amd64 \
    -v "$VOLUME:/v8" \
    -v "$REPO_ROOT:/repo" \
    -v "$SCRIPT_TMP:/build.sh:ro" \
    -e "ABI_ARGS=$ABI_ARGS" \
    -e "NDK_RELEASE=$NDK_RELEASE" \
    debian:bookworm bash /build.sh > /dev/null

echo "Started container '$CONTAINER'. Following logs; ^C leaves it running."
echo "Reattach with:  docker logs -f $CONTAINER"
docker logs -f "$CONTAINER"
