# `ns:` builtin modules

Cross-runtime contract for exposing runtime-provided modules to application
code. This document is the specification both the iOS and Android runtimes
implement; a capability must behave identically on both platforms before it
ships in a stable release.

## The scheme

Builtin modules live under the URL-style `ns:` scheme, mirroring Node's
`node:` prefix:

```js
// CommonJS
const util = require("ns:util");

// ES modules
import util, { inspect } from "ns:util";
const util2 = await import("ns:util");
```

Rules:

- `ns:` specifiers are resolved by the runtime **before any filesystem or
  npm resolution**. They can never be shadowed by a file, a path mapping, or
  a package — and conversely, a file named `ns:util` is not reachable.
- Resolution of an unknown builtin fails synchronously with an `Error` whose
  message is exactly `No such built-in module: ns:<name>` (matching Node's
  wording for familiarity).
- A builtin module is a **singleton per JS realm** (main context and each
  worker get their own instance). `require("ns:util")` twice returns the same
  object; the CJS exports object and the ESM namespace expose the same
  underlying values (ESM additionally provides the exports object as
  `default`).
- There is no bare-specifier fallback: `require("util")` is **not** an alias
  for `require("ns:util")`. The unprefixed name continues to resolve to npm
  packages as it always has.
- Builtin exports are frozen. Apps patch behavior by wrapping, not by
  mutating the runtime's module.

## Modules

### `ns:util` (v1)

| export | description |
|---|---|
| `inspect(value[, options])` | Formats any value for human consumption: depth-limited, output-capped, cycle-safe, never invokes getters (except a guarded `error.stack` read and custom `toString` overrides, which are honored). `options.depth` (number) overrides the default depth of 2. Other option keys are reserved. |
| `format(fmt, ...args)` | Node-style printf formatting: `%s`, `%d`, `%i`, `%f`, `%j`, `%o`, `%O`, `%%`. Extra arguments are appended space-separated, objects rendered via `inspect`. When `fmt` is not a string or contains no substitutions, all arguments are formatted and joined with spaces. `console.*` routes its arguments through this, so `console.log("%d apples", 3)` works. |

