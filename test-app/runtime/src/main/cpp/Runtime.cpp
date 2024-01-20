#include "CallbackHandlers.h"
#include "MetadataNode.h"
#include "JsArgConverter.h"
#include "JsArgToArrayConverter.h"
#include "ArgConverter.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "Constants.h"
#include "libplatform/libplatform.h"
#include "Version.h"
#include "WeakRef.h"
#include "NativeScriptAssert.h"
#include "SimpleProfiler.h"
#include "SimpleAllocator.h"
#include "ModuleInternal.h"
#include "NativeScriptException.h"
#include "Runtime.h"
#include "ArrayHelper.h"
#include "include/libplatform/libplatform.h"
#include "include/zipconf.h"
#include <csignal>
#include <sstream>
#include <mutex>
#include <dlfcn.h>
#include <console/Console.h>
#include "sys/system_properties.h"
#include "ManualInstrumentation.h"
#include "IsolateDisposer.h"
#include <unistd.h>
#include <thread>
#include "File.h"
#include "ModuleBinding.h"
#include "URLImpl.h"
#include "URLSearchParamsImpl.h"

#ifdef APPLICATION_IN_DEBUG
// #include "NetworkDomainCallbackHandlers.h"
#include "JsV8InspectorClient.h"
#endif

using namespace v8;
using namespace std;
using namespace tns;

bool tns::LogEnabled = true;
SimpleAllocator g_allocator;

void SIG_handler(int sigNumber) {
    stringstream msg;
    msg << "JNI Exception occurred (";
    switch (sigNumber) {
        case SIGABRT:
            msg << "SIGABRT";
            break;
        case SIGSEGV:
            msg << "SIGSEGV";
            break;
        default:
            // Shouldn't happen, but for completeness
            msg << "Signal #" << sigNumber;
            break;
    }
    msg << ").\n=======\nCheck the 'adb logcat' for additional information about the error.\n=======\n";
    throw NativeScriptException(msg.str());
}

void Runtime::Init(JavaVM* vm, void* reserved) {
    __android_log_print(ANDROID_LOG_INFO, "TNS.Runtime", "NativeScript Runtime Version %s, commit %s", NATIVE_SCRIPT_RUNTIME_VERSION, NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);

    if (Runtime::s_jvm == nullptr) {
        s_jvm = vm;

        JEnv::Init(s_jvm);
    }

    // handle SIGABRT/SIGSEGV only on API level > 20 as the handling is not so efficient in older versions
    if (m_androidVersion > 20) {
        struct sigaction action;
        action.sa_handler = SIG_handler;
        sigaction(SIGABRT, &action, NULL);
        sigaction(SIGSEGV, &action, NULL);
    }
}

int Runtime::GetAndroidVersion() {
    char sdkVersion[PROP_VALUE_MAX];
    __system_property_get("ro.build.version.sdk", sdkVersion);

    stringstream strValue;
    strValue << sdkVersion;

    unsigned int intValue;
    strValue >> intValue;

    return intValue;
}

Runtime::Runtime(JNIEnv* env, jobject runtime, int id)
    : m_id(id), m_isolate(nullptr), m_lastUsedMemory(0), m_gcFunc(nullptr), m_runGC(false) {
    m_runtime = env->NewGlobalRef(runtime);
    m_objectManager = new ObjectManager(m_runtime);
    m_loopTimer = new MessageLoopTimer();
    s_id2RuntimeCache.emplace(id, this);

    if (GET_USED_MEMORY_METHOD_ID == nullptr) {
        auto RUNTIME_CLASS = env->FindClass("com/tns/Runtime");
        assert(RUNTIME_CLASS != nullptr);

        GET_USED_MEMORY_METHOD_ID = env->GetMethodID(RUNTIME_CLASS, "getUsedMemory", "()J");
        assert(GET_USED_MEMORY_METHOD_ID != nullptr);
    }
}

