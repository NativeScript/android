#include "V8StringConstants.h"
#include "Runtime.h"
#include "NativeScriptException.h"
#include "CallbackHandlers.h"
#include <sstream>
#include "com_tns_AssetExtractor.h"
#include "com_tns_AndroidJsV8Inspector.h"
using namespace std;
using namespace tns;

#define ANDROID_O 26

int getApiVersion(JNIEnv *env) {
    jint sdkInt = -1;
    bool success = true;
    jclass versionClass = env->FindClass("android/os/Build$VERSION");

    if (versionClass == nullptr) {
        success = false;
    }

    jfieldID sdkIntFieldID;
    if (success) {
        success = ((sdkIntFieldID = env->GetStaticFieldID(versionClass, "SDK_INT", "I")) != nullptr);
    }

    if (success) {
        sdkInt = env->GetStaticIntField(versionClass, sdkIntFieldID);
    }

    return sdkInt;
}

static JNICALL void SetManualInstrumentationMode(JNIEnv* _env, jclass _clazz, jstring mode) {
    try {
        Runtime::SetManualInstrumentationMode(mode);
    } catch (...) {
        NativeScriptException nsEx(std::string("Error: c++ exception!"));
        nsEx.ReThrowToJava();
    }
}

static JNICALL void initNativeScript(JNIEnv* _env, jobject obj, jint runtimeId, jstring filesPath, jstring nativeLibDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, jint maxLogcatObjectSize, jboolean forceLog) {
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

static JNICALL void runModule(JNIEnv* _env, jobject obj, jint runtimeId, jstring scriptFile) {
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

static  JNICALL void runWorker(JNIEnv* _env, jobject obj, jint runtimeId, jstring scriptFile) {
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

static JNICALL jobject runScript(JNIEnv* _env, jobject obj, jint runtimeId, jstring scriptFile) {
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

static JNICALL jobject callJSMethodNative(JNIEnv* _env, jobject obj, jint runtimeId, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs) {
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

static JNICALL void createJSInstanceNative(JNIEnv* _env, jobject obj, jint runtimeId, jobject javaObject, jint javaObjectID, jstring className) {
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

static JNICALL jint generateNewObjectId(JNIEnv* env, jobject obj, jint runtimeId) {
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
    // this is only to avoid warnings, we should never come here
    return 0;
}


static JNICALL jboolean notifyGc(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return JNI_FALSE;
    }

    jboolean success = runtime->NotifyGC(env, obj) ? JNI_TRUE : JNI_FALSE;
    return success;
}

static JNICALL void rtLock(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime != nullptr) {
        runtime->Lock();
    }
}

static JNICALL void rtUnlock(JNIEnv* env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime != nullptr) {
        runtime->Unlock();
    }
}

static JNICALL void passExceptionToJsNative(JNIEnv* env, jobject obj, jint runtimeId, jthrowable exception, jstring message, jstring fullStackTrace, jstring jsStackTrace, jboolean isDiscarded) {
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

static JNICALL jint getPointerSize(JNIEnv* env, jobject obj) {
    return sizeof(void*);
}

static JNICALL jint getCurrentRuntimeId(JNIEnv* _env, jobject obj) {
    Isolate* isolate = Isolate::TryGetCurrent();
    if (isolate == nullptr) {
        return -1;
    }

    Runtime* runtime = Runtime::GetRuntime(isolate);
    int id = runtime->GetId();
    return id;
}

static JNICALL void WorkerGlobalOnMessageCallback(JNIEnv* env, jobject obj, jint runtimeId, jstring msg) {
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

static JNICALL void WorkerObjectOnMessageCallback(JNIEnv* env, jobject obj, jint runtimeId, jint workerId, jstring msg) {
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

static JNICALL void TerminateWorkerCallback(JNIEnv* env, jobject obj, jint runtimeId) {
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

static JNICALL void ClearWorkerPersistent(JNIEnv* env, jobject obj, jint runtimeId, jint workerId) {
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

static JNICALL void CallWorkerObjectOnErrorHandleMain(JNIEnv* env, jobject obj, jint runtimeId, jint workerId, jstring message, jstring stackTrace, jstring filename, jint lineno, jstring threadName) {
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

static JNICALL void ResetDateTimeConfigurationCache(JNIEnv* _env, jobject obj, jint runtimeId) {
    auto runtime = TryGetRuntime(runtimeId);
    if (runtime == nullptr) {
        return;
    }

    auto isolate = runtime->GetIsolate();
    isolate->DateTimeConfigurationChangeNotification(Isolate::TimeZoneDetection::kRedetect);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    try {

        JNIEnv *env;
        if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
            return JNI_ERR;
        }

        int apiVersion = getApiVersion(env);

        if (apiVersion == -1) {
            __android_log_write(ANDROID_LOG_ERROR, "JS", "Could not get Android API version!");
            return JNI_ERR;
        }

        jint result;




        if (apiVersion >= ANDROID_O) {
            // Redirect func_bangJni to fastNativeFunc since !bang JNI is not supported anymore
            JNINativeMethod methodsInternal[] = {
                    { "SetManualInstrumentationMode", "(Ljava/lang/String;)V", (void *) SetManualInstrumentationMode },
                    { "initNativeScript", "(ILjava/lang/String;Ljava/lang/String;ZZLjava/lang/String;[Ljava/lang/Object;Ljava/lang/String;IZ)V", (void *) initNativeScript },
                    { "runModule", "(ILjava/lang/String;)V", (void *) runModule },
                    { "runWorker", "(ILjava/lang/String;)V", (void *) runWorker },
                    { "runScript", "(ILjava/lang/String;)Ljava/lang/Object;", (void *) runScript },
                    { "callJSMethodNative", "(IILjava/lang/String;IZ[Ljava/lang/Object;)Ljava/lang/Object;", (void *) callJSMethodNative },
                    { "createJSInstanceNative", "(ILjava/lang/Object;ILjava/lang/String;)V", (void *) createJSInstanceNative },
                    { "generateNewObjectId", "(I)I", (void *) generateNewObjectId },
                    { "notifyGc", "(I)Z", (void *) notifyGc },
                    { "lock", "(I)V", (void *) rtLock },
                    { "unlock", "(I)V", (void *) rtUnlock },
                    { "passExceptionToJsNative", "(ILjava/lang/Throwable;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V", (void *) passExceptionToJsNative },
                    { "getPointerSize", "()I", (void *) getPointerSize },
                    { "getCurrentRuntimeId", "()I", (void *) getCurrentRuntimeId },
                    { "WorkerGlobalOnMessageCallback", "(ILjava/lang/String;)V", (void *) WorkerGlobalOnMessageCallback },
                    { "WorkerObjectOnMessageCallback", "(IILjava/lang/String;)V", (void *) WorkerObjectOnMessageCallback },
                    { "TerminateWorkerCallback", "(I)V", (void *) TerminateWorkerCallback },
                    { "ClearWorkerPersistent", "(II)V", (void *) ClearWorkerPersistent },
                    { "CallWorkerObjectOnErrorHandleMain", "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V", (void *) CallWorkerObjectOnErrorHandleMain },
                    { "ResetDateTimeConfigurationCache", "(I)V", (void *) ResetDateTimeConfigurationCache },
            };


            result = env->RegisterNatives(
                    env->FindClass("com/tns/Runtime"),
                    methodsInternal,
                    sizeof(methodsInternal) / sizeof(JNINativeMethod));


            if (result != JNI_OK) {
                __android_log_print(ANDROID_LOG_ERROR, "JS", "Error code %d when registering native functions!",
                                    result);
                return JNI_ERR;
            }


            JNINativeMethod assetMethodsInternal[] = {
                    { "extractAssets", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V", (void *) extractAssets }
            };

            result = env->RegisterNatives(
                    env->FindClass("com/tns/AssetExtractor"),
                    assetMethodsInternal,
                    sizeof(assetMethodsInternal) / sizeof(JNINativeMethod));



            if (result != JNI_OK) {
                __android_log_print(ANDROID_LOG_ERROR, "JS", "Error code %d when registering native functions!",
                                    result);
                return JNI_ERR;
            }


            JNINativeMethod inspectorMethodsInternal[] = {
                    { "init", "()V", (void *) initCritical },
                    { "connect", "(Ljava/lang/Object;)V", (void *) connect },
                    { "scheduleBreak", "()V", (void *) scheduleBreakCritical },
                    { "disconnect", "()V", (void *) disconnectCritical },
                    { "dispatchMessage", "(Ljava/lang/String;)V", (void *) dispatchMessage }
            };

            result = env->RegisterNatives(
                    env->FindClass("com/tns/AndroidJsV8Inspector"),
                    inspectorMethodsInternal,
                    sizeof(inspectorMethodsInternal) / sizeof(JNINativeMethod));


        }
        else {
            // Redirect CriticalNative function to !bang JNI function (need JNIEnv and jclass on Android 7-)
            // !bang JNI notation used to demonstrate faster calls on Android 7-
            JNINativeMethod methodsInternal[] = {
                    { "SetManualInstrumentationMode", "!(Ljava/lang/String;)V", (void *) SetManualInstrumentationMode },
                    { "initNativeScript", "!(ILjava/lang/String;Ljava/lang/String;ZZLjava/lang/String;[Ljava/lang/Object;Ljava/lang/String;IZ)V", (void *) initNativeScript },
                    { "runModule", "!(ILjava/lang/String;)V", (void *) runModule },
                    { "runWorker", "!(ILjava/lang/String;)V", (void *) runWorker },
                    { "runScript", "!(ILjava/lang/String;)Ljava/lang/Object;", (void *) runScript },
                    { "callJSMethodNative", "!(IILjava/lang/String;IZ[Ljava/lang/Object;)Ljava/lang/Object;", (void *) callJSMethodNative },
                    { "createJSInstanceNative", "!(ILjava/lang/Object;ILjava/lang/String;)V", (void *) createJSInstanceNative },
                    { "generateNewObjectId", "!(I)I", (void *) generateNewObjectId },
                    { "notifyGc", "!(I)Z", (void *) notifyGc },
                    { "lock", "!(I)V", (void *) rtLock },
                    { "unlock", "!(I)V", (void *) rtUnlock },
                    { "passExceptionToJsNative", "!(ILjava/lang/Throwable;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V", (void *) passExceptionToJsNative },
                    { "getPointerSize", "!()I", (void *) getPointerSize },
                    { "getCurrentRuntimeId", "!()I", (void *) getCurrentRuntimeId },
                    { "WorkerGlobalOnMessageCallback", "!(ILjava/lang/String;)V", (void *) WorkerGlobalOnMessageCallback },
                    { "WorkerObjectOnMessageCallback", "!(IILjava/lang/String;)V", (void *) WorkerObjectOnMessageCallback },
                    { "TerminateWorkerCallback", "!(I)V", (void *) TerminateWorkerCallback },
                    { "ClearWorkerPersistent", "!(II)V", (void *) ClearWorkerPersistent },
                    { "CallWorkerObjectOnErrorHandleMain", "!(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V", (void *) CallWorkerObjectOnErrorHandleMain },
                    { "ResetDateTimeConfigurationCache", "!(I)V", (void *) ResetDateTimeConfigurationCache },
            };




            result = env->RegisterNatives(
                    env->FindClass("com/tns/Runtime"),
                    methodsInternal,
                    sizeof(methodsInternal) / sizeof(JNINativeMethod));




            if (result != JNI_OK) {
                __android_log_print(ANDROID_LOG_ERROR, "JS", "Error code %d when registering native functions!",
                                    result);
                return JNI_ERR;
            }


            JNINativeMethod assetMethodsInternal[] = {
                    { "extractAssets", "!(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V", (void *) extractAssets }
            };

            result = env->RegisterNatives(
                    env->FindClass("com/tns/AssetExtractor"),
                    assetMethodsInternal,
                    sizeof(assetMethodsInternal) / sizeof(JNINativeMethod));


            if (result != JNI_OK) {
                __android_log_print(ANDROID_LOG_ERROR, "JS", "Error code %d when registering native functions!",
                                    result);
                return JNI_ERR;
            }


            JNINativeMethod inspectorMethodsInternal[] = {
                    { "init", "!()V", (void *) init },
                    { "connect", "!(Ljava/lang/Object;)V", (void *) connect },
                    { "scheduleBreak", "!()V", (void *) scheduleBreak },
                    { "disconnect", "!()V", (void *) disconnect },
                    { "dispatchMessage", "!(Ljava/lang/String;)V", (void *) dispatchMessage }
            };

            result = env->RegisterNatives(
                    env->FindClass("com/tns/AndroidJsV8Inspector"),
                    inspectorMethodsInternal,
                    sizeof(inspectorMethodsInternal) / sizeof(JNINativeMethod));

        }

        if (result != JNI_OK) {
            __android_log_print(ANDROID_LOG_ERROR, "JS", "Error code %d when registering native functions!",
                                result);
            return JNI_ERR;
        }


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