#ifndef INTEROP_H_
#define INTEROP_H_

#include "JEnv.h"
#include "v8.h"

namespace tns {

/*
 * The `interop` global, mirroring the iOS runtime's interop namespace. On
 * Android it currently exposes a single member:
 *
 *   interop.escapeException(x)
 *
 * Brands an error so the JS->Java boundary (NativeScriptException::
 * ReThrowToJava) forwards the ORIGINAL Java throwable carried by `x` (its
 * `nativeException`, or `x` itself when it wraps a throwable) to the native
 * caller unwrapped - so a Java `catch` of the concrete exception type still
 * matches, instead of receiving a com.tns.NativeScriptException wrapper.
 * A branded error with no underlying Java throwable keeps the default escape
 * behavior (a com.tns.NativeScriptException carrying the JS message/stack).
 * Branded escapes bypass discardUncaughtJsExceptions, which only handles
 * com.tns.NativeScriptException.
 */
class Interop {
public:
    /*
     * Installs the `interop` object on the global. Evaluated once per isolate
     * during PrepareV8Runtime, for both the main and worker isolates.
     */
    static void Init(v8::Local<v8::Context> context);

    /*
     * Returns the original Java throwable carried by a branded
     * interop.escapeException(...) error, as a new local reference the caller
     * may env.Throw() directly, or nullptr when `errObj` is not branded or
     * carries no Java throwable.
     */
    static jthrowable ExtractEscapedJavaException(JEnv& env,
                                                  const v8::Local<v8::Object>& errObj);
};

}  // namespace tns

#endif /* INTEROP_H_ */
