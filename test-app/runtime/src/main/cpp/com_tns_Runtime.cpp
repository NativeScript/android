#include "V8StringConstants.h"
#include "Runtime.h"
#include "WrapperLifetimeCanary.h"
#include "RuntimeGates.h"
#include "NativeScriptException.h"
#include "CallbackHandlers.h"
#include "ArgConverter.h"
#include "MetadataNode.h"
#include <cstring>
#include <optional>
#include <sstream>
#include <vector>
#include <android/log.h>

using namespace std;
using namespace tns;

// Forward declarations for natives that must be explicitly registered via
// RegisterNatives. Dynamic JNI lookup of @CriticalNative / @FastNative methods
// is unimplemented on Android 8-10 and buggy on Android 11, so for those
// versions the Java side dispatches to the *Legacy variants (auto-bound).
// On Android 12+ explicit registration also works, so we always register.
static jint generateNewObjectIdCritical_impl(jint runtimeId);
static jboolean notifyGcFast_impl(JNIEnv* env, jobject obj, jint runtimeId);
static jint getCurrentRuntimeIdCritical_impl();
static jint getPointerSizeCritical_impl();
static void setManualInstrumentationModeFast_impl(JNIEnv* env, jclass clazz, jstring mode);

static void RegisterOptimizedNatives(JNIEnv* env) {
    jclass cls = env->FindClass("com/tns/Runtime");
    if (cls == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "TNS.Native",
                            "Failed to find com/tns/Runtime for RegisterNatives");
        return;
    }

    // @CriticalNative: signatures must omit JNIEnv* / jclass.
    static const JNINativeMethod criticalMethods[] = {
        {const_cast<char*>("generateNewObjectIdCritical"), const_cast<char*>("(I)I"), reinterpret_cast<void*>(generateNewObjectIdCritical_impl)},
        {const_cast<char*>("getCurrentRuntimeIdCritical"), const_cast<char*>("()I"), reinterpret_cast<void*>(getCurrentRuntimeIdCritical_impl)},
        {const_cast<char*>("getPointerSizeCritical"),      const_cast<char*>("()I"), reinterpret_cast<void*>(getPointerSizeCritical_impl)},
    };
    // @FastNative: standard JNI ABI (JNIEnv*, jobject/jclass, ...).
    static const JNINativeMethod fastMethods[] = {
        {const_cast<char*>("notifyGcFast"),                       const_cast<char*>("(I)Z"),                  reinterpret_cast<void*>(notifyGcFast_impl)},
        {const_cast<char*>("setManualInstrumentationModeFast"),   const_cast<char*>("(Ljava/lang/String;)V"), reinterpret_cast<void*>(setManualInstrumentationModeFast_impl)},
    };

    if (env->RegisterNatives(cls, criticalMethods,
                             sizeof(criticalMethods) / sizeof(criticalMethods[0])) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "TNS.Native",
                            "RegisterNatives failed for @CriticalNative methods");
        env->ExceptionClear();
    }
    if (env->RegisterNatives(cls, fastMethods,
                             sizeof(fastMethods) / sizeof(fastMethods[0])) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, "TNS.Native",
                            "RegisterNatives failed for @FastNative methods");
        env->ExceptionClear();
    }
    env->DeleteLocalRef(cls);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    try {
        Runtime::Init(vm, reserved);

        JNIEnv* env = nullptr;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK && env != nullptr) {
            RegisterOptimizedNatives(env);
        }
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }

    return JNI_VERSION_1_6;
}

