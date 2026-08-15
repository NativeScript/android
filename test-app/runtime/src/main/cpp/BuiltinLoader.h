#ifndef BUILTINLOADER_H_
#define BUILTINLOADER_H_

#include "generated/RuntimeBuiltins.h"
#include "v8.h"

namespace tns {

class BuiltinLoader {
public:
    /*
     * Compiles the builtin identified by id as a function body with the fixed
     * parameters `exports`, `require`, `module`, `binding` (Node's module
     * wrapper plus its internalBinding idiom) and `primordials`, calls it with
     * the given bag of natives (or undefined when omitted) plus this isolate's
     * frozen intrinsics snapshot, and returns the resulting `module.exports`.
     * `require` reaches the builtin modules (NsBuiltinModules) and nothing
     * else. The snapshot is produced by the kPrimordials builtin on first use
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
};

}  // namespace tns

#endif /* BUILTINLOADER_H_ */