Runtime* Runtime::GetRuntime(int runtimeId) {
    auto itFound = s_id2RuntimeCache.find(runtimeId);
    auto runtime = (itFound != s_id2RuntimeCache.end())
                   ? itFound->second
                   : nullptr;

    if (runtime == nullptr) {
        stringstream ss;
        ss << "Cannot find runtime for id:" << runtimeId;
        throw NativeScriptException(ss.str());
    }

    return runtime;
}

Runtime* Runtime::GetRuntime(v8::Isolate* isolate) {
    auto runtime = s_isolate2RuntimesCache.at(isolate);

    if (runtime == nullptr) {
        stringstream ss;
        ss << "Cannot find runtime for isolate: " << isolate;
        throw NativeScriptException(ss.str());
    }

    return runtime;
}

Runtime* Runtime::GetRuntimeFromIsolateData(v8::Isolate* isolate) {
    void* maybeRuntime = isolate->GetData((uint32_t)Runtime::IsolateData::RUNTIME);
    auto runtime = static_cast<Runtime*>(maybeRuntime);

    if (runtime == nullptr) {
        stringstream ss;
        ss << "Cannot find runtime for isolate: " << isolate;
        throw NativeScriptException(ss.str());
    }

    return runtime;
}

ObjectManager* Runtime::GetObjectManager(v8::Isolate* isolate) {
    return GetRuntime(isolate)->GetObjectManager();
}

Isolate* Runtime::GetIsolate() const {
    return m_isolate;
}

jobject Runtime::GetJavaRuntime() const {
    return m_runtime;
}

ObjectManager* Runtime::GetObjectManager() const {
    return m_objectManager;
}

void Runtime::Init(JNIEnv* _env, jobject obj, int runtimeId, jstring filesPath, jstring nativeLibDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, int maxLogcatObjectSize, bool forceLog) {
    JEnv env(_env);

    auto runtime = new Runtime(env, obj, runtimeId);

    auto enableLog = verboseLoggingEnabled == JNI_TRUE;

    runtime->Init(env, filesPath, nativeLibDir, enableLog, isDebuggable, packageName, args, callingDir, maxLogcatObjectSize, forceLog);
}

void Runtime::Init(JNIEnv* env, jstring filesPath, jstring nativeLibDir, bool verboseLoggingEnabled, bool isDebuggable, jstring packageName, jobjectArray args, jstring callingDir, int maxLogcatObjectSize, bool forceLog) {
    LogEnabled = verboseLoggingEnabled;

    auto filesRoot = ArgConverter::jstringToString(filesPath);
    auto nativeLibDirStr = ArgConverter::jstringToString(nativeLibDir);
    auto packageNameStr = ArgConverter::jstringToString(packageName);
    auto callingDirStr = ArgConverter::jstringToString(callingDir);

    Constants::APP_ROOT_FOLDER_PATH = filesRoot + "/app/";
    // read config options passed from Java
    // Indices correspond to positions in the com.tns.AppConfig.KnownKeys enum
    JniLocalRef v8Flags(env->GetObjectArrayElement(args, 0));
    Constants::V8_STARTUP_FLAGS = ArgConverter::jstringToString(v8Flags);
    JniLocalRef cacheCode(env->GetObjectArrayElement(args, 1));
    Constants::V8_CACHE_COMPILED_CODE = (bool) cacheCode;
    JniLocalRef profilerOutputDir(env->GetObjectArrayElement(args, 2));

    DEBUG_WRITE("Initializing Telerik NativeScript");

    auto profilerOutputDirStr = ArgConverter::jstringToString(profilerOutputDir);

    NativeScriptException::Init();
    m_isolate = PrepareV8Runtime(filesRoot, nativeLibDirStr, packageNameStr, isDebuggable, callingDirStr, profilerOutputDirStr, maxLogcatObjectSize, forceLog);
}

