#!/usr/bin/env python3
"""Re-vendor the V8 internal headers that the inspector glue compiles against.

JsV8InspectorClient and ns-v8-tracing-agent-impl reach into src/inspector, which
is not part of V8's public API, so a copy of those headers (and everything they
pull in) lives under test-app/runtime/src/main/cpp/v8_inspector. Keeping that
copy pinned to an older V8 than libv8_monolith.a is an ABI mismatch, so it has
to be refreshed with every version bump.

Usage: vendor_inspector_sources.py --v8-dir <path> --gen-dir <path> [--dest <path>]
"""
import argparse
import os
import re
import shutil

# What the runtime includes directly; everything else is reached transitively.
ROOTS = [
    "src/inspector/v8-inspector-impl.h",
    "src/inspector/v8-runtime-agent-impl.h",
    "src/inspector/v8-stack-trace-impl.h",
    "src/inspector/v8-console-message.h",
    "src/inspector/v8-inspector-session-impl.h",
    "src/inspector/string-util.h",
]

INCLUDE_RE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.MULTILINE)

# Prefixes we vendor. Anything else (v8-inspector.h, libplatform, libc++, NDK
# headers) is already reachable through include/ or the toolchain.
#
# absl is here because src/common/globals.h reaches it through
# base/numbers/double.h -> diy-fp.h. It lands at the top of the destination so
# that the "absl/..." spelling resolves against the v8_inspector include dir.
VENDORED_PREFIXES = ("src/", "third_party/inspector_protocol/", "absl/")

ABSL_SOURCE_PREFIX = "third_party/abseil-cpp/"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--v8-dir", required=True)
    ap.add_argument("--gen-dir", required=True,
                    help="out.gn/<config>/gen, for the generated protocol sources")
    ap.add_argument("--dest", default=os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        "../../test-app/runtime/src/main/cpp/v8_inspector"))
    args = ap.parse_args()

    v8_dir = os.path.abspath(args.v8_dir)
    gen_dir = os.path.abspath(args.gen_dir)
    dest = os.path.abspath(args.dest)

    def resolve(rel):
        candidates = [rel]
        if rel.startswith("absl/"):
            candidates.append(ABSL_SOURCE_PREFIX + rel)
        for base in (v8_dir, gen_dir):
            for candidate in candidates:
                path = os.path.join(base, candidate)
                if os.path.isfile(path):
                    return path
        return None

    seen, queue, missing = set(), list(ROOTS), []
    while queue:
        rel = queue.pop()
        if rel in seen:
            continue
        src = resolve(rel)
        if src is None:
            missing.append(rel)
            continue
        seen.add(rel)
        text = open(src, encoding="utf-8", errors="replace").read()
        for inc in INCLUDE_RE.findall(text):
            if inc.startswith(VENDORED_PREFIXES):
                queue.append(inc)
                continue
            # The crdtp headers include their siblings by bare name.
            sibling = os.path.normpath(os.path.join(os.path.dirname(rel), inc))
            if resolve(sibling) is not None:
                queue.append(sibling)

    # Only the directories we own -- ns-v8-tracing-agent-impl.* and Utils.* live
    # at the top level of the destination and must survive.
    for sub in ("src", "third_party", "absl"):
        shutil.rmtree(os.path.join(dest, sub), ignore_errors=True)

    for rel in sorted(seen):
        out = os.path.join(dest, rel)
        os.makedirs(os.path.dirname(out), exist_ok=True)
        shutil.copyfile(resolve(rel), out)

    print(f"vendored {len(seen)} files into {dest}")
    if missing:
        # A partial tree still compiles against the wrong V8, so this has to be
        # loud. Usually it means --gen-dir points at a build that has not
        # generated the inspector protocol sources yet.
        raise SystemExit("unresolved includes:\n  " +
                         "\n  ".join(sorted(set(missing))))


if __name__ == "__main__":
    main()