static void setManualInstrumentationModeFast_impl(JNIEnv* env, jclass clazz, jstring mode) {
    try {
        Runtime::SetManualInstrumentationMode(mode);
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}

// Auto-bound legacy variant for Android < 8.0 (annotation absent / unsupported).
extern "C" JNIEXPORT void Java_com_tns_Runtime_setManualInstrumentationModeLegacy(JNIEnv* _env, jclass clazz, jstring mode) {
    setManualInstrumentationModeFast_impl(_env, clazz, mode);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_initNativeScript(JNIEnv* _env, jobject obj, jint runtimeId, jstring filesPath, jstring nativeLibDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jstring v8Flags, jboolean codeCache, jstring profilerOutputDir, jboolean discardUncaughtJsExceptions, jstring uncaughtErrorPolicy, jstring wrapperLifetimeMode, jstring wrapperLifetimeScope, jstring callingDir, jint maxLogcatObjectSize, jboolean forceLog) {
    JEnv::EntryScope envScope(_env);
    try {
        Runtime::StartupOptions options{
            ArgConverter::jstringToString(v8Flags),
            codeCache == JNI_TRUE,
            ArgConverter::jstringToString(profilerOutputDir),
            discardUncaughtJsExceptions == JNI_TRUE,
            ArgConverter::jstringToString(uncaughtErrorPolicy),
            ArgConverter::jstringToString(wrapperLifetimeMode),
            ArgConverter::jstringToString(wrapperLifetimeScope)};
        Runtime::Init(_env, obj, runtimeId, filesPath, nativeLibDir, verboseLoggingEnabled, isDebuggable, packageName, options, callingDir, maxLogcatObjectSize, forceLog);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}

Runtime* TryGetRuntime(int runtimeId) {
    Runtime* runtime = nullptr;
    try {
        runtime = Runtime::GetRuntime(runtimeId);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
    return runtime;
}

namespace {

/*
 * A Java->JS entry's hold on a runtime: its Locker, after the admission gate
 * when `admit`. Entering another thread's runtime pins it first (RuntimeGates,
 * as the iOS runtime's native callbacks do), so a runtime tearing down while
 * this thread waits for the Locker cannot dispose its isolate or free itself
 * before the wait is over, and is found destroyed once the Locker is granted.
 * Get() is null when the call is dropped: the runtime is gone or shutting
 * down.
 */
class RuntimeEntry {
public:
    RuntimeEntry(jint runtimeId, bool admit) {
        auto runtime = Runtime::GetCurrentRuntime();
        // This thread's own runtime is torn down only on this thread.
        if (runtime == nullptr || runtime->GetId() != runtimeId) {
            m_pin.emplace(runtimeId);
            runtime = m_pin->runtime();
            if (runtime == nullptr) {
                return;
            }
        }
        auto isolate = runtime->GetIsolate();
        // A runtime whose bootstrap failed before it had an isolate.
        if (isolate == nullptr) {
            return;
        }
        if (admit) {
            m_admission.emplace(runtime->GetAdmissionGate(), isolate);
        }
        m_locker.emplace(isolate);
        if (runtime->IsDestroyed()) {
            return;
        }
        m_runtime = runtime;
    }

    tns::Runtime* Get() const { return m_runtime; }

private:
    tns::Runtime* m_runtime = nullptr;
    // Released in reverse: the Locker, the admission gate, then the pin.
    std::optional<RuntimePin> m_pin;
    std::optional<AdmissionScope> m_admission;
    std::optional<v8::Locker> m_locker;
};

// Same tag and wording as the Java side's dropped calls (Runtime.logDiscardedCall).
void LogDropped(jint runtimeId, const std::string& what) {
    __android_log_print(ANDROID_LOG_WARN, "Warning",
                        "NativeScript discarding %s: runtime id=%d is shutting down",
                        what.c_str(), runtimeId);
}

}  // namespace

extern "C" JNIEXPORT void Java_com_tns_Runtime_runModule(JNIEnv* _env, jobject obj, jint runtimeId, jstring scriptFile) {
    JEnv::EntryScope envScope(_env);
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return;
    }

    auto isolate = runtime->GetIsolate();
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    try {
        runtime->RunModule(_env, obj, scriptFile);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}

extern "C" JNIEXPORT jobject Java_com_tns_Runtime_runScript(JNIEnv* _env, jobject obj, jint runtimeId, jstring scriptFile) {
    JEnv::EntryScope envScope(_env);
    jobject result = nullptr;

    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return result;
    }

    auto isolate = runtime->GetIsolate();
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    try {
        result = runtime->RunScript(_env, obj, scriptFile);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
    return result;
}

extern "C" JNIEXPORT jobject Java_com_tns_Runtime_callJSMethodNative(JNIEnv* _env, jobject obj, jint runtimeId, jobject javaObject, jint javaObjectID, jstring methodName, jint methodNameId, jint retType, jboolean isConstructor, jint argCount, jlongArray argValues, jobjectArray argRefs, jlongArray primitiveResult) {
    JEnv::EntryScope envScope(_env);
    jobject result = nullptr;

    RuntimeEntry entry(runtimeId, true);
    auto runtime = entry.Get();
    if (runtime == nullptr) {
        LogDropped(runtimeId, "call to \"" + ArgConverter::jstringToString(methodName) + "\"");
        return result;
    }

    auto isolate = runtime->GetIsolate();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    JavaCallArgs args{argCount, argValues, argRefs};
    try {
        result = runtime->CallJSMethodNative(_env, obj, javaObject, javaObjectID, methodName, methodNameId, retType, isConstructor, args, primitiveResult);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
    auto objectManager = runtime->GetObjectManager();
    // Objects Java registered for a call that never reached them would stay strong.
    objectManager->ReleaseUnwrappedArgs(args);
    // Wrappers collected during the call, settled while the isolate is still held.
    objectManager->FlushFinalizedBatchIfPending();
    return result;
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_createJSInstanceNative(JNIEnv* _env, jobject obj, jint runtimeId, jobject javaObject, jint javaObjectID, jstring className) {
    JEnv::EntryScope envScope(_env);
    RuntimeEntry entry(runtimeId, true);
    auto runtime = entry.Get();
    if (runtime == nullptr) {
        LogDropped(runtimeId, "the JS instance of a " + ArgConverter::jstringToString(className));
        return;
    }

    auto isolate = runtime->GetIsolate();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    try {
        runtime->CreateJSInstanceNative(_env, obj, javaObject, javaObjectID, className);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}

// @CriticalNative ABI: no JNIEnv* / jclass.
static jint generateNewObjectIdCritical_impl(jint runtimeId) {
    try {
        auto current = Runtime::GetCurrentRuntime();
        if (current != nullptr && current->GetId() == runtimeId) {
            return current->GenerateNewObjectId(nullptr, nullptr);
        }
        // Java registers a call's arguments on the calling thread, without the
        // isolate lock, so this can run on any thread while the runtime tears
        // down. -1 is ObjectTable.NO_ID; Java drops the call it was for.
        return RuntimeGates::TryGenerateObjectId(runtimeId);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
    // this is only to avoid warnings, we should never come here
    return 0;
}

extern "C" JNIEXPORT jint Java_com_tns_Runtime_generateNewObjectIdLegacy(JNIEnv* env, jclass clazz, jint runtimeId) {
    return generateNewObjectIdCritical_impl(runtimeId);
}

// @FastNative ABI: standard JNI signature.
static jboolean notifyGcFast_impl(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return JNI_FALSE;
    }

    jboolean success = runtime->NotifyGC(env, obj) ? JNI_TRUE : JNI_FALSE;
    return success;
}

extern "C" JNIEXPORT jboolean Java_com_tns_Runtime_notifyGcLegacy(JNIEnv* env, jobject obj, jint runtimeId) {
    return notifyGcFast_impl(env, obj, runtimeId);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_lock(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime != nullptr) {
        runtime->Lock();
    }
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_unlock(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime != nullptr) {
        runtime->Unlock();
    }
}

extern "C" JNIEXPORT jboolean Java_com_tns_Runtime_passExceptionToJsNative(JNIEnv* env, jobject obj, jint runtimeId, jthrowable exception, jstring message, jstring fullStackTrace, jstring jsStackTrace, jboolean isDiscarded) {
    RuntimeEntry entry(runtimeId, false);
    auto runtime = entry.Get();
    if (runtime == nullptr) {
        LogDropped(runtimeId, "an exception report");
        return JNI_FALSE;
    }

    auto isolate = runtime->GetIsolate();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    try {
        return runtime->PassExceptionToJsNative(env, obj, exception, message, fullStackTrace, jsStackTrace, isDiscarded);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    } catch (std::exception e) {
        stringstream ss;
        ss << "Error: c++ exception: " << e.what() << endl;
        NativeScriptException nsEx(ss.str());
        nsEx.ReThrowToJava();
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
    return JNI_FALSE;
}

// @CriticalNative ABI: no JNIEnv* / jclass.
static jint getPointerSizeCritical_impl() {
    return sizeof(void*);
}

extern "C" JNIEXPORT jint Java_com_tns_Runtime_getPointerSizeLegacy(JNIEnv* env, jclass clazz) {
    return getPointerSizeCritical_impl();
}

// @CriticalNative ABI: no JNIEnv* / jclass.
static jint getCurrentRuntimeIdCritical_impl() {
    Isolate* isolate = Isolate::TryGetCurrent();
    if (isolate == nullptr) {
        return -1;
    }

    Runtime* runtime = Runtime::GetRuntime(isolate);
    int id = runtime->GetId();
    return id;
}

extern "C" JNIEXPORT jint Java_com_tns_Runtime_getCurrentRuntimeIdLegacy(JNIEnv* env, jclass clazz) {
    return getCurrentRuntimeIdCritical_impl();
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_ResetDateTimeConfigurationCache(JNIEnv* _env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return;
    }

    auto isolate = runtime->GetIsolate();
    isolate->DateTimeConfigurationChangeNotification(Isolate::TimeZoneDetection::kRedetect);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_initContentKeyedBindings(JNIEnv* _env, jclass clazz, jobjectArray collisions) {
    vector<string> names;
    jsize length = collisions == nullptr ? 0 : _env->GetArrayLength(collisions);
    for (jsize i = 0; i < length; i++) {
        auto name = static_cast<jstring>(_env->GetObjectArrayElement(collisions, i));
        if (name != nullptr) {
            names.push_back(ArgConverter::jstringToString(name));
            _env->DeleteLocalRef(name);
        }
    }
    MetadataNode::InitContentKeyedBindings(names);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_setAdmissionSpinMicros(JNIEnv* _env, jclass clazz, jint micros) {
    AdmissionGate::s_spinMicros.store(micros, std::memory_order_relaxed);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_registerInterfaceProxyClass(JNIEnv* env, jclass, jstring proxyClassName, jstring interfaceName) {
    try {
        MetadataNode::RegisterInterfaceProxyClass(ArgConverter::jstringToString(proxyClassName), ArgConverter::jstringToString(interfaceName));
    } catch (NativeScriptException& e) {
        __android_log_print(ANDROID_LOG_WARN, "JS", "No metadata for the interface of %s: %s",
                            ArgConverter::jstringToString(proxyClassName).c_str(), e.GetErrorMessage().c_str());
    }
}

// Raw JNI throughout: an exception the method throws must stay pending, so that it reaches
// the Java caller unchanged.
extern "C" JNIEXPORT jobject Java_com_tns_Runtime_invokeNonvirtualNative(JNIEnv* env, jclass, jobject receiver, jobject method, jclass declaringClass, jstring shorty,
                                                                          jlongArray primitiveArgs, jobjectArray args, jlongArray primitiveResult) {
    jmethodID mid = env->FromReflectedMethod(method);

    const char* chars = env->GetStringUTFChars(shorty, nullptr);
    string types(chars);
    env->ReleaseStringUTFChars(shorty, chars);

    size_t argc = types.size() - 1;
    vector<jvalue> values(argc > 0 ? argc : 1);
    vector<jlong> bits(argc);
    if (argc > 0 && primitiveArgs != nullptr) {
        env->GetLongArrayRegion(primitiveArgs, 0, static_cast<jsize>(argc), bits.data());
    }
    for (size_t i = 0; i < argc; i++) {
        jvalue& value = values[i];
        switch (types[i + 1]) {
            case 'Z': value.z = bits[i] != 0 ? JNI_TRUE : JNI_FALSE; break;
            case 'B': value.b = static_cast<jbyte>(bits[i]); break;
            case 'C': value.c = static_cast<jchar>(bits[i]); break;
            case 'S': value.s = static_cast<jshort>(bits[i]); break;
            case 'I': value.i = static_cast<jint>(bits[i]); break;
            case 'J': value.j = bits[i]; break;
            case 'F': {
                auto raw = static_cast<int32_t>(bits[i]);
                memcpy(&value.f, &raw, sizeof(value.f));
                break;
            }
            case 'D': memcpy(&value.d, &bits[i], sizeof(value.d)); break;
            default: value.l = env->GetObjectArrayElement(args, static_cast<jsize>(i)); break;
        }
    }

    jlong result = 0;
    jobject objectResult = nullptr;
    switch (types[0]) {
        case 'V': env->CallNonvirtualVoidMethodA(receiver, declaringClass, mid, values.data()); break;
        case 'Z': result = env->CallNonvirtualBooleanMethodA(receiver, declaringClass, mid, values.data()); break;
        case 'B': result = env->CallNonvirtualByteMethodA(receiver, declaringClass, mid, values.data()); break;
        case 'C': result = env->CallNonvirtualCharMethodA(receiver, declaringClass, mid, values.data()); break;
        case 'S': result = env->CallNonvirtualShortMethodA(receiver, declaringClass, mid, values.data()); break;
        case 'I': result = env->CallNonvirtualIntMethodA(receiver, declaringClass, mid, values.data()); break;
        case 'J': result = env->CallNonvirtualLongMethodA(receiver, declaringClass, mid, values.data()); break;
        case 'F': {
            jfloat f = env->CallNonvirtualFloatMethodA(receiver, declaringClass, mid, values.data());
            int32_t raw;
            memcpy(&raw, &f, sizeof(raw));
            result = raw;
            break;
        }
        case 'D': {
            jdouble d = env->CallNonvirtualDoubleMethodA(receiver, declaringClass, mid, values.data());
            memcpy(&result, &d, sizeof(result));
            break;
        }
        default: objectResult = env->CallNonvirtualObjectMethodA(receiver, declaringClass, mid, values.data()); break;
    }

    if (primitiveResult != nullptr && !env->ExceptionCheck()) {
        env->SetLongArrayRegion(primitiveResult, 0, 1, &result);
    }
    return objectResult;
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_notifyCanaryObjectsCollected(JNIEnv* env, jclass, jint runtimeId, jintArray ids, jint count) {
    if (count <= 0) {
        return;
    }
    std::vector<jint> values(count);
    env->GetIntArrayRegion(ids, 0, count, values.data());
    WrapperLifetimeCanary::NotifyCollected(runtimeId, values.data(), count);
}