Runtime::~Runtime() {
    delete this->m_objectManager;
    delete this->m_loopTimer;
    CallbackHandlers::RemoveIsolateEntries(m_isolate);
    if (m_isMainThread) {
        if (m_mainLooper_fd[0] != -1) {
            ALooper_removeFd(m_mainLooper, m_mainLooper_fd[0]);
        }
        ALooper_release(m_mainLooper);

        if (m_mainLooper_fd[0] != -1) {
            close(m_mainLooper_fd[0]);
        }

        if (m_mainLooper_fd[1] != -1) {
            close(m_mainLooper_fd[1]);
        }
    }
}

std::string Runtime::ReadFileText(const std::string& filePath) {
#ifdef APPLICATION_IN_DEBUG
    std::lock_guard<std::mutex> lock(m_fileWriteMutex);
#endif
    return File::ReadText(filePath);
}


void Runtime::Lock() {
#ifdef APPLICATION_IN_DEBUG
    m_fileWriteMutex.lock();
#endif
}

void Runtime::Unlock() {
#ifdef APPLICATION_IN_DEBUG
    m_fileWriteMutex.unlock();
#endif
}

void Runtime::RunModule(JNIEnv* _env, jobject obj, jstring scriptFile) {
    JEnv env(_env);

    string filePath = ArgConverter::jstringToString(scriptFile);
    auto context = this->GetContext();
    m_module.Load(context, filePath);
}

void Runtime::RunWorker(jstring scriptFile) {
    // TODO: Pete: Why do I crash here with a JNI error (accessing bad jni)
    string filePath = ArgConverter::jstringToString(scriptFile);
    auto context = this->GetContext();
    m_module.LoadWorker(context, filePath);
}

jobject Runtime::RunScript(JNIEnv* _env, jobject obj, jstring scriptFile) {
    JEnv env(_env);
    jobject res = nullptr;

    auto isolate = m_isolate;
    auto context = this->GetContext();

    auto filename = ArgConverter::jstringToString(scriptFile);
    auto src = ReadFileText(filename);

    auto source = ArgConverter::ConvertToV8String(isolate, src);

    TryCatch tc(isolate);

    Local<Script> script;
    ScriptOrigin origin(isolate, ArgConverter::ConvertToV8String(isolate, filename));
    auto maybeScript = Script::Compile(context, source, &origin).ToLocal(&script);

    if (tc.HasCaught()) {
        throw NativeScriptException(tc, "Script " + filename + " contains compilation errors!");
    }

    if (!script.IsEmpty()) {
        Local<Value> result;
        auto maybeResult = script->Run(context).ToLocal(&result);

        if (tc.HasCaught()) {
            throw NativeScriptException(tc, "Error running script " + filename);
        }
        if (!result.IsEmpty()) {
            res = ConvertJsValueToJavaObject(env, result, static_cast<int>(Type::Null));
        } else {
            DEBUG_WRITE(">>runScript maybeResult is empty");
        }
    } else {
        DEBUG_WRITE(">>runScript maybeScript is empty");
    }

    return res;
}

jobject Runtime::CallJSMethodNative(JNIEnv* _env, jobject obj, jint javaObjectID, jstring methodName, jint retType, jboolean isConstructor, jobjectArray packagedArgs) {
    SET_PROFILER_FRAME();

    auto isolate = m_isolate;

    JEnv env(_env);

    DEBUG_WRITE("CallJSMethodNative called javaObjectID=%d", javaObjectID);

    auto jsObject = m_objectManager->GetJsObjectByJavaObject(javaObjectID);
    if (jsObject.IsEmpty()) {
        stringstream ss;
        ss << "JavaScript object for Java ID " << javaObjectID << " not found." << endl;
        ss << "Attempting to call method " << ArgConverter::jstringToString(methodName) << endl;

        throw NativeScriptException(ss.str());
    }

    if (isConstructor) {
        DEBUG_WRITE("CallJSMethodNative: Updating linked instance with its real class");
        jclass instanceClass = env.GetObjectClass(obj);
        m_objectManager->SetJavaClass(jsObject, instanceClass);
    }

    DEBUG_WRITE("CallJSMethodNative called jsObject=%d", jsObject->GetIdentityHash());

    string method_name = ArgConverter::jstringToString(methodName);
    auto jsResult = CallbackHandlers::CallJSMethod(m_isolate, env, jsObject, method_name, packagedArgs);

    int classReturnType = retType;
    jobject javaObject = ConvertJsValueToJavaObject(env, jsResult, classReturnType);
    return javaObject;
}

