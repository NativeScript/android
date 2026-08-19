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

### `ns:runtime` (v1)

Runtime-level configuration. Keys, value domains, and scope are defined and
validated natively; the module surface is a thin frozen wrapper.

| export | description |
|---|---|
| `setConfig(key, value)` | Sets a runtime config key. Throws `TypeError` on an unknown key, an invalid value, or (for process-wide keys) when called from a worker isolate. |
| `getConfig(key)` | Returns the current value of a config key. Throws `TypeError` on an unknown key. Readable from any isolate. |

Config keys:

| key | values | scope | default |
|---|---|---|---|
| `debug` | comma-separated category list, e.g. `"esm,fetch"` | process-wide (main-isolate writes only; read live by every isolate) | the `NS_DEBUG` environment variable, or `""` |

Remote-module security (`security.allowRemoteModules`,
`security.remoteModuleAllowlist`) is **not** part of this surface. Those
values are read once from nativescript.config / package.json the first time
the HTTP loader gates a fetch, and they cannot be inspected or changed
through `getConfig` / `setConfig`.

iOS additionally registers `releasedObjectPolicy`; Android does not, because it
has no released-native-counterpart machinery for that key to govern.

`debug` turns on the runtime's category-scoped trace logs. Categories:

| category | covers |
|---|---|
| `esm` | module resolution, compilation, linking, evaluation, registry keying |
| `fetch` | the HTTP module transport (one line per fetched URL — high volume) |
| `registry` | registry invalidation and dynamic-import cache bookkeeping |

Each write replaces the whole set, so `setConfig('debug', '')` disables
tracing and no caller needs to know what was already on. `getConfig('debug')`
returns the canonical comma-separated list of what is enabled. Unknown names
are ignored, with one warning line naming the valid ones.

The same list can be given before boot as the `NS_DEBUG` environment variable
(`NS_DEBUG=esm,fetch`), which is the only way to trace boot itself. Traces are
compiled into release builds as well: a release build that cannot be traced is
a release build that cannot be diagnosed. Each category writes under its own
logcat tag — `TNS.esm`, `TNS.fetch`, `TNS.registry` — so `adb logcat -s TNS.esm`
can filter them without matching message text.

### `ns:module` (v1)

The module-loader control surface consumed by development tooling
(`@nativescript/vite`). Mechanism only: every policy concern (boot
orchestration, `import.meta.hot`, full reload, CSS apply, worker teardown,
WebSocket protocol) lives in the tooling. See `HMR_RUNTIME_BOUNDARY.md` for
the full contract rationale.

| export | description |
|---|---|
| `configureLoader(config)` | Install loader policy before the session imports anything: `importMap` (`imports` + `scopes`, consulted inside the synchronous resolver — see below), `volatilePatterns` (URL substrings always re-fetched), `canonicalization` (`stripParams`/`forPathPrefixes`/`preserveQueryFor` vocabulary for registry keying). Each present section replaces its state wholesale. An invalid `importMap` throws a `TypeError` and leaves the previously installed map untouched. **Configures the calling isolate.** A worker inherits a copy of its parent's vocabulary taken at spawn, so a worker started after `configureLoader` resolves through it; a worker already running does **not** see a later reconfiguration — the dev client restarts workers when the vocabulary changes. |
| `invalidateModules(urls)` | Evict the given URLs (canonicalized) from the module registry and mark them bust-next-fetch, so the next network fetch bypasses every HTTP cache layer. |
| `getLoadedModuleUrls()` | URL-like keys currently in the module registry (used to compute full-reload eviction sets). |
| `createRequire(filenameOrURL)` | A `require` resolving against `filenameOrURL`'s directory (a trailing slash names the directory itself). Accepts an absolute path string, a `file:` URL string, or a URL object; anything else throws a `TypeError`, and an `http(s)` base is refused outright because `require()` of a dev-served module is not supported — import those. ES module graphs load under Node's `require(esm)` rule: a graph containing top-level await is refused before it evaluates. |
| `createPumpingRequire(filenameOrURL, options?)` | Same argument contract and same resolution, but an ES module graph with top-level await is evaluated by driving V8's nestable tasks and microtasks until it settles, instead of being refused. **Callable only from a task context** — see below. `options` (validated at mint time; unknown keys throw `TypeError`): `deadlineSeconds` (positive finite, default 60), `onTimeout` (`"throw"` default, or `"return-pending"`), `pumpRunLoop` (default `false`). They govern the evaluation-settle phase only — the graph walk's fetch deadline is separate. Passing `options` to `createRequire` throws. |

