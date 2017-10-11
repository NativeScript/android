#include "V8StringConstants.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include "CallbackHandlers.h"
#include <sstream>

using namespace std;
using namespace tns;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    try {
        Runtime::Init(vm, reserved);
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_SetManualInstrumentationMode(JNIEnv* _env, jobject obj, jstring mode) {
    try {
        Runtime::SetManualInstrumentationMode(mode);
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_initNativeScript(JNIEnv* _env, jobject obj, jint runtimeId, jstring filesPath, jstring nativeLibDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jobjectArray args, jstring callingDir) {
    try {
        Runtime::Init(_env, obj, runtimeId, filesPath, nativeLibDir, verboseLoggingEnabled, isDebuggable, packageName, args, callingDir);
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
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

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
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

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
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

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
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

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
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

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

extern "C" JNIEXPORT jint Java_com_tns_Runtime_generateNewObjectId(JNIEnv* env, jobject obj, jint runtimeId) {
    try {
        auto runtime = TryGetRuntime(runtimeId);
        if (runtime == nullptr) {
            return 0;
        }
        return runtime->GenerateNewObjectId(env, obj);
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

extern "C" JNIEXPORT jboolean Java_com_tns_Runtime_notifyGc(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return JNI_FALSE;
    }

    jboolean success = runtime->NotifyGC(env, obj) ? JNI_TRUE : JNI_FALSE;
    return success;
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_passUncaughtExceptionToJsNative(JNIEnv* env, jobject obj, jint runtimeId, jthrowable exception, jstring stackTrace) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return;
    }

    auto isolate = runtime->GetIsolate();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    try {
        runtime->PassUncaughtExceptionToJsNative(env, obj, exception, stackTrace);
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

extern "C" JNIEXPORT void Java_com_tns_Runtime_clearStartupData(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return;
    }

    runtime->ClearStartupData(env, obj);
}

extern "C" JNIEXPORT jint Java_com_tns_Runtime_getPointerSize(JNIEnv* env, jobject obj) {
    return sizeof(void*);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_WorkerGlobalOnMessageCallback(JNIEnv* env, jobject obj, jint runtimeId, jstring msg) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
    }

    auto isolate = runtime->GetIsolate();

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    CallbackHandlers::WorkerGlobalOnMessageCallback(isolate, msg);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_WorkerObjectOnMessageCallback(JNIEnv* env, jobject obj, jint runtimeId, jint workerId, jstring msg) {
    // Main Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
    }

    auto isolate = runtime->GetIsolate();

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    CallbackHandlers::WorkerObjectOnMessageCallback(isolate, workerId, msg);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_TerminateWorkerCallback(JNIEnv* env, jobject obj, jint runtimeId) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
    }

    auto isolate = runtime->GetIsolate();

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    CallbackHandlers::TerminateWorkerThread(isolate);

    runtime->DestroyRuntime();
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_ClearWorkerPersistent(JNIEnv* env, jobject obj, jint runtimeId, jint workerId) {
    // Worker Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
    }

    auto isolate = runtime->GetIsolate();

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    CallbackHandlers::ClearWorkerPersistent(workerId);
}

extern "C" JNIEXPORT void Java_com_tns_Runtime_CallWorkerObjectOnErrorHandleMain(JNIEnv* env, jobject obj, jint runtimeId, jint workerId, jstring message, jstring stackTrace, jstring filename, jint lineno, jstring threadName) {
    // Main Thread runtime
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        // TODO: Pete: Log message informing the developer of the failure
    }

    auto isolate = runtime->GetIsolate();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    try {
        CallbackHandlers::CallWorkerObjectOnErrorHandle(isolate, workerId, message, stackTrace, filename, lineno, threadName);
    } catch (NativeScriptException& e) {
        e.ReThrowToJava();
    }
}