void Runtime::CreateJSInstanceNative(JNIEnv* _env, jobject obj, jobject javaObject, jint javaObjectID, jstring className) {
    SET_PROFILER_FRAME();

    DEBUG_WRITE("createJSInstanceNative called");

    auto isolate = m_isolate;

    JEnv env(_env);

    string existingClassName = ArgConverter::jstringToString(className);
    string jniName = Util::ConvertFromCanonicalToJniName(existingClassName);
    Local<Object> jsInstance;
    Local<Object> implementationObject;

    auto proxyClassName = m_objectManager->GetClassName(javaObject);
    DEBUG_WRITE("createJSInstanceNative class %s", proxyClassName.c_str());
    jsInstance = MetadataNode::CreateExtendedJSWrapper(isolate, m_objectManager, proxyClassName);

    if (jsInstance.IsEmpty()) {
        throw NativeScriptException(string("Failed to create JavaScript extend wrapper for class '" + proxyClassName + "'"));
    }

    implementationObject = MetadataNode::GetImplementationObject(isolate, jsInstance);
    if (implementationObject.IsEmpty()) {
        string msg("createJSInstanceNative: implementationObject is empty");
        throw NativeScriptException(msg);
    }
    DEBUG_WRITE("createJSInstanceNative: implementationObject :%d", implementationObject->GetIdentityHash());

    jclass clazz = env.FindClass(jniName);
    m_objectManager->Link(jsInstance, javaObjectID, clazz);
}

jint Runtime::GenerateNewObjectId(JNIEnv* env, jobject obj) {
    int objectId = m_objectManager->GenerateNewObjectID();

    return objectId;
}

void Runtime::AdjustAmountOfExternalAllocatedMemory() {
    JEnv env;
    int64_t usedMemory = env.CallLongMethod(m_runtime, GET_USED_MEMORY_METHOD_ID);
    int64_t changeInBytes = usedMemory - m_lastUsedMemory;
    int64_t externalMemory = 0;

    if (changeInBytes != 0) {
        externalMemory = m_isolate->AdjustAmountOfExternalAllocatedMemory(changeInBytes);
    }

    DEBUG_WRITE("usedMemory=%" PRId64 " changeInBytes=%" PRId64 " externalMemory=%" PRId64, usedMemory, changeInBytes, externalMemory);

    m_lastUsedMemory = usedMemory;
}

bool Runtime::NotifyGC(JNIEnv* env, jobject obj) {
    bool success = __sync_bool_compare_and_swap(&m_runGC, false, true);
    return success;
}

bool Runtime::TryCallGC() {
    auto success = m_gcFunc != nullptr;
    if (success) {
        success = __sync_bool_compare_and_swap(&m_runGC, true, false);
        if (success) {
            auto ctx = m_isolate->GetCurrentContext();
            auto globalObject = ctx->Global();
            auto gcFunc = Local<Function>::New(m_isolate, *m_gcFunc);
            auto maybeResult = gcFunc.As<Function>()->Call(ctx, globalObject, 0, nullptr);
            DEBUG_WRITE("Induced GC runtimeId=%d", m_id);
        }
    }
    return success;
}

