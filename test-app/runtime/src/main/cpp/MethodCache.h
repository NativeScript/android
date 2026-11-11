#ifndef METHODCACHE_H_
#define METHODCACHE_H_

#include <string>
#include <map>
#include "v8.h"
#include "JEnv.h"
#include "MetadataEntry.h"
#include "ArgsWrapper.h"

namespace tns {
/*
 * MethodCache: class dealing with method/constructor resolution.
 */
class MethodCache {
    public:
        /*
         * CacheMethodInfo: struct holding resolved methods/constructor resolution
         */
        struct CacheMethodInfo {
            CacheMethodInfo()
                :
                retType(MethodReturnType::Unknown), mid(nullptr), clazz(nullptr), isStatic(false) {
            }
            std::string signature;
            std::string returnType;
            MethodReturnType retType;
            jmethodID mid;
            jclass clazz;
            bool isStatic;
        };

        static void Init();

        // The returned entry lives as long as the process; mid is null when nothing matched.
        static const CacheMethodInfo& ResolveMethodSignature(const std::string& className, const std::string& methodName, const v8::FunctionCallbackInfo<v8::Value>& args, bool isStatic);

        static const CacheMethodInfo& ResolveConstructorSignature(const ArgsWrapper& argWrapper, const std::string& fullClassName, jclass javaClass, bool isInterface);

    private:
        MethodCache() {
        }

        static void EncodeSignature(std::string& out, const std::string& className, const std::string& methodName, const v8::FunctionCallbackInfo<v8::Value>& args, bool isStatic);

        static void AppendType(std::string& out, v8::Isolate* isolate, const v8::Local<v8::Value>& value);

        static std::string ResolveJavaMethod(const v8::FunctionCallbackInfo<v8::Value>& args, const std::string& className, const std::string& methodName);

        static std::string ResolveConstructor(const v8::FunctionCallbackInfo<v8::Value>& args, jclass javaClass, bool isInterface);

        static jclass RUNTIME_CLASS;

        static jmethodID RESOLVE_METHOD_OVERLOAD_METHOD_ID;

        static jmethodID RESOLVE_CONSTRUCTOR_SIGNATURE_ID;

        /*
         * "s_mthod_ctor_signature_cache" holding all resolved CacheMethodInfo against an encoded_signature string.
         *  Used for caching the resolved constructor or method signature.
         * The encoded signature has template: <className>.S/I.<methodName>.<argsCount>.<arg1class>.<...>
         * A node map, never erased from: callers keep references to its entries.
         */
        static robin_hood::unordered_node_map<std::string, CacheMethodInfo> s_mthod_ctor_signature_cache;
};
}

#endif /* METHODCACHE_H_ */
