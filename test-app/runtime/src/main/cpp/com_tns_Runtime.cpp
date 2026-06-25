#include "V8StringConstants.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include "CallbackHandlers.h"
#include <sstream>
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_initNativeScript(JNIEnv* _env, jobject obj, jint runtimeId, jstring filesPath, jstring nativeLibDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, jint maxLogcatObjectSize, jboolean forceLog) {
    try {
        Runtime::Init(_env, obj, runtimeId, filesPath, nativeLibDir, verboseLoggingEnabled, isDebuggable, packageName, args, callingDir, maxLogcatObjectSize, forceLog);
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_runModule(JNIEnv* _env, jobject obj, jint runtimeId, jstring scriptFile) {
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

extern "C" JNIEXPORT jobject Java_com_tns_Runtime_callJSMethodNative(JNIEnv* _env, jobject obj, jint runtimeId, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs) {
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
        result = runtime->CallJSMethodNative(_env, obj, javaObjectID, methodName, retType, isConstructor, packagedArgs);
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_createJSInstanceNative(JNIEnv* _env, jobject obj, jint runtimeId, jobject javaObject, jint javaObjectID, jstring className) {
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
        auto runtime = TryGetRuntime(runtimeId);
        if (runtime == nullptr) {
            return 0;
        }
        return runtime->GenerateNewObjectId(nullptr, nullptr);
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_passExceptionToJsNative(JNIEnv* env, jobject obj, jint runtimeId, jthrowable exception, jstring message, jstring fullStackTrace, jstring jsStackTrace, jboolean isDiscarded) {
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
        runtime->PassExceptionToJsNative(env, obj, exception, message, fullStackTrace, jsStackTrace, isDiscarded);
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_WorkerGlobalOnMessageCallback(JNIEnv* env, jobject obj, jint runtimeId, jstring msg) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
        return;
    }

    auto isolate = runtime->GetIsolate();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    CallbackHandlers::WorkerGlobalOnMessageCallback(isolate, msg);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_WorkerObjectOnMessageCallback(JNIEnv* env, jobject obj, jint runtimeId, jint workerId, jstring msg) {
    // Main Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
        return;
    }

    auto isolate = runtime->GetIsolate();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    CallbackHandlers::WorkerObjectOnMessageCallback(isolate, workerId, msg);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_TerminateWorkerCallback(JNIEnv* env, jobject obj, jint runtimeId) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
        return;
    }

    auto isolate = runtime->GetIsolate();

    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handleScope(isolate);

        CallbackHandlers::TerminateWorkerThread(isolate);

        runtime->DestroyRuntime();
    }

    isolate->Dispose();

    delete runtime;
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_TerminateRuntimeCallback(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
        return;
    }

    auto isolate = runtime->GetIsolate();

    {
        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handleScope(isolate);

        runtime->DestroyRuntime();
    }

    isolate->Dispose();

    delete runtime;
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_ClearWorkerPersistent(JNIEnv* env, jobject obj, jint runtimeId, jint workerId) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
        return;
    }

    auto isolate = runtime->GetIsolate();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    CallbackHandlers::ClearWorkerPersistent(workerId);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_CallWorkerObjectOnErrorHandleMain(JNIEnv* env, jobject obj, jint runtimeId, jint workerId, jstring message, jstring stackTrace, jstring filename, jint lineno, jstring threadName) {
    // Main Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
        return;
    }

    auto isolate = runtime->GetIsolate();
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    v8::Context::Scope context_scope(context);

    try {
        CallbackHandlers::CallWorkerObjectOnErrorHandle(isolate, workerId, message, stackTrace, filename, lineno, threadName);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    }
}
extern "C" JNIEXPORT void Java_com_tns_Runtime_ResetDateTimeConfigurationCache(JNIEnv* _env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return;
    }

    auto isolate = runtime->GetIsolate();
    isolate->DateTimeConfigurationChangeNotification(Isolate::TimeZoneDetection::kRedetect);
}
