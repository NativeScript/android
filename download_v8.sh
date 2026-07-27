#!/bin/bash
set -euo pipefail
#
# Installs the prebuilt V8 libraries and headers for the pinned release.
#
# The artifacts are built by NativeScript/v8-buildscripts and published as
# GitHub release assets; they are not committed here because two of the four
# monoliths exceed GitHub's 100 MiB per-file push limit, and because the full
# matrix cannot be produced on any single machine.
#
# Deliberately a standalone script rather than a Gradle task: it is a
# prerequisite you run once, trivial to skip when you already have the
# artifacts, and easy to override with a local V8 build.
#
# Set V8_SKIP_DOWNLOAD=1 to make it a no-op -- use that when you have built V8
# yourself and do not want a pinned release overwriting it.
#
# Usage: download_v8.sh [--release <tag>] [--abi <abi>]... [--force]
#

REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
UPSTREAM="NativeScript/v8-buildscripts"
RELEASE_FILE="$REPO_ROOT/V8_RELEASE"
CACHE_DIR="${V8_PREBUILT_CACHE:-$REPO_ROOT/.v8-prebuilt}"

CPP_DIR="$REPO_ROOT/test-app/runtime/src/main/cpp"
LIBS_DIR="$REPO_ROOT/test-app/runtime/src/main/libs"
STAMP="$LIBS_DIR/.v8-release-stamp"

RELEASE=""
FORCE=0
ABIS=()

usage() {
    cat <<EOF
Usage: $(basename "$0") [--release <tag>] [--abi <abi>]... [--force]

  --release <tag>  Release to install (default: contents of V8_RELEASE)
  --abi <abi>      Repeatable. armeabi-v7a, arm64-v8a, x86, x86_64
                   (default: all four)
  --force          Reinstall even if the pinned release is already in place

Downloads are cached in $CACHE_DIR (override with \$V8_PREBUILT_CACHE).
EOF
}

while [ $# -gt 0 ]; do
    case "$1" in
        --release)   RELEASE="$2"; shift 2 ;;
        --release=*) RELEASE="${1#*=}"; shift ;;
        --abi)       ABIS+=("$2"); shift 2 ;;
        --abi=*)     ABIS+=("${1#*=}"); shift ;;
        --force)     FORCE=1; shift ;;
        -h|--help)   usage; exit 0 ;;
        *)           echo "Unknown argument: $1" >&2; usage >&2; exit 1 ;;
    esac
done

if [ "${V8_SKIP_DOWNLOAD:-0}" != "0" ]; then
    echo "V8_SKIP_DOWNLOAD is set; leaving the libraries and headers alone."
    exit 0
fi

if [ -z "$RELEASE" ]; then
    [ -f "$RELEASE_FILE" ] || { echo "Missing $RELEASE_FILE" >&2; exit 1; }
    RELEASE="$(tr -d '[:space:]' < "$RELEASE_FILE")"
fi
[ ${#ABIS[@]} -gt 0 ] || ABIS=(arm64-v8a armeabi-v7a x86_64 x86)

if [ "$FORCE" = "0" ] && [ -f "$STAMP" ] && [ "$(cat "$STAMP")" = "$RELEASE" ]; then
    echo "V8 $RELEASE already installed. Use --force to reinstall."
    exit 0
fi

BASE_URL="https://github.com/$UPSTREAM/releases/download/$RELEASE"
DL="$CACHE_DIR/$RELEASE"
mkdir -p "$DL"

fetch() {
    local name="$1"
    if [ -f "$DL/$name" ]; then return 0; fi
    echo "  downloading $name"
    curl -fSL --retry 3 -o "$DL/$name.part" "$BASE_URL/$name"
    mv "$DL/$name.part" "$DL/$name"
}

echo "Installing V8 $RELEASE from $UPSTREAM"
fetch SHA256SUMS

ASSETS=()
for ABI in "${ABIS[@]}"; do
    ASSETS+=("$(grep -oE "v8-[^ ]*-android-$ABI\.tar\.gz" "$DL/SHA256SUMS" | head -1)")
done
ASSETS+=("$(grep -oE 'v8-[^ ]*-src-headers\.tar\.gz' "$DL/SHA256SUMS" | head -1)")

for a in "${ASSETS[@]}"; do
    [ -n "$a" ] || { echo "Release $RELEASE is missing an expected asset." >&2; exit 1; }
    fetch "$a"
done

# Verify before unpacking anything. A release is only trustworthy because the
# archive matches the checksum published with it.
#
# Linux has sha256sum, macOS has shasum; neither has both reliably.
if command -v sha256sum > /dev/null 2>&1; then
    SHA256_CHECK="sha256sum -c -"
else
    SHA256_CHECK="shasum -a 256 -c -"
fi
echo "Verifying checksums"
( cd "$DL" && grep -E "$(printf '%s|' "${ASSETS[@]}" | sed 's/|$//')" SHA256SUMS | $SHA256_CHECK ) \
    || { echo "Checksum verification FAILED for $RELEASE" >&2; exit 1; }

STAGE="$(mktemp -d)"
trap 'rm -rf "$STAGE"' EXIT
for a in "${ASSETS[@]}"; do tar -xzf "$DL/$a" -C "$STAGE"; done

echo "Installing libraries"
for ABI in "${ABIS[@]}"; do
    src="$STAGE/android-$ABI/lib/libv8_monolith.a"
    [ -f "$src" ] || { echo "Archive for $ABI has no libv8_monolith.a" >&2; exit 1; }
    mkdir -p "$LIBS_DIR/$ABI"
    cp "$src" "$LIBS_DIR/$ABI/libv8_monolith.a"
done

echo "Installing public headers"
# zip.h/zipconf.h belong to libzip and live in the same directory, so the V8
# headers are replaced selectively rather than by wiping include/.
FIRST_ABI="${ABIS[0]}"
SRC_INC="$STAGE/android-$FIRST_ABI/include"
[ -d "$SRC_INC" ] || { echo "Archive has no include/" >&2; exit 1; }
for entry in cppgc libplatform inspector; do
    rm -rf "${CPP_DIR:?}/include/$entry"
done
find "$CPP_DIR/include" -maxdepth 1 -type f \
    ! -name 'zip.h' ! -name 'zipconf.h' -delete
cp -R "$SRC_INC/." "$CPP_DIR/include/"

echo "Vendoring the inspector's V8 internals"
# The closure is computed here rather than shipped, because what the glue
# includes is this repo's business, not the build repo's.
python3 "$REPO_ROOT/tools/v8/vendor_inspector_sources.py" \
    --v8-dir "$STAGE/src-headers" \
    --gen-dir "$STAGE/src-headers" \
    --dest "$CPP_DIR/v8_inspector"

echo "$RELEASE" > "$STAMP"
echo "Installed V8 $RELEASE"
