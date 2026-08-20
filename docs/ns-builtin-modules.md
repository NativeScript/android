# `ns:` builtin modules

Cross-runtime contract for exposing runtime-provided modules to application
code. This document is the specification both the iOS and Android runtimes
implement; a capability must behave identically on both platforms before it
ships in a stable release.

Everything under [The scheme](#the-scheme), [Module reference](#module-reference),
[Loading ES modules](#loading-es-modules), [The internal require](#the-internal-require)
and [Adding a builtin module](#adding-a-builtin-module) is normative. Platform
specifics that a portable app must not depend on are called out as platform
notes, and the closing section collects the Android ones.

## The scheme

Builtin modules live under the URL-style `ns:` scheme, mirroring Node's
`node:` prefix:

```js
// CommonJS
const util = require("ns:util");
```

```js
// ES modules — the exports object is also the default export.
import util, { inspect } from "ns:util";

const same = await import("ns:util");
console.log(same.default === util, same.inspect === inspect); // true true
```

Rules:

- `ns:` and `node:` specifiers are resolved by the runtime **before any
  filesystem or npm resolution**. They can never be shadowed by a file, a path
  mapping, or a package — and conversely, a file named `ns:util` is not
  reachable.
- Resolution of an unregistered builtin fails with an `Error` whose message is
  exactly `No such built-in module: <specifier>` (matching Node's wording for
  familiarity) — e.g. `No such built-in module: node:fs`. The failure is
  identical through `require()`, a static `import`, and a dynamic `import()`;
  the first two throw, the third rejects.
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

## Module reference

### `ns:util`

| export | description |
|---|---|
| `inspect(value[, options])` | Formats any value for human consumption: depth-limited, output-capped, cycle-safe, never invokes getters (except a guarded `error.stack` read and custom `toString` overrides, which are honored). `options.depth` (number) overrides the default depth of 2. Other option keys are reserved. |
| `format(fmt, ...args)` | Node-style printf formatting: `%s`, `%d`, `%i`, `%f`, `%j`, `%o`, `%O`, `%%`. Extra arguments are appended space-separated, objects rendered via `inspect`. When `fmt` is not a string or contains no substitutions, all arguments are formatted and joined with spaces. `console.*` routes its arguments through this, so `console.log("%d apples", 3)` works. |

```js
const { inspect, format } = require("ns:util");

// Depth-limited by default; pass `depth` to see further down.
const tree = { a: { b: { c: { d: 1 } } } };
inspect(tree); // "{ a: { b: { c: [Object] } } }"
inspect(tree, { depth: 4 }); // "{ a: { b: { c: { d: 1 } } } }"

// Cycles are rendered, not thrown on.
const cyclic = { name: "root" };
cyclic.self = cyclic;
inspect(cyclic); // '{ name: "root", self: [Circular] }'

format("%s took %dms", "boot", 12.5); // "boot took 12.5ms"
format("%j", { ok: true }); // '{"ok":true}'
format("100% sure", "extra"); // "100% sure extra"  (no placeholder consumed)
```

**Stability caveat (verbatim from Node's contract):** the output of `inspect`
(and therefore `format`'s object rendering) may change between runtime
versions for readability; it is intended for humans and must not be parsed
programmatically. String quoting is one such detail: Android renders strings
through `JSON.stringify`, so they come back double-quoted where iOS uses
Node's single-quoted style.

### `ns:runtime`

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

iOS additionally registers `releasedObjectPolicy`, which governs access to a
wrapper whose native counterpart has already been released. Android does not
register it: it has no released-native-counterpart machinery for that key to
govern, so the key is unknown here and both functions reject it like any other
unknown key.

```js
const { setConfig, getConfig } = require("ns:runtime");

// Turn on module-resolution and transport tracing for a diagnostic run.
setConfig("debug", "esm,fetch");
getConfig("debug"); // "esm,fetch"

// The list replaces the whole set, so turning tracing off needs no knowledge
// of what was already on.
setConfig("debug", "");
```

The `TypeError` messages are part of the contract:

| condition | message |
|---|---|
| unknown key (either function) | `Unknown runtime config key: '<key>'` |
| bad `setConfig` arity or non-string key | `setConfig expects (key: string, value)` |
| bad `getConfig` arity or non-string key | `getConfig expects (key: string)` |
| process-wide key written from a worker | `'<key>' is process-wide and can only be set from the main isolate` |
| non-string `debug` value | `'debug' must be a comma-separated category string (<categories>), or '' to disable tracing` |

`<categories>` is the runtime's own list of valid category names, which on
Android expands to `esm,fetch,registry`.

Remote-module security (`security.allowRemoteModules`,
`security.remoteModuleAllowlist`) is **not** part of this surface. Those
values are read once from nativescript.config / package.json the first time
the HTTP loader gates a fetch, and they cannot be inspected or changed
through `getConfig` / `setConfig`.

#### `debug`

Turns on the runtime's category-scoped trace logs. Categories:

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
a release build that cannot be diagnosed.

*Platform note (Android):* each category writes under its own logcat tag —
`TNS.esm`, `TNS.fetch`, `TNS.registry` — so `adb logcat -s TNS.esm` can filter
them without matching message text.

### `ns:module`

The module-loader control surface: import-map vocabulary, registry
invalidation, and the `createRequire` family. It is pure mechanism — every
policy concern (boot orchestration, hot-update protocols, full reload, CSS
apply, worker teardown) belongs to whatever tooling drives it.

| export | description |
|---|---|
| `configureLoader(config)` | Installs loader policy for the calling isolate. Sections: `importMap` (`imports` + `scopes`), `volatilePatterns` (URL substrings always re-fetched), `canonicalization` (registry-keying vocabulary). Each **present** section replaces its state wholesale, an empty array included. Throws `TypeError` on any malformed input, having validated the whole config first, so a rejected call installs nothing. |
| `invalidateModules(urls)` | Evicts the given URLs (canonicalized) from the module registry and marks them bust-next-fetch, so the next network fetch bypasses every HTTP cache layer. Takes an array of strings; throws `TypeError` otherwise. |
| `getLoadedModuleUrls()` | The URL-like keys currently in the module registry, as an array of strings (used to compute full-reload eviction sets). |
| `createRequire(filenameOrURL)` | A `require` resolving against `filenameOrURL`'s directory (a trailing slash names the directory itself). Accepts an absolute path string, a `file:` URL string, or a URL object; anything else throws `TypeError`, and an `http(s)` base is refused outright because `require()` of a dev-served module is not supported — import those. ES module graphs load under Node's `require(esm)` rule: a graph containing top-level await is refused before it evaluates. |
| `createPumpingRequire(filenameOrURL, options?)` | Same argument contract and same resolution, but an ES module graph with top-level await is evaluated by driving the loop until it settles, instead of being refused. **Callable only from a task context.** See [Pumping requires](#pumping-requires). |

`ns:module` (loader policy — structured, installed ahead of traffic) is
deliberately separate from `ns:runtime` (live key-value runtime flags via
`setConfig`/`getConfig`).

Both functions validate their arguments and throw `TypeError` on anything
malformed — the behavior WebIDL gives a web API and `ERR_INVALID_ARG_TYPE`
gives a Node one. Nothing is silently skipped or filtered: a mistyped section
or a typo'd key is a caller bug, and reporting it is what keeps it from
becoming a config that quietly does nothing.

| condition | message |
|---|---|
| missing or non-object config | `configureLoader expects a config object` |
| a key other than the three sections | `configureLoader: unknown option '<key>'` |
| `volatilePatterns` not an array | `configureLoader: volatilePatterns must be an array of strings` |
| a non-string in `volatilePatterns` | `configureLoader: volatilePatterns[<index>] must be a string` |
| `canonicalization` not an object | `configureLoader: canonicalization must be an object` |
| a `canonicalization` sub-key not an array | `configureLoader: canonicalization.<key> must be an array of strings` |
| a non-string in a `canonicalization` sub-key | `configureLoader: canonicalization.<key>[<index>] must be a string` |
| `invalidateModules` argument not an array | `invalidateModules expects an array of URL strings` |
| a non-string in that array | `invalidateModules: urls[<index>] must be a string` |

`configureLoader` validates the **entire** config — every section plus the key
names — before installing any of it. A call that throws therefore leaves all
three sections exactly as they were: the atomicity the import map alone used to
have now covers the whole call, so a config that is half-right cannot land
half-applied.

"Replaces its state wholesale" is keyed on a section being **present**, not on
its contents: `volatilePatterns: []` clears the list, and an absent section is
left alone. `undefined` counts as absent, so spreading an optional section is
safe.

```js
const { configureLoader, getLoadedModuleUrls, invalidateModules } =
  require("ns:module");

configureLoader({
  importMap: {
    imports: {
      "lodash": "http://localhost:8080/vendor/lodash.mjs",
      "@scope/pkg/": "http://localhost:8080/pkg/",
    },
  },
  volatilePatterns: ["/@ns/"],
});

// Later: drop everything the server says changed, so the next import refetches.
const stale = getLoadedModuleUrls().filter((url) => url.includes("/src/"));
invalidateModules(stale);
```

`getLoadedModuleUrls()` reports the **URL-like** keys only: registry entries
that are `blob:`-prefixed or contain `://`. A module keyed by a plain
filesystem path is not in the result, so the eviction set a dev client computes
from it covers served modules rather than the app's own bundled files.

`createRequire` gives a module-relative `require` from anywhere, including an
ES module that has no `__filename`:

```js
import { createRequire } from "node:module";

const require = createRequire(import.meta.url);
const config = require("./config.json");
const helper = require("./helpers/format.js");
```

Neither require implements `require.resolve`, `require.cache`, or
`require.main`. They are **absent** rather than throwing, so a feature check
works; adding them is a change to this specification first.

Debug builds additionally carry `canonicalizeHttpUrlKey(url)`, a pure test
diagnostic that takes a string and throws
`canonicalizeHttpUrlKey expects a URL string` otherwise; release builds omit
it. Missing members are simply absent — never
present-but-throwing — so feature checks work. The module is registered in
every build: the security boundary for remote module loading sits at the
network layer (`security.allowRemoteModules` in nativescript.config, enforced
by the HTTP loader), not at the module registry and not at `ns:runtime`.

#### Import maps and scopes

`importMap` takes the WHATWG shape:

```js
const { configureLoader } = require("ns:module");

configureLoader({
  importMap: {
    imports: {
      "lodash": "http://host/vendor/lodash.mjs",
      "@scope/pkg/": "http://host/pkg/",
    },
    scopes: {
      "http://host/legacy/": { "lodash": "http://host/vendor/lodash-3.mjs" },
    },
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
consults the most specific matching scope first (longest prefix wins), then
progressively less specific ones, then `imports` — so a scope can override a
global mapping for one subtree and fall through to it everywhere else. The
synchronous resolver, the graph walk, and `import()` all resolve through the
same cascade.

The whole map is parsed and validated before any of it is installed: a
rejected map throws a `TypeError` out of `configureLoader` and the previously
installed map keeps resolving, so a typo in an update cannot empty a live
session's vocabulary. Every message is prefixed `configureLoader: `.

| condition | message (after the `configureLoader: ` prefix) |
|---|---|
| `importMap` is neither an object nor a non-empty string | `importMap must be an object or a JSON string` |
| empty JSON | `an import map must be a non-empty JSON object` |
| unparseable JSON | `an import map must be valid JSON: <detail>` |
| JSON that is not an object | `an import map must be a JSON object` |
| any import-map section other than `imports`/`scopes` | `unsupported import-map section '<name>'; only "imports" and "scopes" are supported` |
| `imports` is not an object | `the "imports" section must be an object` |
| `scopes` is not an object | `the "scopes" section must be an object` |
| non-string scope key | `scopes: every scope key must be a string` |
| empty scope key | `scopes: a scope key must not be empty` |
| a scope's value is not an object | `scopes: the map for scope '<scope>' must be an object` |

Inside either section — labelled `imports` or `scope '<scope>'`:

| condition | message |
|---|---|
| non-string specifier key | `<section>: every key must be a string` |
| empty specifier key | `<section>: a specifier key must not be empty` |
| non-string target | `<section>: the target for '<specifier>' must be a string` |
| empty target | `<section>: the target for '<specifier>' must not be empty` |
| trailing-slash key, non-trailing-slash target | `<section>: the target for '<specifier>' must end with '/' because the specifier key does` |

#### Registry canonicalization

The registry keys modules by a canonical URL. The mechanism — fragment strip,
cache-buster param drop, param sort — is the runtime's; the *vocabulary* is
server policy, supplied here:

| key | meaning |
|---|---|
| `stripParams` | query param names that are pure cache busters and are dropped for dev endpoints (e.g. `t`, `v`, `import`) |
| `forPathPrefixes` | path prefixes (starts-with) identifying the dev endpoints whose query may be normalized (e.g. `/ns/`, `/@id/`) |
| `preserveQueryFor` | path substrings whose query **is** the module identity and must be preserved verbatim (e.g. `/@ng/component`) |

```js
const { configureLoader } = require("ns:module");

configureLoader({
  canonicalization: {
    stripParams: ["t", "v", "import"],
    forPathPrefixes: ["/ns/", "/@id/"],
    preserveQueryFor: ["/@ng/component"],
  },
});
```

Presence of the `canonicalization` object marks the vocabulary as configured
and replaces the built-in fallback entirely; empty arrays are honored as
explicit policy. `preserveQueryFor` is checked before the dev-endpoint prefix
test, so a path that matches both keeps its query.

#### Reconfiguration and workers

The loader vocabulary is **per-isolate**: `configureLoader` writes the isolate
that calls it and nothing is shared between isolates, so no lock guards it.

A worker receives a **copy** of its parent's vocabulary, captured on the
parent's thread while the worker spawns and installed on the worker's isolate
before it loads its first module. That copy is a snapshot: reconfiguring the
parent afterwards leaves running workers on the vocabulary they started with.

Normatively: **tooling that reconfigures loader vocabulary must restart
workers for the change to reach them.** A worker started after the
`configureLoader` call resolves through the new vocabulary; a live worker
never observes a later reconfiguration.

### `node:` compatibility shims

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
  when every member is re-exported unchanged. `ns:` modules may grow
  runtime-specific members freely; a `node:` shim only ever gains members that
  track Node's actual API. This mirrors how Bun (`bun:*`), Deno (`Deno.*`/JSR)
  and Cloudflare (`cloudflare:*`) all keep their own surface strictly apart
  from their `node:` compat layer.
- Shims ship on both runtimes under the same parity rule as `ns:` modules.

| module | exports | notes |
|---|---|---|
| `node:util` | `inspect`, `format` | Re-exports `ns:util`'s members unchanged (`nodeUtil.inspect === nsUtil.inspect`) from a **distinct, separately frozen module object**. Documented as partial. |
| `node:url` | `fileURLToPath`, `pathToFileURL` | Node-strict converters between `file:` URLs and paths. Documented as partial — no `URL`/`URLSearchParams` re-exports (both are globals), no legacy `url.parse`/`format`/`resolve`. |
| `node:module` | `createRequire` | Re-exports `ns:module`'s `createRequire` unchanged from a **distinct, separately frozen module object**. `createPumpingRequire` is deliberately absent: it has no Node counterpart, so code written against this shim keeps running on Node. `require.resolve`/`.cache`/`.main` are not implemented, and neither is any other `node:module` member (`Module`, `builtinModules`, `isBuiltin`, `register`, `syncBuiltinESMExports`). Documented as partial. |

`node:url`'s parsing goes through the URL intrinsic, so `file://localhost/x` is
accepted (the URL spec folds a `localhost` authority to none) while any other
host throws, and the query and fragment are never part of the path.
`fileURLToPath` rejects a non-`file:` scheme and rejects `%2F` in the path
rather than decoding a separator into it. `pathToFileURL` returns a real `URL`
and requires an **absolute** path: Node resolves a relative one against the
process working directory, and there is no such thing here.

```js
const { fileURLToPath, pathToFileURL } = require("node:url");

fileURLToPath("file:///app/src/main.js"); // "/app/src/main.js"
fileURLToPath("file://localhost/app/a.js"); // "/app/a.js"
fileURLToPath("file:///app/a.js?v=2#frag"); // "/app/a.js"

pathToFileURL("/app/my file.js").href; // "file:///app/my%20file.js"
```

Its `TypeError` messages are Node's:

| condition | message |
|---|---|
| argument is neither a string nor a URL-like object, or is unparseable | `The "path" argument must be of type string or an instance of URL.` |
| non-`file:` scheme | `The URL must be of scheme file` |
| a host other than `localhost` or empty | `File URL host must be "localhost" or empty` |
| `%2F` in the path | `File URL path must not include encoded / characters` |
| `pathToFileURL` given a non-string | `The "path" argument must be of type string.` |
| `pathToFileURL` given a relative path | `The "path" argument must be an absolute path.` |

## Loading ES modules

### The `require()` specifier

Every `require` — the global one and any minted by `createRequire` /
`createPumpingRequire` — takes a **string** specifier. Anything else throws a
`TypeError` with Node's `ERR_INVALID_ARG_TYPE` wording, before any builtin,
`http(s)` or filesystem handling runs:

```
The "id" argument must be of type string. Received <what>
```

`<what>` follows Node's `determineSpecificType`: `undefined`, `null`,
`type number (42)`, `an instance of Object`, `function foo`, and so on.

### `require()` of an ES module

`require()` of an ES module works, under Node's `require(esm)` rule: the graph
is loaded and evaluated synchronously **unless it contains top-level await**,
in which case it is refused *before evaluation* with an `Error` reading

```
require() cannot load ES module '<canonical path>': the module graph contains top-level await. Use import() or createPumpingRequire from ns:module instead.
```

The refusal never evicts the module: the graph stays instantiated, so the very
same module still loads through `import()`. Match the text with
`toContain`-style substring checks rather than full-string equality — how much
surrounding context a platform adds to a `require()` failure is not part of
this contract.

### What `require()` of an ES module returns

A namespace object is not a CommonJS exports object, so the runtime applies
Node's `populateCJSExportsFromESM` cascade, in this order:

1. **An own export literally named `module.exports` wins outright** — its value
   is what `require()` returns. This is the escape hatch for a module that
   wants full control of its CJS shape.
2. **Otherwise the namespace is returned unchanged** when it has **no own
   `default` export**, *or* when it **already declares its own `__esModule`**.
   Declaring `__esModule` yourself is therefore an explicit opt-out of step 3.
3. **Otherwise** (an own `default`, no own `__esModule`) `require()` returns a
   **live-binding facade**: a synthetic module re-exporting everything from the
   target plus `__esModule = true`. Transpiled consumers reading
   `_mod.__esModule ? _mod.default : _mod` find the default, and because the
   facade re-exports rather than copies, bindings stay live.

```js
// a.mjs — no default export: the namespace passes through.
export const x = 1;
// require("./a.mjs")  →  { x: 1 }

// b.mjs — a default and no __esModule: the facade is built.
export default function boot() {}
export const version = "1.0";
// require("./b.mjs")  →  { default: boot, version: "1.0", __esModule: true }

// c.mjs — takes over the CJS shape completely.
const handler = () => {};
export { handler as "module.exports" };
// require("./c.mjs")  →  handler
```

### Pumping requires

`createPumpingRequire` lifts the top-level-await refusal by driving the loop —
running nestable tasks and draining microtasks — until the graph settles. That
default slice services engine work only: a graph whose settling depends on a
JS timer, a worker reply, or anything else riding the platform loop needs
`pumpRunLoop: true` (see below), on both platforms.

Options are validated once, **when the require is minted**; a `require()` call
itself does no option work. Unknown keys throw rather than being silently
ignored.

| option | values | default | meaning |
|---|---|---|---|
| `deadlineSeconds` | positive finite number | `60` | how long the graph gets to settle in-pump. Governs the **evaluation-settle phase only** — the graph walk's fetch deadline is separate and unaffected. |
| `onTimeout` | `"throw"` \| `"return-pending"` | `"throw"` | what an expired deadline means. `"return-pending"` hands back a namespace whose evaluation is still in flight. |
| `pumpRunLoop` | boolean | `false` | also run the platform loop's own due work each pump iteration — a run-loop slice on iOS, the looper-equivalent drain on Android (JS timers, worker messages, plain loop posts) — for graphs whose progress depends on more than engine tasks and microtasks. |

Validation errors, all `TypeError`:

| condition | message |
|---|---|
| `options` present but not an object | `createPumpingRequire: options must be an object` |
| unrecognized key | `createPumpingRequire: unknown option '<key>'` |
| bad `deadlineSeconds` (non-number, non-finite, `<= 0`) | `createPumpingRequire: 'deadlineSeconds' must be a positive finite number` |
| bad `onTimeout` | `createPumpingRequire: 'onTimeout' must be 'throw' or 'return-pending'` |
| bad `pumpRunLoop` | `createPumpingRequire: 'pumpRunLoop' must be a boolean` |
| `options` passed to `createRequire` | `options are not supported on createRequire` |

Both requires share the base-argument contract, and both reject an `http(s)`
base:

| condition | message |
|---|---|
| not an absolute path, `file:` URL string, or URL object | `The argument 'filename' must be a file URL object, file URL string, or absolute path string.` |
| an `http(s)` base | `createRequire() cannot take an http(s) URL (<value>): require() of a dev-served module is not supported. Pass an app-root file path and use import() for remote modules.` |

**The microtask-reentrancy refusal.** The loop cannot be pumped re-entrantly:
the engine ignores a microtask checkpoint while the isolate is already draining
the microtask queue. A top-level await resumes through a promise reaction — a
microtask — so such a graph can never settle from inside a microtask turn.
Requiring one from after an `await` or inside a `.then` callback therefore
throws immediately, before evaluation, leaving the graph instantiated so
`import()` can still load it:

```
createPumpingRequire cannot settle module graph '<canonical path>' from inside a microtask (after an await or inside a promise callback): the event loop cannot be pumped re-entrantly. Call it from a task context, or use import().
```

Call it from a task context instead — a native boundary, an event handler, a
timer callback, or module evaluation itself. A **synchronous** graph needs no
pumping and stays legal from anywhere, microtask turns included.

### What an HTTP module response must be

A module fetched over `http(s)` is classified by status and MIME type before it
ever reaches the compiler, so a dev server that answers with an error page
produces a clear diagnostic instead of a syntax error. Both the synchronous
fallback and the async graph walk use the same classifier, so they cannot
disagree about what a response means. Every failure below surfaces as a plain
`Error` whose `message` is exactly the quoted text — as a throw during module
instantiation, or as the rejection of a dynamic `import()`.

The MIME **essence** is the Content-Type with everything from the first `;`
discarded, then trimmed of spaces and tabs and lowercased — so
`Content-Type: TEXT/JavaScript; charset=utf-8` has essence `text/javascript`.

**Loads as JavaScript** — the HTML spec's JavaScript MIME type essence list,
matched exactly:

`application/ecmascript`, `application/javascript`, `application/x-ecmascript`,
`application/x-javascript`, `text/ecmascript`, `text/javascript`,
`text/javascript1.0`, `text/javascript1.1`, `text/javascript1.2`,
`text/javascript1.3`, `text/javascript1.4`, `text/javascript1.5`,
`text/jscript`, `text/livescript`, `text/x-ecmascript`, `text/x-javascript`.

**Loads as a JSON module**: essence `application/json`, `text/json`, or any
essence ending in `+json` (e.g. `application/vnd.api+json`).

**An empty 2xx body with a JavaScript MIME is a valid empty module.** Type-only
TypeScript modules transform to zero runtime code and dev servers serve them as
empty 200s; the runtime substitutes a canonical empty module rather than
failing the whole graph. An empty **JSON** body is a failure — there is no
canonical empty JSON module.

Failures, in the order they are checked:

| condition | message |
|---|---|
| no response at all | `HTTP import failed: <url> (network error)` |
| status 204 or 205 | `HTTP import failed: <url> (status=<status>, no content)` |
| any other non-2xx status | `HTTP import failed: <url> (status=<status>)` |
| missing or empty Content-Type | `Expected a JavaScript module but '<url>' responded with no MIME type` |
| JSON MIME, empty body | `Expected a JSON module but '<url>' responded with an empty body` |
| any other MIME (e.g. `text/html`) | `Expected a JavaScript module but '<url>' responded with MIME type '<essence>'` |

Ahead of all of these sits the security gate: when remote module loading is not
permitted, no request is made at all and the failure is
`HTTP import blocked: remote module loading is not allowed for <url>`.

204 and 205 are checked before the MIME type, so a "no content" response fails
as such even when it carries a JavaScript Content-Type — the web likewise
treats it as a network error for a module script rather than as an empty
module. The `<essence>` in the foreign-MIME message is the normalized essence,
not the raw header.

### App entries and bootstraps

An app's entry can be either CommonJS or an ES module, and the choice decides
when loader vocabulary can be installed.

**The ordering rule is normative: `configureLoader` must run before any ES
module traffic it is meant to govern.** The import map is consulted inside the
engine's *synchronous* resolver, so it cannot be produced on demand — it has to
be installed ahead of the imports that need it.

**An ES module main entry is supported directly**, top-level await included.
When the app's `main` resolves to an ES module, the entry is evaluated as a
module rather than `require()`d, so `import`/`export` are legal there. A local
entry is given a **short, non-throwing yield**: one brief in-place window in
which the graph may settle, after which evaluation simply continues on the real
event loop. Only nestable tasks can run while the entry's frames are on the
stack, so a top-level await parked on anything else could never settle in
place; returning instead of throwing is the Node shape. Should the entry's
evaluation promise still be pending when the yield ends, a **boot backstop**
holds the process until it settles, **bounded at twice the module deadline** —
and the backstop, unlike the yield, drains the loop's own due work (JS timers
and worker messages included), so an entry parked on those settles there.

The trade-off: an ES module entry's own **static** imports resolve *before* its
body runs, so anything that needs `configureLoader` to have run must be reached
through a dynamic `import()` after that call. Keep the entry's static imports
to builtins only.

```js
// main.mjs — static imports are builtins only, so nothing races the config.
import { configureLoader } from "ns:module";

configureLoader({
  importMap: { imports: { "lodash": "http://localhost:8080/vendor/lodash.mjs" } },
});

// Everything that resolves through the map is reached dynamically, after.
const { start } = await import("./app.mjs");
start();
```

**A CommonJS bootstrap avoids that constraint by being synchronous**: it
configures the loader and then pulls in the ES module entry, with no static
imports to resolve early.

```js
// main.js — a CommonJS bootstrap for an ESM app.
const { configureLoader, createPumpingRequire } = require("ns:module");

configureLoader({
  importMap: { imports: { "lodash": "http://localhost:8080/vendor/lodash.mjs" } },
});

createPumpingRequire(__filename, {
  pumpRunLoop: true,
  onTimeout: "return-pending",
  deadlineSeconds: 1,
})("./entry.mjs");
```

Two warnings on that bootstrap, both load-bearing:

- `pumpRunLoop: true` is sane **only while boot owns the looper**. After boot
  the looper belongs to the app, and slicing it from inside a require re-enters
  arbitrary looper sources — including UI callbacks — underneath JS frames.
- With `onTimeout: "return-pending"` the returned namespace may still be
  evaluating. A bootstrap must **discard it** and never read a binding off it;
  reading one is a TDZ error at best.

Pick whichever fits the app: the ESM entry is simpler and needs no bootstrap
file, the CommonJS bootstrap buys unconstrained ordering.

*Platform note (Android):* there is no never-returning entry point here — the
entry is evaluated from `Runtime::RunModule`, which returns to Java when boot
finishes — so the boot backstop is on the path of every app, not just the ones
that park. Its two failures are fatal and reported in every build:
`Fatal: the main entry module's evaluation rejected during boot: <reason>` and
`Fatal: the main entry module '<path>' never settled within 120s`. A
**CommonJS** main that throws is fatal the same way: the failure is rethrown
through the boot boundary (prefixed `require() failed for module <path>`)
rather than left pending under the backstop, in every build.

## The internal require

Builtin modules reach each other — and only each other — through an internal
`require` the runtime provides to every builtin source. This is the mechanism
shims are built on, so it is normative: both runtimes provide it.

- It resolves **builtin specifiers only**. A path, a package name or any other
  specifier is not reachable from a builtin; an unregistered builtin name
  throws the same `No such built-in module: <specifier>` an app sees.
- It materializes the target module on first use and returns the realm's
  singleton afterwards, which is what makes shims lazy.
- Requiring a module that is still being built throws rather than recursing, so
  a dependency cycle between builtins is a loud error and not a hang. The
  message is exactly `Circular require of built-in module: <specifier>`.

## Adding a builtin module

- The name is a short, lowercase identifier (`ns:util`, `ns:timers`, ...).
- One module, one source file, one registry entry — including shims.
- New modules and new exports require this document to be updated first and
  an implementation on both runtimes before a stable release; a module may
  ship on one platform behind a documented "experimental, iOS-only" (or
  Android-only) note in between.
- Internal runtime machinery must never be reachable through the scheme.

That last rule holds because public modules and internal builtins are **two
separate loading paths**, not one registry with a per-entry flag:

- The **public registry** is a table mapping specifier → builtin, and it is the
  only thing the `ns:`/`node:` resolver consults. A specifier absent from it
  does not resolve, full stop. Today it holds six entries: `ns:module`,
  `ns:runtime`, `ns:util`, `node:module`, `node:url`, `node:util`.
- **Internal builtins** (the intrinsics snapshot, the require factory, the
  console formatter, and so on) are invoked directly from their own native call
  sites. They are never named in the public registry, so there is no specifier
  that could reach them and nothing to mark private.

Adding an internal builtin therefore cannot accidentally expose it; exposing
one is an explicit registry entry, which is also the change this document has
to describe.

## Source-text modules: deliberately not supported

Builtins are classic function bodies, not ES modules, on both runtimes. If
cross-builtin code sharing is ever needed, the first answer is bundling at
generation time (author as ESM, emit function bodies); runtime source-text
builtin modules (Node's `kSourceTextModule`) are justified only by a concrete
need for live module semantics (TLA, live bindings, cyclic imports), which no
current or planned builtin has. Revisit here before building either.

## Android implementation notes (non-normative)

Builtin modules are function-body builtins
(`test-app/runtime/src/main/cpp/js/`, see the README there). A CMake custom
command runs `tools/js2c.mjs` to embed them into `generated/RuntimeBuiltins.cpp`,
and `BuiltinLoader::RunBuiltin` compiles them with an `internal/<name>.js`
script origin and a process-wide bytecode cache: the first compile in the
process runs eagerly and produces a code cache that every later realm —
including every worker — consumes instead of recompiling. The public registry
lives in `NsBuiltinModules.{h,cpp}` and intercepts specifiers in the CommonJS
require path (`ModuleInternal::RequireCallbackImpl`) and in the ES module
resolve and dynamic-import callbacks (`ModuleInternalCallbacks.cpp`); all three
paths share one `NsBuiltinModules::NotFoundMessage`, which is why the failure
text is identical across them. ESM consumption is served by a synthetic module
whose exports are populated from the same per-realm exports object. The
internal require is a fixed parameter of the builtin function wrapper
(`exports`, `require`, `module`, `binding`, `primordials`).

The files under `js/` that are *not* in the public registry — `primordials.js`,
`require-factory.js`, `inspect.js`, `json-helper.js`, `events.js`,
`error-events.js`, `structured-clone.js`, `blob-url.js`, `performance.js`,
`weak-ref.js` — are the internal builtins: each is run from its own native call
site and none is named in the registry table.

Per-realm builtin state — the exports objects, the synthetic modules, the
in-progress set that produces the circular-require error, the cached `format`,
the builtin `require` — and the loader's `ModuleLoaderState` (module registry,
loader vocabulary, in-flight graph loads) live in `RuntimeState` slots rather
than in isolate-keyed shared maps. A slot is reached with an isolate data-slot
read and a vector index, needs no lock, and is destroyed with its isolate, so a
worker gets its own instances as the spec requires and teardown cannot leave a
stale entry behind.

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

A local entry counts as an ES module when its path ends in `.mjs`. The module
deadline is a single constant, `kModuleEvaluateDeadlineSeconds` = 60 seconds
(`ModuleInternal.h`), shared by the HTTP entry's settle window, the pumped
graph walk, and — doubled, at 120 seconds — the boot backstop. The waits are
*designed* to nest — transport timeouts within the module deadline within the
boot backstop — but the transport numbers are per-attempt bounds (connect) and
per-read inactivity bounds (read), not totals: a retried fetch or a slowly
dripping response can legally spend longer than one connect+read sum, and the
deadline above it is what actually cuts the wait off. The local entry's short
yield is deliberately *not* derived from that constant: it is an independent
one-second literal in `BootEntryEvaluationOptions`, with `return-pending`
behavior. An HTTP entry instead gets the full deadline and throws on expiry,
because the tooling driving it needs the rejection reason synchronously. The
backstop itself is `HoldBootBackstop` in `Runtime.cpp`, called from both
`Runtime::RunModule` overloads; it pumps the isolate's event loop in place
(`EventLoop::PumpUntil`) until the entry and all async graph work settle.

Every pump on Android runs the same `EventLoop::PumpUntil` primitive, in one
of two modes that mirror the iOS pump exactly:

- **The default slice** — nestable V8 tasks plus a microtask checkpoint —
  is all a pumping require gets unless it opts in, matching iOS's default
  pump body. Work outside that lane (JS timers, worker messages, `Handler`
  posts) does not run: it rides Java `Handler` messages or its own fds, which
  cannot dispatch while the pump's JS frames hold the thread.
- **`pumpRunLoop: true` adds the looper-equivalent drain**, standing exactly
  where iOS slices its run loop: due ordered-lane work (JS timers) and plain
  internal-lane posts (worker messages, Node-API completions) run directly
  from the pump, which is what lets a graph parked on `setTimeout` or a
  worker reply settle in-pump. The boot backstop and the pumped graph walk
  always drain, just as iOS's boot path always pumps its run loop. The pump
  still never re-enters the platform looper itself, and non-nestable V8
  tasks stay queued in both modes.

The drain runs the loop's own work *early*, while the looper is blocked: a
due timer callback can execute in the middle of a `require()` — before
`Handler.post` runnables queued ahead of it — and can observe the require in
progress. That is inherent to pumping (iOS's run-loop slice does the same)
and is the reason the default mode stays conservative.

Workers copy the loader vocabulary from the parent at spawn
(`CaptureLoaderVocabulary` on the parent's thread, `InstallLoaderVocabulary`
before the worker's first module load) and, for WHATWG parity, keep the
implicit port's message queue disabled until the worker entry finishes
evaluating — including after a pending top-level await settles. Messages sent
before that stay buffered.

Unlike iOS, Android ships no `.d.ts` declarations for the `ns:` modules; the
`.d.ts` files in this repo describe the Android platform classes, not this
surface.