`createPumpingRequire` pumps the loop, and the loop cannot be pumped
re-entrantly: V8 ignores a microtask checkpoint while the isolate is already
draining the microtask queue. A top-level await resumes through a promise
reaction — a microtask — so such a graph can never settle from inside a
microtask turn. Requiring one from after an `await` or inside a `.then`
callback therefore throws immediately, before evaluation, leaving the graph
instantiated so `import()` can still load it. Call it from a task context
instead — a native boundary, an event handler, a timer callback, or module
evaluation itself. A **synchronous** graph needs no pumping and stays legal
from anywhere, microtask turns included.

### Import maps and scopes

`importMap` takes the WHATWG shape:

```js
configureLoader({
  importMap: {
    imports: { "lodash": "http://host/vendor/lodash.mjs", "@scope/pkg/": "http://host/pkg/" },
    scopes: { "http://host/legacy/": { "lodash": "http://host/vendor/lodash-3.mjs" } },
  },
});
```

Within any one section, a specifier matches exactly first, then against the
longest trailing-slash key, whose remainder is appended to the target. A key
ending in `/` must have a target ending in `/`.

A **scope key is matched as a plain prefix of the importing module's canonical
registry key** — an absolute `http(s)` URL for a served module, or a canonical
absolute path for a file on disk. That key is this runtime's analogue of the
web's resolved referrer URL, which is what scope prefixes match in a browser.
End a scope key with `/` to keep it on a directory boundary. Resolution
consults the most specific matching scope first, then progressively less
specific ones, then `imports` — so a scope can override a global mapping for
one subtree and fall through to it everywhere else. The resolver, the graph
walk, and `import()` all resolve through the same cascade.

The vocabulary is per-isolate: `configureLoader` writes the isolate that
calls it, and nothing is shared between isolates, so no lock guards it. A
worker receives a copy captured on the parent's thread while it spawns and
installed before the worker loads its first module. That copy is a snapshot —
reconfiguring the parent afterwards leaves running workers on the vocabulary
they started with, which is why the dev client restarts workers on an update.

The whole map is parsed and validated before any of it is installed. A
malformed map, a non-string target, an unknown top-level section, or a
trailing-slash key with a non-trailing-slash target throws a `TypeError`
naming the offending key or section, and the previously installed map keeps
resolving — a typo in an update cannot empty a live session's vocabulary.

#### Booting an ESM entry from a CJS bootstrap

The supported way to give an ESM app its loader vocabulary before any ESM
traffic — which closes the pre-configure window described in the
canonicalization notes — is a small CommonJS bootstrap as the app entry:

```js
const { configureLoader, createPumpingRequire } = require("ns:module");

configureLoader({ importMap: { imports: { /* … */ } } });

createPumpingRequire(__filename, {
  pumpRunLoop: true,
  onTimeout: "return-pending",
  deadlineSeconds: 1,
})("./entry.mjs");
```

Two warnings, both load-bearing:

- `pumpRunLoop: true` is sane **only while boot owns the looper**. After boot
  the looper belongs to the app, and slicing it from inside a require
  re-enters arbitrary looper sources — including UI callbacks — underneath JS
  frames.
- With `onTimeout: "return-pending"` the returned namespace may still be
  evaluating. A bootstrap must **discard it** and never read a binding off it;
  reading one is a TDZ error at best.

The bootstrap is not the only option: **an ES module main entry is supported
directly**, top-level await included. When the app's `main` resolves to a
`.mjs`, the entry is evaluated as a module rather than `require()`d — so
`import`/`export` are legal there — under the boot evaluation options: a one
second in-place yield that never throws, after which the boot backstop holds
the process while the entry's evaluation promise is still pending, bounded at
twice the module deadline. The trade-off is that the entry's own static imports
resolve *before* its body runs, so anything the entry needs `configureLoader`
to have configured must be reached through a dynamic `import()` after that
call. The CommonJS bootstrap above avoids that constraint by being synchronous;
pick whichever fits the app.

Not implemented on either require: `require.resolve`, `require.cache`, and
`require.main`. They are absent rather than throwing, so a feature check
works; adding them is a spec change here first.

Debug builds additionally carry `canonicalizeHttpUrlKey(url)`, a pure test
diagnostic; release builds omit it. Missing members are simply absent —
never present-but-throwing — so feature checks work. The module is
registered in every build; the security boundary for remote module loading
sits at the network layer (`security.allowRemoteModules` in
nativescript.config, enforced inside `HttpLoader`), not the module
registry and not `ns:runtime` getConfig/setConfig.

