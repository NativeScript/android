# TextEncoder / TextDecoder and atob / btoa

Native, WHATWG-conformant `TextEncoder`, `TextDecoder`
([Encoding Standard](https://encoding.spec.whatwg.org)) and `atob` / `btoa`
([HTML Standard §8.3](https://html.spec.whatwg.org/multipage/webappapis.html#atob))
globals, and the **lazy-global tier** they ride on.

## Lazy globals

These globals are registered on the global template as lazy data properties
(`LazyGlobals`, `test-app/runtime/src/main/cpp/LazyGlobals.cpp`): the builtin
behind a name is not compiled, run, or allocated until app code first reads it,
and V8 then replaces the property with a plain data property so later reads
cost nothing. Sibling names from one builtin (`TextEncoder` + `TextDecoder`)
share a single run per isolate. Workers get the same globals — the tier is
registered in every isolate's template. Assigning over one of these names
before its first read replaces the global, like any other writable global.

The tier is the intended home for further web globals (`Blob`, `fetch`,
`crypto`, `DOMException`, …) with zero cost when unused; see
`test-app/runtime/src/main/cpp/js/README.md` for the rules a lazy builtin
lives by.

The per-isolate exports cache behind the tier (`BuiltinLoader::GetExports`) is
shared with the `ns:`/`node:` module registry: `require("ns:util").TextDecoder`
and `require("node:util").TextDecoder` are the very class objects the globals
hold, whichever entry point is reached first
(see [ns-builtin-modules](ns-builtin-modules.md)).

## TextEncoder / TextDecoder

Node's split: `js/text-encoding.js` owns the WebIDL surface (brand checks via
private fields, enumerable prototype members, `Symbol.toStringTag`),
`TextEncoding.cpp` owns the bytes.

- **Decoder encodings**: the `TextDecoder` constructor resolves utf-8,
  utf-16le, utf-16be and windows-1252, each with its complete WHATWG label
  set; an unknown label throws `RangeError`. (Precedent: Node without ICU
  ships utf-8/utf-16le; utf-16be and windows-1252 are cheap, and windows-1252
  covers the `ascii`/`latin1`/`iso-8859-1` aliases web code actually uses.)
  `TextEncoder` is UTF-8-only and takes no label, as the spec defines it.
- **Streaming**: full `decode(…, { stream: true })` support. Incomplete
  sequences (split BOMs and split utf-16 code units included) carry across
  calls in a 16-byte `Uint8Array` the builtin owns — no per-instance native
  handle, no finalizer.
- **Replacement semantics**: WHATWG utf-8 state machine with one U+FFFD per
  maximal invalid subpart; `fatal: true` throws `TypeError`; `ignoreBOM`
  honored.
- `encode()` / `encodeInto()` with correct USV conversion and partial-write
  boundaries (`encodeInto` never splits an encoded code point).
- **Fast paths**: pure-ASCII utf-8 and C1-free windows-1252 decode straight
  through `String::NewFromOneByte`; results downgrade to one-byte strings when
  possible. `encodeInto` registers a V8 Fast API overload
  (`NATIVESCRIPT_ENABLE_FAST_API`, default on), live once a call site tiers
  up.

## atob / btoa

WHATWG forgiving-base64 (`Base64.cpp`): whitespace stripping, padding rules,
alphabet validation. With no `DOMException` in the runtime yet, failures throw
the name-patched `Error` (`InvalidCharacterError`) stand-in the abort-signal
and performance builtins already use; a follow-up will introduce
`DOMException` and upgrade these.

## Tests

The shared suite (`test-app/app/src/main/assets/app/shared/TextEncoding`)
holds the conformance specs, feature-detecting so runtimes without these
globals report pending rather than failing; it was independently validated
against Node 24 (full ICU) as a reference.
