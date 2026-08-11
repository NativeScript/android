#ifndef NSBUILTINMODULES_H_
#define NSBUILTINMODULES_H_

#include <string>

#include "v8.h"

namespace tns {

/*
 * The `ns:` builtin modules and their `node:` compatibility shims, as
 * specified by docs/ns-builtin-modules.md. Every module is a per-realm
 * singleton built from a RuntimeBuiltins source, so a specifier always yields
 * the same frozen exports object and the same synthetic ES module.
 */
class NsBuiltinModules {
public:
    /*
     * Any `ns:`/`node:` specifier, registered or not. The CommonJS resolver
     * claims all of them so a prefixed name can never reach the filesystem;
     * bare specifiers (`util`) keep resolving through npm untouched.
     */
    static bool IsBuiltinScheme(const std::string& specifier);

    /*
     * The `ns:` half of the scheme, whose unknown names must fail rather than
     * fall through to any legacy handling.
     */
    static bool IsNsScheme(const std::string& specifier);

    // Whether a module of that name exists.
    static bool IsRegistered(const std::string& specifier);

    /*
     * Frozen exports object of a registered specifier. Empty when the
     * specifier is not registered (nothing thrown) or when the module failed
     * to build (an exception is pending).
     */
    static v8::MaybeLocal<v8::Object> GetExports(v8::Local<v8::Context> context,
                                                 const std::string& specifier);

    /*
     * Evaluated synthetic module exporting the same values as GetExports under
     * their own names plus `default` (the exports object itself).
     */
    static v8::MaybeLocal<v8::Module> GetModule(v8::Local<v8::Context> context,
                                                const std::string& specifier);

    /*
     * What every resolver reports for a specifier in the scheme that names no
     * module.
     */
    static std::string NotFoundMessage(const std::string& specifier);

    /*
     * `ns:util`'s format, used by console.* for %-substitution. Empty when the
     * module could not be built; callers degrade instead of failing the log.
     */
    static v8::Local<v8::Function> GetFormatFunc(v8::Local<v8::Context> context);

    static void onDisposeIsolate(v8::Isolate* isolate);
};

}  // namespace tns

#endif /* NSBUILTINMODULES_H_ */