void Runtime::PassExceptionToJsNative(JNIEnv* env, jobject obj, jthrowable exception, jstring message, jstring fullStackTrace, jstring jsStackTrace, jboolean isDiscarded) {
    auto isolate = m_isolate;

    string errMsg = ArgConverter::jstringToString(message);

    auto errObj = Exception::Error(ArgConverter::ConvertToV8String(isolate, errMsg)).As<Object>();

    //create a new native exception js object
    jint javaObjectID = m_objectManager->GetOrCreateObjectId((jobject) exception);
    auto nativeExceptionObject = m_objectManager->GetJsObjectByJavaObject(javaObjectID);

    if (nativeExceptionObject.IsEmpty()) {
        string className = m_objectManager->GetClassName((jobject) exception);
        //create proxy object that wraps the java err
        nativeExceptionObject = m_objectManager->CreateJSWrapper(javaObjectID, className);
        if (nativeExceptionObject.IsEmpty()) {
            nativeExceptionObject = Object::New(isolate);
        }
    }

    //create a JS error object
    auto context = isolate->GetCurrentContext();
    errObj->Set(context, V8StringConstants::GetNativeException(isolate), nativeExceptionObject);
    errObj->Set(context, V8StringConstants::GetStackTrace(isolate), ArgConverter::jstringToV8String(isolate, fullStackTrace));
    if (jsStackTrace != NULL) {
        errObj->Set(context, V8StringConstants::GetStack(isolate), ArgConverter::jstringToV8String(isolate, jsStackTrace));
    }

    //pass err to JS
    NativeScriptException::CallJsFuncWithErr(errObj, isDiscarded);
}

void Runtime::PassUncaughtExceptionFromWorkerToMainHandler(Local<v8::String> message, Local<v8::String> stackTrace, Local<v8::String> filename, int lineno) {
    JEnv env;
    auto runtimeClass = env.GetObjectClass(m_runtime);

    auto mId = env.GetStaticMethodID(runtimeClass, "passUncaughtExceptionFromWorkerToMain",
                                     "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");

    auto jMsg = ArgConverter::ConvertToJavaString(message);
    auto jfileName = ArgConverter::ConvertToJavaString(filename);
    auto stckTrace = ArgConverter::ConvertToJavaString(stackTrace);

    JniLocalRef jMsgLocal(jMsg);
    JniLocalRef jfileNameLocal(jfileName);
    JniLocalRef stTrace(stckTrace);

    env.CallStaticVoidMethod(runtimeClass, mId, (jstring) jMsgLocal, (jstring) jfileNameLocal, (jstring) stTrace, lineno);
}

static void InitializeV8() {
    Runtime::platform = v8::platform::NewDefaultPlatform().release();
    V8::InitializePlatform(Runtime::platform);
    V8::Initialize();
}

