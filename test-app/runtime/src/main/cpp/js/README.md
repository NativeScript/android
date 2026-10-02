# Runtime builtins

The `.js` files in this directory are the runtime's internal JavaScript. At
build time a CMake custom command runs `tools/js2c.mjs`, which embeds them into
`test-app/runtime/src/main/cpp/generated/RuntimeBuiltins.cpp`; at runtime
`BuiltinLoader::RunBuiltin` compiles and executes them with an
`internal/<name>.js` script origin and a process-wide bytecode cache.

## Contract (Node's module wrapper + internalBinding idiom)

Every file is compiled as a **function body** via `v8::ScriptCompiler::CompileFunction`
with the fixed parameters `exports`, `require`, `module`, `binding` and
`primordials`:

```js
const { someNative, anotherNative } = binding;
const { ArrayPrototypeSlice, ObjectCreate } = primordials;
const { inspect } = require("ns:util");

module.exports = somethingTheCallSiteNeeds;
```

- `binding` is a plain object of natives built by the C++ call site; a file
  that needs nothing from C++ simply doesn't mention it.
- `require` resolves **builtin specifiers only** (`ns:util`, `node:util`, …),
  never a path or a package; an unknown one throws
  `No such built-in module: <specifier>`. It is how a `node:` shim consumes the
  `ns:` module it adapts, and it materializes that module on first use.
  Requiring a module that is still loading throws rather than recursing.
  It also resolves the **internal tier** (`internal/events`,
  `internal/dom-exception`): registry rows marked internal-only in
  `NsBuiltinModules.cpp` that the module system refuses, so only builtins can
  name them — Node's internal-module idiom. This is the one channel for
  cross-builtin capabilities that must never leak to app code (the
  `kListenerChanged` hook key abort-signal.js takes from events.js, the
  `setListenerErrorReporter` setter error-events.js calls): the producer puts
  the capability in its `module.exports`, the consumer requires it. The
  `internal/events` bag publishes `globalEventTarget`, `CustomEvent`,
  `kListenerChanged`, `setListenerErrorReporter`, `Event`, `EventTarget`,
  `defineEventHandler` and `dispatchEventRethrowing` — the base classes and the
  handler-attribute helper are there because a lazy builtin may not read live
  globals (see the rule two sections down), so this is the sanctioned door to
  them.
  `require("internal/…")` at first use runs the file through the shared
  exports cache — for a consumer of an eager producer that is a cache hit,
  and a miss runs the producer on demand. A consumer can therefore never
  observe a missing capability: it gets the exports, or the require throws
  (circular require, or the producer failing to run).
- `primordials` is the frozen intrinsics snapshot built by `primordials.js`
  (see below), the same object for every builtin in an isolate.
- **`module.exports` is the export channel** — whatever it holds when the file
  finishes is what `RunBuiltin` hands back to C++ (used for factory functions
  and init results). Both CommonJS styles work: replace the whole export with
  `module.exports = x`, or hang properties off `exports`. A file that only
  installs globals exports nothing and the call site ignores the value.
- **No top-level `return`.** It would work — these are function bodies — but
  every tool that isn't reading this repo's ESLint config (editors' TS server,
  prettier, review bots) rejects the file as invalid JavaScript.
- Strict mode is per-file: start the file with `"use strict";` to opt in.
- `inspect.js` is the console formatter (util.inspect-lite, exposed as the
  internal `__inspect` global): budgeted output, no getter invocation,
  tamper-immune via primordials. Console routes all object formatting
  through it.
- `ns-util.js` is the `ns:util` module app code requires and `node-util.js` the
  `node:util` shim: one source file per specifier, the shim owning every bit of
  Node compatibility. See `docs/ns-builtin-modules.md` for the cross-runtime
  contract.
- `ns-module.js` is the `ns:module` loader-control surface and `ns-runtime.js`
  is the `ns:runtime` live config surface (`setConfig`/`getConfig`).
- `node-module.js` re-exports `ns:module`'s `createRequire` as the `node:module`
  shim, and `node-url.js` is the `node:url` shim (`fileURLToPath` /
  `pathToFileURL`), the one shim with no `ns:` counterpart to adapt.
- Destructure `binding` and `primordials` once, at the top of the file, so the
  file's dependencies are visible and greppable.

## Eager and lazy builtins

