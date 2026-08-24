#ifndef LAZYGLOBALS_H_
#define LAZYGLOBALS_H_

#include "v8.h"

namespace tns {

/*
 * Globals whose implementation is a runtime builtin that must not run until
 * someone actually reaches for the name. Each entry is registered on the
 * global template as a lazy data property; the first read runs the builtin
 * once per isolate and caches its exports, so sibling names (TextEncoder and
 * TextDecoder) share the run, and V8 then replaces the property with a plain
 * data property so later reads cost nothing.
 *
 * A builtin behind this tier runs at an arbitrary point in the isolate's life
 * rather than during init, so it may only consume `internals` keys published
 * by eager builtins (see src/main/cpp/js/README.md).
 */
class LazyGlobals {
public:
    /*
     * Registers every lazy global. Must run before Context::New, on the same
     * template the eager globals use.
     */
    static void Init(v8::Isolate* isolate,
                     v8::Local<v8::ObjectTemplate> globalTemplate);
};

}  // namespace tns

#endif /* LAZYGLOBALS_H_ */