Isolate* Runtime::PrepareV8Runtime(const string& filesPath, const string& nativeLibDir, const string& packageName, bool isDebuggable, const string& callingDir, const string& profilerOutputDir, const int maxLogcatObjectSize, const bool forceLog) {
    tns::instrumentation::Frame frame("Runtime.PrepareV8Runtime");

    Isolate::CreateParams create_params;

    create_params.array_buffer_allocator = &g_allocator;

    /*
     * Setup the V8Platform only once per process - once for the application lifetime
     * Don't execute again if main thread has already been initialized
     */
    if (!s_mainThreadInitialized) {
        InitializeV8();
    }

    tns::instrumentation::Frame isolateFrame;
    auto isolate = Isolate::New(create_params);
    isolateFrame.log("Isolate.New");

    s_isolate2RuntimesCache[isolate] = this;
    v8::Locker locker(isolate);
    Isolate::Scope isolate_scope(isolate);
    HandleScope handleScope(isolate);

    m_objectManager->SetInstanceIsolate(isolate);

    // Sets a structure with v8 String constants on the isolate object at slot 1
    auto consts = new V8StringConstants::PerIsolateV8Constants(isolate);
    isolate->SetData((uint32_t)Runtime::IsolateData::RUNTIME, this);
    isolate->SetData((uint32_t)Runtime::IsolateData::CONSTANTS, consts);

    V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(), Constants::V8_STARTUP_FLAGS.size());
    isolate->SetCaptureStackTraceForUncaughtExceptions(true, 100, StackTrace::kOverview);

    isolate->AddMessageListener(NativeScriptException::OnUncaughtError);

    __android_log_print(ANDROID_LOG_DEBUG, "TNS.Runtime", "V8 version %s", V8::GetVersion());

    auto globalFunctionTemplate = FunctionTemplate::New(isolate);
    globalFunctionTemplate->SetClassName(ArgConverter::ConvertToV8String(isolate, "NativeScriptGlobalObject"));
    tns::binding::CreateInternalBindingTemplates(isolate, globalFunctionTemplate);
    auto globalTemplate = ObjectTemplate::New(isolate, globalFunctionTemplate);

    const auto readOnlyFlags = static_cast<PropertyAttribute>(PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);

    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__log"), FunctionTemplate::New(isolate, CallbackHandlers::LogMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__dumpReferenceTables"), FunctionTemplate::New(isolate, CallbackHandlers::DumpReferenceTablesMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__drainMicrotaskQueue"), FunctionTemplate::New(isolate, CallbackHandlers::DrainMicrotaskCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__enableVerboseLogging"), FunctionTemplate::New(isolate, CallbackHandlers::EnableVerboseLoggingMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__disableVerboseLogging"), FunctionTemplate::New(isolate, CallbackHandlers::DisableVerboseLoggingMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__exit"), FunctionTemplate::New(isolate, CallbackHandlers::ExitMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__runtimeVersion"), ArgConverter::ConvertToV8String(isolate, NATIVE_SCRIPT_RUNTIME_VERSION), readOnlyFlags);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__time"), FunctionTemplate::New(isolate, CallbackHandlers::TimeCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__releaseNativeCounterpart"), FunctionTemplate::New(isolate, CallbackHandlers::ReleaseNativeCounterpartCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__markingMode"), Number::New(isolate, m_objectManager->GetMarkingMode()), readOnlyFlags);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__runOnMainThread"), FunctionTemplate::New(isolate, CallbackHandlers::RunOnMainThreadCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__postFrameCallback"), FunctionTemplate::New(isolate, CallbackHandlers::PostFrameCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__removeFrameCallback"), FunctionTemplate::New(isolate, CallbackHandlers::RemoveFrameCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "URL"), URLImpl::GetCtor(isolate));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "URLSearchParams"), URLSearchParamsImpl::GetCtor(isolate));

    /*
     * Attach `Worker` object constructor only to the main thread (isolate)'s global object
     * Workers should not be created from within other Workers, for now
     */
    if (!s_mainThreadInitialized) {
        m_isMainThread = true;
        pipe2(m_mainLooper_fd, O_NONBLOCK | O_CLOEXEC);
        m_mainLooper = ALooper_forThread();

       ALooper_acquire(m_mainLooper);

        // try using 2MB
        int ret = fcntl(m_mainLooper_fd[1], F_SETPIPE_SZ, 2 * (1024 * 1024));

        // try using 1MB
        if (ret != 0) {
            ret = fcntl(m_mainLooper_fd[1], F_SETPIPE_SZ, 1 * (1024 * 1024));
        }

        // try using 512KB
        if (ret != 0) {
            ret = fcntl(m_mainLooper_fd[1], F_SETPIPE_SZ, (512 * 1024));
        }

        ALooper_addFd(m_mainLooper, m_mainLooper_fd[0], ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, CallbackHandlers::RunOnMainThreadFdCallback, nullptr);

        Local<FunctionTemplate> workerFuncTemplate = FunctionTemplate::New(isolate, CallbackHandlers::NewThreadCallback);
        Local<ObjectTemplate> prototype = workerFuncTemplate->PrototypeTemplate();

        /*
         * Attach methods from the EventTarget interface (postMessage, terminate) to the Worker object prototype
         */
        auto postMessageFuncTemplate = FunctionTemplate::New(isolate, CallbackHandlers::WorkerObjectPostMessageCallback);
        auto terminateWorkerFuncTemplate = FunctionTemplate::New(isolate, CallbackHandlers::WorkerObjectTerminateCallback);

        prototype->Set(ArgConverter::ConvertToV8String(isolate, "postMessage"), postMessageFuncTemplate);
        prototype->Set(ArgConverter::ConvertToV8String(isolate, "terminate"), terminateWorkerFuncTemplate);

        globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "Worker"), workerFuncTemplate);
    }
    /*
     * Emulate a `WorkerGlobalScope`
     * Attach 'postMessage', 'close' to the global object
     */
    else {
        m_isMainThread = false;
        auto postMessageFuncTemplate = FunctionTemplate::New(isolate, CallbackHandlers::WorkerGlobalPostMessageCallback);
        globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "postMessage"), postMessageFuncTemplate);
        auto closeFuncTemplate = FunctionTemplate::New(isolate, CallbackHandlers::WorkerGlobalCloseCallback);
        globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "close"), closeFuncTemplate);
    }