Most builtins run during `Runtime::PrepareV8Runtime` and install their globals
themselves. A **lazy** builtin instead exports its interfaces and is run by
`LazyGlobals` (`src/main/cpp/LazyGlobals.cpp`), which registers each global it
backs as a lazy data property on the global template: the first read of the
name runs the file through the per-isolate exports cache
(`BuiltinLoader::GetExports`) so sibling names share one run, and V8 replaces
the property with a plain data property. That cache is the same one the
`ns:`/`node:` module registry uses, so a module re-exporting a lazy builtin's
interfaces (`ns:util`'s `TextEncoder`) hands out the objects the globals hold,
in either access order. Until then nothing of it exists — no compile, no run,
no allocation. `text-encoding.js` (`TextEncoder`/`TextDecoder`), `base64.js`
(`atob`/`btoa`), `dom-exception.js` (`DOMException`), `message-event.js`
(`MessageEvent`), `message-channel.js` (`MessagePort`/`MessageChannel`) and
`broadcast-channel.js` (`BroadcastChannel`) are the current ones; new globals
join by adding a row to `kLazyGlobals`.

Two neighbours of that set are deliberately not in it. `worker-events.js` is
**eager**: it defines the handler attributes on `Worker.prototype` and the
worker global scope, which have to exist before app code assigns one.
`node-worker-threads.js` is a **public builtin module** (`node:worker_threads`)
rather than a lazy global — it is reached by specifier, so nothing places a
name for it.

An **eager** file can also feed the tier: `events.js` (eager, `Events::Init`)
exports `CustomEvent`, and the `CustomEvent` row reads it through the same
exports cache — the run happened at init, so only the placement is lazy.

The two extra rules a lazy builtin lives by:

- **It runs at an arbitrary point in the isolate's life, not at init.**
  Anything it needs from another builtin has to come through `require`
  (including the internal tier) or its `binding` — never from init-order
  assumptions.
- **It must not install anything on `globalThis`.** The C++ tier owns
  placement; a file that self-installs would have to run to do it, which is
  the thing being avoided.

## Rules

- Eager builtins run at isolate init, before any user code: capture any global
  you rely on (e.g. `globalThis.Event`) eagerly so later monkey-patching can't
  break you. For intrinsics that is what `primordials` is; for everything else
  (`URLSearchParams`, …) capture it into a file-level `const`. A lazy builtin
  gets the same pristine `primordials`, but the live globals it would capture
  are whatever user code left behind, so it should not reach for them at all.
- The per-instance wrappers `defineEventHandler` creates live on the target's
  **own listener bag**, under a private symbol — never in a WeakMap keyed by
  the target. Own-instance state is Node's own design for handler attributes,
  and it keeps the builtins independent of how the collector treats the
  weak-collection entries of objects native code keeps alive — a `Worker` is
  rooted by the runtime for as long as its thread runs.
- No `import`/`export` — these are classic function bodies, not modules.
- ESLint (`eslint.config.mjs` at the repo root, `npm run lint`) declares
  `exports`, `require`, `module`, `binding`, `primordials` and the reachable
  native globals; `no-undef` is the typo net. If a builtin starts using a new native
  global, add it there. `no-restricted-properties` fails the lint on direct use
  of the captured statics (`JSON.stringify`, `Object.defineProperty`, …).
  Uncurried instance methods can't be matched that way, so `list.slice()`
  instead of `ArrayPrototypeSlice(list)` is caught by review, not by the
  linter.
- File names are kebab-case; the name determines the `BuiltinId` enum value
  (`weak-ref.js` → `kWeakRef`) and the script origin. New files must also be
  added to `RUNTIME_BUILTIN_JS` in `test-app/runtime/CMakeLists.txt` — the
  build fails with an explicit message if that list drifts out of sync
  (`js2c.mjs --check-dir`).

## primordials

`primordials.js` runs first in every isolate — lazily, on the first
`RunBuiltin` call, which happens during runtime init — and its frozen,
null-prototype export is cached per runtime (`BuiltinLoader`, in
`RuntimeState`) and handed to every other builtin, so a builtin that
compiles later in the isolate's life still sees intrinsics as they were before
user code ran.

Naming follows Node: statics keep their path (`JSONStringify`,
`ObjectDefineProperty`), instance methods are **uncurried** so the receiver
becomes the first argument:

```js
ArrayPrototypeSlice(list, 1)     // not list.slice(1)
FunctionPrototypeCall(cb, this, event)  // not cb.call(this, event)
```

Uncurrying is `Function.prototype.bind.bind(Function.prototype.call)`, which is
both faster than a captured `fn.call(...)` and immune to a replaced
`Function.prototype.call`.

Add only what a builtin actually needs; this is not a mirror of Node's list.
Plain constructor calls made once at init time (`new Map()` while
bootstrapping) may stay direct — the rule targets code in closures that
outlive init.
