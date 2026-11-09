#ifndef INTEROP_H_
#define INTEROP_H_

#include "JEnv.h"
#include "v8.h"

namespace tns {

/*
 * escapeException(x), exported by `ns:runtime` (and by the legacy `interop`
 * global, which js/interop-globals.js assembles from that module).
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
     * The brand payload of an escapeException(...) error, decoded at
     * the JS->Java boundary. `original` (a new local reference the caller may
     * env.Throw() directly) is nullptr for a branded error that carries no
     * Java throwable - the synthesized-escape case.
     */
    struct EscapedExceptionInfo {
        bool branded = false;
        jthrowable original = nullptr;
        std::string name;
        std::string message;
        /* The JS stack of the escaped error itself (may be empty). */
        std::string stack;
        /* The JS stack of the escapeException(...) call site. */
        std::string escapeSiteStack;
    };

    /*
     * A new escapeException function for the `ns:runtime` binding.
     */
    static v8::MaybeLocal<v8::Function> CreateEscapeException(v8::Local<v8::Context> context);

    /*
     * Decodes the brand payload of `errObj` into `out`. Returns true when the
     * object is branded.
     */
    static bool GetEscapedExceptionInfo(JEnv& env,
                                        const v8::Local<v8::Object>& errObj,
                                        EscapedExceptionInfo& out);

    /*
     * Attaches a com.tns.JavaScriptStackTrace carrier to `target` as a
     * suppressed exception, so the JS journey of an escaped original Java
     * throwable renders in stack dumps and crash reports. Idempotent (the
     * Java side skips duplicates).
     */
    static void AttachJavaScriptStackTrace(JEnv& env, jthrowable target,
                                           const EscapedExceptionInfo& info);

    /*
     * Replaces `target`'s stack trace with frames synthesized from the JS
     * stack (com.tns.JavaScriptStackTrace.applyFrames), so JS-originated
     * escapes group by their actual JS frames in crash reporters. Only used
     * on throwables the runtime itself constructs.
     */
    static void ApplyJavaScriptFrames(JEnv& env, jthrowable target,
                                      const EscapedExceptionInfo& info);
};

}  // namespace tns

#endif /* INTEROP_H_ */
