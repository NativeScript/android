# Runtime builtins

The `.js` files in this directory are the runtime's internal JavaScript. At
build time a CMake custom command runs `tools/js2c.mjs`, which embeds them into
`test-app/runtime/src/main/cpp/generated/RuntimeBuiltins.cpp`; at runtime
`BuiltinLoader::RunBuiltin` compiles and executes them with an
`internal/<name>.js` script origin and a process-wide bytecode cache.

## Contract (Node's module wrapper + internalBinding idiom)

Every file is compiled as a **function body** via `v8::ScriptCompiler::CompileFunction`
with the fixed parameters `exports`, `module` and `binding`:

```js
const { someNative, anotherNative } = binding;

module.exports = somethingTheCallSiteNeeds;
```

- `binding` is a plain object of natives built by the C++ call site; a file
  that needs nothing from C++ simply doesn't mention it.
- **`module.exports` is the export channel** — whatever it holds when the file
  finishes is what `RunBuiltin` hands back to C++ (used for factory functions
  and init results). Both CommonJS styles work: replace the whole export with
  `module.exports = x`, or hang properties off `exports`. A file that only
  installs globals exports nothing and the call site ignores the value.
- **No top-level `return`.** It would work — these are function bodies — but
  every tool that isn't reading this repo's ESLint config (editors' TS server,
  prettier, review bots) rejects the file as invalid JavaScript.
- Strict mode is per-file: start the file with `"use strict";` to opt in.
- Destructure `binding` once, at the top of the file, so the file's native
  dependencies are visible and greppable.

## Rules

- Run at isolate init, before any user code: capture any global you rely on
  (e.g. `globalThis.Event`) eagerly so later monkey-patching can't break you.
- No `import`/`export` — these are classic function bodies, not modules.
- ESLint (`eslint.config.mjs` at the repo root, `npm run lint`) declares
  `exports`, `module`, `binding` and the reachable native globals; `no-undef`
  is the typo net. If a builtin starts using a new native global, add it there.
- File names are kebab-case; the name determines the `BuiltinId` enum value
  (`weak-ref.js` → `kWeakRef`) and the script origin. New files must also be
  added to `RUNTIME_BUILTIN_JS` in `test-app/runtime/CMakeLists.txt` — the
  build fails with an explicit message if that list drifts out of sync
  (`js2c.mjs --check-dir`).