**Stability caveat (verbatim from Node's contract):** the output of `inspect`
(and therefore `format`'s object rendering) may change between runtime
versions for readability; it is intended for humans and must not be parsed
programmatically.

## `node:` compatibility shims

The same registry serves the `node:` scheme with **compatibility shims** so
npm packages that require Node builtins by their prefixed names can run
unmodified where a shim exists:

- A shim implements a documented **subset** of the corresponding Node module's
  API, backed by `ns:` modules. Unimplemented members are simply absent
  (so `typeof util.promisify === "function"` feature-checks behave
  correctly); they are never present-but-throwing.
- **One source file per specifier.** A shim is its own module that consumes
  the `ns:` module it adapts through the internal require, and it owns *all*
  the adaptation — argument shapes, option names, aliases, anything that has
  to track Node. A standard `ns:` module never contains compatibility code
  and never knows a shim exists.
- Shims are **lazy**: a shim's source is only evaluated when its specifier is
  first resolved, so an app that never touches the `node:` scheme never pays
  for one.
- `node:` modules with no shim fail with the same error shape:
  `No such built-in module: node:<name>`.
- **Bare specifiers are untouched**: `require("util")` resolves through npm
  as it always has (many apps bundle the `util` polyfill package). Only the
  explicit `node:` prefix reaches the shim registry, so existing apps cannot
  break. Bundler-level aliases (webpack/rollup) continue to work and take
  precedence at build time.
- A shim is always a **distinct module object** from any `ns:` module, even
  when every member is re-exported unchanged. `ns:` modules may grow runtime-specific
  members freely; a `node:` shim only ever gains members that track Node's
  actual API. This mirrors how Bun (`bun:*`), Deno (`Deno.*`/JSR) and
  Cloudflare (`cloudflare:*`) all keep their own surface strictly apart from
  their `node:` compat layer.
- Shims ship on both runtimes under the same parity rule as `ns:` modules.

### v1 shims

| module | exports | notes |
|---|---|---|
| `node:util` | `inspect`, `format` | Re-exports `ns:util`'s members unchanged (`nodeUtil.inspect === nsUtil.inspect`) from a **distinct, separately frozen module object**. Documented as partial. |

Candidates for future shims, in rough order of ecosystem demand:
`node:events` (EventEmitter), `node:path` (pure JS), `node:buffer`,
`node:process` (subset). Each requires a spec update here first.

## The internal require

Builtin modules reach each other — and only each other — through an internal
`require` the runtime provides to every builtin source:

- It resolves **builtin specifiers only**. A path, a package name or any other
  specifier is not reachable from a builtin; an unregistered builtin name
  throws the same `No such built-in module: <specifier>` an app sees.
- It materializes the target module on first use and returns the realm's
  singleton afterwards, which is what makes shims lazy.
- Requiring a module that is still being built throws rather than recursing,
  so a dependency cycle between builtins is a loud error and not a hang.

This is the mechanism shims are built on, so it is normative: both runtimes
provide it.

## Adding a builtin module

- The name is a short, lowercase identifier (`ns:util`, `ns:timers`, ...).
- One module, one source file, one registry entry — including shims.
- New modules and new exports require this document to be updated first and
  an implementation on both runtimes before a stable release; a module may
  ship on one platform behind a documented "experimental, iOS-only" (or
  Android-only) note in between.
- Internal runtime machinery must never be reachable through the scheme:
  the registry distinguishes public modules from internal builtins, and only
  public ones resolve (Node's `canBeRequiredByUsers` split).

## Source-text modules: deliberately not supported

Builtins are classic function bodies, not ES modules, on both runtimes. If
cross-builtin code sharing is ever needed, the first answer is bundling at
generation time (author as ESM, emit function bodies); runtime source-text
builtin modules (Node's `kSourceTextModule`) are justified only by a concrete
need for live module semantics (TLA, live bindings, cyclic imports), which no
current or planned builtin has. Revisit here before building either.

## iOS implementation notes (non-normative)

Builtin modules are function-body builtins (`NativeScript/runtime/js/`,
see the README there) compiled via the RuntimeBuiltins table. The `ns:`
resolver intercepts specifiers in the CommonJS require path and in the ES
module resolve/dynamic-import callbacks; ESM consumption is served by a
synthetic module whose exports are populated from the same per-realm exports
object. The internal require is a fixed parameter of the builtin function
wrapper (`exports`, `require`, `module`, `binding`, `primordials`).

iOS also keeps a pre-registry `node:url` polyfill (`fileURLToPath`,
`pathToFileURL`) that predates this document. It is compiled from module
source inside the ES module resolver and is therefore reachable through
`import` only, not through `require()`.

## Android implementation notes (non-normative)

Builtin modules are function-body builtins
(`test-app/runtime/src/main/cpp/js/`, see the README there) compiled via the
RuntimeBuiltins table. The registry lives in
`test-app/runtime/src/main/cpp/NsBuiltinModules.{h,cpp}` and intercepts
specifiers in the CommonJS require path (`ModuleInternal::RequireCallbackImpl`)
and in the ES module resolve and dynamic-import callbacks
(`ModuleInternalCallbacks.cpp`); ESM consumption is served by a synthetic
module whose exports are populated from the same per-realm exports object. The
internal require is a fixed parameter of the builtin function wrapper
(`exports`, `require`, `module`, `binding`, `primordials`).

Android has no `Caches` class, so every per-realm cache — exports objects,
synthetic modules, the in-progress set, the cached `format` and the builtin
`require` — lives in an isolate-keyed map released from `disposeIsolate`.
The ES module registry app modules land in (`g_moduleRegistry`) is
process-global and shared by every isolate; the builtin caches deliberately do
not use it, so workers get their own instances as the spec requires.

Android also keeps three pre-registry `node:` polyfills that predate this
document: `node:url` (`fileURLToPath`, `pathToFileURL`), `node:module`
(`createRequire`) and `node:path` (`sep`, `delimiter`, `basename`, `dirname`,
`extname`, `join`, `resolve`, `isAbsolute`). They are compiled from module
source inside the ES module resolver and are therefore reachable through
`import` only; `require("node:path")` reaches the registry and fails with the
not-found message.

There is deliberately no `node:fs` polyfill and no catch-all for unshimmed
`node:` names: a stub whose members throw on use violates the
absent-not-present-but-throwing rule above, and an empty-default fallback lets
an import that cannot work succeed.