#ifdef APPLICATION_IN_DEBUG
    /*
     * Attach __inspector object with function callbacks that report to the Chrome DevTools frontend
     */
    if (isDebuggable) {
        JsV8InspectorClient::attachInspectorCallbacks(isolate, globalTemplate);
    }
#endif

    SimpleProfiler::Init(isolate, globalTemplate);

    CallbackHandlers::CreateGlobalCastFunctions(isolate, globalTemplate);

    Local<Context> context = Context::New(isolate, nullptr, globalTemplate);

    auto blob_methods = R"js(
    const BLOB_STORE = new Map();
    URL.createObjectURL = function (object, options = null) {
        try {
            if (object instanceof Blob || object instanceof File) {
                const id = java.util.UUID.randomUUID().toString();
                const ret = `blob:nativescript/${id}`;
                BLOB_STORE.set(ret, {
                    blob: object,
                    type: object?.type,
                    ext: options?.ext,
                });
                return ret;
            }
        } catch (error) {
            return null;
        }
        return null;
    };
    URL.revokeObjectURL = function (url) {
        BLOB_STORE.delete(url);
    };
    const InternalAccessor = class {};
    InternalAccessor.getData = function (url) {
        return BLOB_STORE.get(url);
    };
    URL.InternalAccessor = InternalAccessor;
    Object.defineProperty(URL.prototype, 'searchParams', {
        get() {
            if (this._searchParams == null) {
                this._searchParams = new URLSearchParams(this.search);
                Object.defineProperty(this._searchParams, '_url', {
                    enumerable: false,
                    writable: false,
                    value: this,
                });
                this._searchParams._append = this._searchParams.append;
                this._searchParams.append = function (name, value) {
                    this._append(name, value);
                    this._url.search = this.toString();
                };
                this._searchParams._delete = this._searchParams.delete;
                this._searchParams.delete = function (name) {
                    this._delete(name);
                    this._url.search = this.toString();
                };
                this._searchParams._set = this._searchParams.set;
                this._searchParams.set = function (name, value) {
                    this._set(name, value);
                    this._url.search = this.toString();
                };
                this._searchParams._sort = this._searchParams.sort;
                this._searchParams.sort = function () {
                    this._sort();
                    this._url.search = this.toString();
                };
            }
            return this._searchParams;
        },
    });
    )js";


    auto global = context->Global();

    v8::Context::Scope contextScope{context};

    v8::Local<v8::Script> script;
    v8::Script::Compile(context, ArgConverter::ConvertToV8String(isolate, blob_methods)).ToLocal(&script);

    v8::Local<v8::Value> out;
    script->Run(context).ToLocal(&out);
    m_objectManager->Init(isolate);

    m_module.Init(isolate, callingDir);

    Local<Value> gcFunc;
    global->Get(context, ArgConverter::ConvertToV8String(isolate, "gc")).ToLocal(&gcFunc);
    if (!gcFunc.IsEmpty() && gcFunc->IsFunction()) {
        m_gcFunc = new Persistent<Function>(isolate, gcFunc.As<Function>());
    }

    global->DefineOwnProperty(context, ArgConverter::ConvertToV8String(isolate, "global"), global, readOnlyFlags);
    global->DefineOwnProperty(context, ArgConverter::ConvertToV8String(isolate, "__global"), global, readOnlyFlags);
    m_weakRef.Init(isolate, context);

    // Do not set 'self' accessor to main thread JavaScript
    if (s_mainThreadInitialized) {
        global->DefineOwnProperty(context, ArgConverter::ConvertToV8String(isolate, "self"), global, readOnlyFlags);
    }