Note: `ns:module` (loader policy, structured, boot-time) is deliberately
separate from `ns:runtime` (live key-value runtime flags, `setConfig`/
`getConfig`).

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
| `node:url` | `fileURLToPath`, `pathToFileURL` | Node-strict converters between `file:` URLs and paths. Parsing goes through the URL intrinsic, so `file://localhost/x` is accepted (the URL spec folds a `localhost` authority to none) while any other host throws, and the query and fragment are not part of the path. `fileURLToPath` rejects a non-`file:` scheme and rejects `%2F` in the path rather than decoding a separator into it. `pathToFileURL` returns a real `URL` and requires an **absolute** path: Node resolves a relative one against the process working directory, and there is no such thing here. Documented as partial — no `URL`/`URLSearchParams` re-exports (both are globals), no legacy `url.parse`/`format`/`resolve`. |
| `node:module` | `createRequire` | Re-exports `ns:module`'s `createRequire` unchanged from a **distinct, separately frozen module object**. `createPumpingRequire` is deliberately absent: it has no Node counterpart, so code written against this shim keeps running on Node. `require.resolve`/`.cache`/`.main` are not implemented, and neither is any other `node:module` member (`Module`, `builtinModules`, `isBuiltin`, `register`, `syncBuiltinESMExports`). Documented as partial. |

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

## Android implementation notes (non-normative)

Builtin modules are function-body builtins
(`test-app/runtime/src/main/cpp/js/`, see the README there), compiled through
`BuiltinLoader`: the first compile in the process runs eagerly and produces a
code cache, and every later realm — including every worker — consumes that
process-wide bytecode cache instead of recompiling. The registry lives in
`NsBuiltinModules.{h,cpp}` and intercepts specifiers in the CommonJS require
path (`ModuleInternal::RequireCallbackImpl`) and in the ES module resolve and
dynamic-import callbacks (`ModuleInternalCallbacks.cpp`); ESM consumption is
served by a synthetic module whose exports are populated from the same
per-realm exports object. The internal require is a fixed parameter of the
builtin function wrapper (`exports`, `require`, `module`, `binding`,
`primordials`).

Per-realm builtin state — the exports objects, the synthetic modules, the
in-progress set, the cached `format`, the builtin `require` — and the loader's
`ModuleLoaderState` (module registry, loader vocabulary, in-flight graph loads)
live in `RuntimeState` slots rather than in isolate-keyed shared maps. A slot
is reached with an isolate data-slot read and a vector index, needs no lock,
and is destroyed with its isolate, so a worker gets its own instances as the
spec requires and teardown cannot leave a stale entry behind.

The ES module pipeline is a three-phase module map: a graph walk starting from
the entry discovers the transitive closure and compiles + registers every
module in it, so that by `InstantiateModule` time V8's synchronous
`ResolveModuleCallback` is a pure registry lookup — compile-and-register only,
never a fetch. Discovery is scheme-agnostic and every edge goes through the
same `ResolveSpecifierToPath` the resolver uses, so both agree on a module's
registry key; only the fetch differs per scheme. `http(s)` edges are fetched
concurrently off-thread and their completions hop back to the isolate's home
thread as **nestable** V8 foreground tasks on that isolate's event loop, so
`RunNestableV8Tasks` can drain them with JS frames already on the stack.

The boot backstop lives inside `Runtime::RunModule` (`HoldBootBackstop` in
`Runtime.cpp`). It holds the launching thread while either the entry's own
evaluation promise is still pending or async module-graph work is in flight,
pumping nestable V8 tasks, microtask checkpoints and `ALooper_pollOnce` until
both settle, bounded at twice `kModuleEvaluateDeadlineSeconds` (120s). A
settled entry simply exits the loop; only two outcomes are fatal, and both are
reported in every build:
`Fatal: the main entry module's evaluation rejected during boot: <reason>` and
`Fatal: the main entry module '<path>' never settled within 120s`.

Workers copy the loader vocabulary from the parent at spawn
(`CaptureLoaderVocabulary` on the parent's thread, `InstallLoaderVocabulary`
before the worker's first module load) and, for WHATWG parity, keep the
implicit port's message queue disabled until the worker entry finishes
evaluating — including after a pending top-level await settles. Messages sent
before that stay buffered.
