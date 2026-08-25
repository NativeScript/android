#ifndef BUILTINLOADER_H_
#define BUILTINLOADER_H_

#include "generated/RuntimeBuiltins.h"
#include "v8.h"

namespace tns {

class BuiltinLoader {
public:
    /*
     * Builds the bag of natives a builtin receives as its `binding` parameter.
     * GetExports calls it only when the builtin actually runs, so a call site
     * that hits the cache pays nothing for it.
     */
    using BindingFactory = v8::MaybeLocal<v8::Object> (*)(v8::Local<v8::Context>);

    /*
     * Compiles the builtin identified by id as a function body with the fixed
     * parameters `exports`, `require`, `module`, `binding` (Node's module
     * wrapper plus its internalBinding idiom) and `primordials`, calls it
     * with the given bag of natives (or undefined when omitted) and this
     * isolate's frozen intrinsics snapshot, and returns the resulting
     * `module.exports`. `require` reaches the builtin modules
     * (NsBuiltinModules) — including the internal-only tier, which is also
     * how builtins hand each other capabilities app code must not see — and
     * nothing else.
     * The snapshot is produced by the kPrimordials builtin on first use
     * and cached per isolate, so it is taken before any user code can replace
     * a global.
     * Scripts carry an "internal/<name>.js" origin so runtime frames are
     * identifiable in stack traces. Compilation goes through a process-wide
     * bytecode cache: the first run in the process compiles eagerly and
     * populates the cache, later isolates (workers, which run on their own
     * threads) consume it instead of re-parsing the source.
     */
    static v8::MaybeLocal<v8::Value> RunBuiltin(
            v8::Local<v8::Context> context, BuiltinId id,
            v8::Local<v8::Value> binding = v8::Local<v8::Value>());

    /*
     * The builtin's `module.exports`, running it at most once per isolate.
     * Every entry point that reaches the same file — the `ns:`/`node:` module
     * registry, the lazy-global tier, another builtin's `require` — shares
     * that one run, so a value a file exports is the same object through all
     * of them. Empty when the builtin failed to run (an exception is pending)
     * or exported a non-object.
     */
    static v8::MaybeLocal<v8::Object> GetExports(v8::Local<v8::Context> context,
                                                 BuiltinId id,
                                                 BindingFactory binding);
};

}  // namespace tns

#endif /* BUILTINLOADER_H_ */
