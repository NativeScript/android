#include "V8StringConstants.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include "CallbackHandlers.h"
#include <sstream>

using namespace std;
using namespace tns;

Runtime* TryGetRuntime(int runtimeId);

namespace {
jint GenerateNewObjectIdFast(jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return 0;
    }

    return runtime->GenerateNewObjectId(nullptr, nullptr);
}

jboolean NotifyGcFast(JNIEnv* env, jclass, jobject runtimeObject, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return JNI_FALSE;
    }

    return runtime->NotifyGC(env, runtimeObject) ? JNI_TRUE : JNI_FALSE;
}

jint GetPointerSizeFast() {
    return sizeof(void*);
}

jint GetCurrentRuntimeIdFast() {
    Isolate* isolate = Isolate::TryGetCurrent();
    if (isolate == nullptr) {
        return -1;
    }

    Runtime* runtime = Runtime::GetRuntime(isolate);
    return runtime->GetId();
}

void SetManualInstrumentationModeFast(JNIEnv*, jclass, jstring mode) {
    Runtime::SetManualInstrumentationMode(mode);
}

void RegisterRuntimeNativeFastMethods(JNIEnv* env) {
    jclass runtimeNativeFastClass = env->FindClass("com/tns/RuntimeNativeFast");
    if (runtimeNativeFastClass == nullptr) {
        throw NativeScriptException("Unable to find com/tns/RuntimeNativeFast for JNI registration.");
    }

    JNINativeMethod methods[] = {
        { const_cast<char*>("generateNewObjectId"), const_cast<char*>("(I)I"), reinterpret_cast<void*>(GenerateNewObjectIdFast) },
        { const_cast<char*>("notifyGc"), const_cast<char*>("(Lcom/tns/Runtime;I)Z"), reinterpret_cast<void*>(NotifyGcFast) },
        { const_cast<char*>("getCurrentRuntimeId"), const_cast<char*>("()I"), reinterpret_cast<void*>(GetCurrentRuntimeIdFast) },
        { const_cast<char*>("getPointerSize"), const_cast<char*>("()I"), reinterpret_cast<void*>(GetPointerSizeFast) },
        { const_cast<char*>("SetManualInstrumentationMode"), const_cast<char*>("(Ljava/lang/String;)V"), reinterpret_cast<void*>(SetManualInstrumentationModeFast) },
    };

    if (env->RegisterNatives(runtimeNativeFastClass, methods, sizeof(methods) / sizeof(methods[0])) != JNI_OK) {
        throw NativeScriptException("Unable to register com/tns/RuntimeNativeFast native methods.");
    }

    env->DeleteLocalRef(runtimeNativeFastClass);
}
} // namespace

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    try {
        Runtime::Init(vm, reserved);
        JNIEnv* env = nullptr;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK || env == nullptr) {
            throw NativeScriptException("Unable to retrieve JNIEnv during JNI_OnLoad.");
        }
        RegisterRuntimeNativeFastMethods(env);
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_SetManualInstrumentationModeLegacy(JNIEnv*, jclass, jstring mode) {
    try {
        Runtime::SetManualInstrumentationMode(mode);
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_runWorker(JNIEnv* _env, jobject obj, jint runtimeId, jstring scriptFile) {
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
        runtime->RunWorker(scriptFile);
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

extern "C" JNIEXPORT jint Java_com_tns_Runtime_generateNewObjectIdLegacy(JNIEnv*, jclass, jint runtimeId) {
    try {
        return GenerateNewObjectIdFast(runtimeId);
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

extern "C" JNIEXPORT jboolean Java_com_tns_Runtime_notifyGcLegacy(JNIEnv* env, jobject obj, jint runtimeId) {
    return NotifyGcFast(env, nullptr, obj, runtimeId);
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

extern "C" JNIEXPORT jint Java_com_tns_Runtime_getPointerSizeLegacy(JNIEnv*, jclass) {
    return GetPointerSizeFast();
}

extern "C" JNIEXPORT jint Java_com_tns_Runtime_getCurrentRuntimeIdLegacy(JNIEnv*, jclass) {
    return GetCurrentRuntimeIdFast();
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_WorkerGlobalOnMessageCallback(JNIEnv* env, jobject obj, jint runtimeId, jstring msg) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_ClearWorkerPersistent(JNIEnv* env, jobject obj, jint runtimeId, jint workerId) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
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