#ifdef APPLICATION_IN_DEBUG
    v8::Local<v8::Object> console = Console::createConsole(context, JsV8InspectorClient::consoleLogCallback, maxLogcatObjectSize, forceLog);
#else
    v8::Local<v8::Object> console = Console::createConsole(context, nullptr, maxLogcatObjectSize, forceLog);
#endif

    /*
     * Attach 'console' object to the global object
     */
    global->DefineOwnProperty(context, ArgConverter::ConvertToV8String(isolate, "console"), console, readOnlyFlags);

    ArgConverter::Init(context);

    CallbackHandlers::Init(isolate);

    m_profiler.Init(isolate, global, packageName, profilerOutputDir);

    // Do not build metadata (which should be static for the process) for non-main threads
    if (!s_mainThreadInitialized) {
        MetadataNode::BuildMetadata(filesPath);
    }

    auto enableProfiler = !profilerOutputDir.empty();
    MetadataNode::EnableProfiler(enableProfiler);

    MetadataNode::CreateTopLevelNamespaces(isolate, global);

    ArrayHelper::Init(context);

    m_arrayBufferHelper.CreateConvertFunctions(context, global, m_objectManager);

    m_loopTimer->Init(context);

    this->m_context = new Persistent<Context>(isolate, context);

    s_mainThreadInitialized = true;

    return isolate;
}

jobject Runtime::ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType) {
    auto context = this->GetContext();
    JsArgToArrayConverter argConverter(context, value, false/*is implementation object*/, classReturnType);
    jobject jr = argConverter.GetConvertedArg();
    jobject javaResult = nullptr;
    if (jr != nullptr) {
        javaResult = env.NewLocalRef(jr);
    }

    return javaResult;
}

void Runtime::SetManualInstrumentationMode(jstring mode) {
    auto modeStr = ArgConverter::jstringToString(mode);
    if (modeStr == "timeline") {
        tns::instrumentation::Frame::enable();
    }
}

void Runtime::DestroyRuntime() {
    s_id2RuntimeCache.erase(m_id);
    s_isolate2RuntimesCache.erase(m_isolate);
    tns::disposeIsolate(m_isolate);
}

Local<Context> Runtime::GetContext() {
    return this->m_context->Get(this->m_isolate);
}

int Runtime::GetId() {
    return this->m_id;
}

int Runtime::GetWriter(){
    return m_mainLooper_fd[1];
}

int Runtime::GetReader(){
    return m_mainLooper_fd[0];
}

JavaVM* Runtime::s_jvm = nullptr;
jmethodID Runtime::GET_USED_MEMORY_METHOD_ID = nullptr;
robin_hood::unordered_map<int, Runtime*> Runtime::s_id2RuntimeCache;
robin_hood::unordered_map<Isolate*, Runtime*> Runtime::s_isolate2RuntimesCache;
bool Runtime::s_mainThreadInitialized = false;
v8::Platform* Runtime::platform = nullptr;
int Runtime::m_androidVersion = Runtime::GetAndroidVersion();
ALooper* Runtime::m_mainLooper = nullptr;
int Runtime::m_mainLooper_fd[2];
