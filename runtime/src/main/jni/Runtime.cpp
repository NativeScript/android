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
#include "V8NativeScriptExtension.h"
#include "Runtime.h"
#include "ArrayHelper.h"
#include "include/libplatform/libplatform.h"
#include "include/zipconf.h"
#include <sstream>
#include <dlfcn.h>
#include "NetworkDomainCallbackHandlers.h"
#include "sys/system_properties.h"
#include "JsV8InspectorClient.h"
#include "ManualInstrumentation.h"

using namespace v8;
using namespace std;
using namespace tns;

bool tns::LogEnabled = true;
SimpleAllocator g_allocator;

void Runtime::Init(JavaVM* vm, void* reserved) {
    __android_log_print(ANDROID_LOG_INFO, "TNS.Native", "NativeScript Runtime Version %s, commit %s", NATIVE_SCRIPT_RUNTIME_VERSION, NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);
    DEBUG_WRITE("JNI_ONLoad");

    if (Runtime::s_jvm == nullptr) {
        s_jvm = vm;

        JEnv::Init(s_jvm);
    }

    DEBUG_WRITE("JNI_ONLoad END");
}

Runtime::Runtime(JNIEnv* env, jobject runtime, int id)
    : m_env(env), m_id(id), m_isolate(nullptr), m_lastUsedMemory(0), m_gcFunc(nullptr), m_runGC(false) {
    m_runtime = m_env.NewGlobalRef(runtime);
    m_objectManager = new ObjectManager(m_runtime);
    s_id2RuntimeCache.insert(make_pair(id, this));

    if (GET_USED_MEMORY_METHOD_ID == nullptr) {
        auto RUNTIME_CLASS = m_env.FindClass("com/tns/Runtime");
        assert(RUNTIME_CLASS != nullptr);

        GET_USED_MEMORY_METHOD_ID = m_env.GetMethodID(RUNTIME_CLASS, "getUsedMemory", "()J");
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
    auto itFound = s_isolate2RuntimesCache.find(isolate);
    auto runtime = (itFound != s_isolate2RuntimesCache.end())
                   ? itFound->second
                   : nullptr;

    if (runtime == nullptr) {
        stringstream ss;
        ss << "Cannot find runtime for isolate: " << isolate;
        throw NativeScriptException(ss.str());
    }

    return runtime;
}

ObjectManager* Runtime::GetObjectManager(v8::Isolate* isolate) {
    auto itFound = s_isolate2RuntimesCache.find(isolate);
    auto runtime = (itFound != s_isolate2RuntimesCache.end())
                   ? itFound->second
                   : nullptr;

    if (runtime == nullptr) {
        stringstream ss;
        ss << "Cannot find runtime for isolate: " << isolate;
        throw NativeScriptException(ss.str());
    }

    return runtime->GetObjectManager();
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

void Runtime::Init(JNIEnv* _env, jobject obj, int runtimeId, jstring filesPath, jstring nativeLibDir, jboolean verboseLoggingEnabled, jboolean isDebuggable, jstring packageName, jobjectArray args, jstring callingDir) {
    JEnv env(_env);

    auto runtime = new Runtime(env, obj, runtimeId);

    auto enableLog = verboseLoggingEnabled == JNI_TRUE;

    runtime->Init(filesPath, nativeLibDir, enableLog, isDebuggable, packageName, args, callingDir);
}

void Runtime::Init(jstring filesPath, jstring nativeLibDir, bool verboseLoggingEnabled, bool isDebuggable, jstring packageName, jobjectArray args, jstring callingDir) {
    LogEnabled = verboseLoggingEnabled;

    auto filesRoot = ArgConverter::jstringToString(filesPath);
    auto nativeLibDirStr = ArgConverter::jstringToString(nativeLibDir);
    auto packageNameStr = ArgConverter::jstringToString(packageName);
    auto callingDirStr = ArgConverter::jstringToString(callingDir);

    Constants::APP_ROOT_FOLDER_PATH = filesRoot + "/app/";
    // read config options passed from Java
    JniLocalRef v8Flags(m_env.GetObjectArrayElement(args, 0));
    Constants::V8_STARTUP_FLAGS = ArgConverter::jstringToString(v8Flags);
    JniLocalRef cacheCode(m_env.GetObjectArrayElement(args, 1));
    Constants::V8_CACHE_COMPILED_CODE = (bool) cacheCode;
    JniLocalRef snapshotScript(m_env.GetObjectArrayElement(args, 2));
    Constants::V8_HEAP_SNAPSHOT_SCRIPT = ArgConverter::jstringToString(snapshotScript);
    JniLocalRef snapshotBlob(m_env.GetObjectArrayElement(args, 3));
    Constants::V8_HEAP_SNAPSHOT_BLOB = ArgConverter::jstringToString(snapshotBlob);
    JniLocalRef profilerOutputDir(m_env.GetObjectArrayElement(args, 4));

    DEBUG_WRITE("Initializing Telerik NativeScript");

    auto profilerOutputDirStr = ArgConverter::jstringToString(profilerOutputDir);

    NativeScriptException::Init(m_objectManager);
    m_isolate = PrepareV8Runtime(filesRoot, nativeLibDirStr, packageNameStr, isDebuggable, callingDirStr, profilerOutputDirStr);

    s_isolate2RuntimesCache.insert(make_pair(m_isolate, this));
}

void Runtime::RunModule(JNIEnv* _env, jobject obj, jstring scriptFile) {
    JEnv env(_env);

    string filePath = ArgConverter::jstringToString(scriptFile);
    m_module.Load(filePath);
}

void Runtime::RunWorker(jstring scriptFile) {
    // TODO: Pete: Why do I crash here with a JNI error (accessing bad jni)
    string filePath = ArgConverter::jstringToString(scriptFile);
    m_module.LoadWorker(filePath);
}

jobject Runtime::RunScript(JNIEnv* _env, jobject obj, jstring scriptFile) {
    JEnv env(_env);
    jobject res = nullptr;

    auto isolate = m_isolate;
    auto context = isolate->GetCurrentContext();

    auto filename = ArgConverter::jstringToString(scriptFile);
    auto src = File::ReadText(filename);
    auto source = ArgConverter::ConvertToV8String(isolate, src);

    TryCatch tc;

    Local<Script> script;
    ScriptOrigin origin(ArgConverter::ConvertToV8String(isolate, filename));
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
    int64_t usedMemory = m_env.CallLongMethod(m_runtime, GET_USED_MEMORY_METHOD_ID);
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

void Runtime::PassUncaughtExceptionToJsNative(JNIEnv* env, jobject obj, jthrowable exception, jstring stackTrace) {
    auto isolate = m_isolate;

    //create error message
    string errMsg = "The application crashed because of an uncaught exception. You can look at \"stackTrace\" or \"nativeException\" for more detailed information about the exception.";
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

    string stackTraceText = ArgConverter::jstringToString(stackTrace);
    errMsg += "\n" + stackTraceText;

    //create a JS error object
    errObj->Set(V8StringConstants::GetNativeException(isolate), nativeExceptionObject);
    errObj->Set(V8StringConstants::GetStackTrace(isolate), ArgConverter::jstringToV8String(isolate, stackTrace));

    //pass err to JS
    NativeScriptException::CallJsFuncWithErr(errObj);
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

void Runtime::ClearStartupData(JNIEnv* env, jobject obj) {
    delete m_heapSnapshotBlob;
    delete m_startupData;
}

static void InitializeV8() {
    Runtime::platform = v8::platform::CreateDefaultPlatform();
    V8::InitializePlatform(Runtime::platform);
    V8::Initialize();
}

Isolate* Runtime::PrepareV8Runtime(const string& filesPath, const string& nativeLibDir, const string& packageName, bool isDebuggable, const string& callingDir, const string& profilerOutputDir) {
    Isolate::CreateParams create_params;
    bool didInitializeV8 = false;

    create_params.array_buffer_allocator = &g_allocator;

    m_startupData = new StartupData();

    // Retrieve the device android Sdk version
    char sdkVersion[PROP_VALUE_MAX];
    __system_property_get("ro.build.version.sdk", sdkVersion);

    void* snapshotPtr;

    // If device isn't running on Sdk 17
    if (strcmp(sdkVersion, string("17").c_str()) != 0) {
        snapshotPtr = dlopen("libsnapshot.so", RTLD_LAZY | RTLD_LOCAL);
    } else {
        // If device is running on android Sdk 17
        // dlopen reads relative path to dynamic libraries or reads from folder different than the nativeLibsDirs on the android device
        string snapshotPath = nativeLibDir + "/libsnapshot.so";
        snapshotPtr = dlopen(snapshotPath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    }

    if (snapshotPtr == nullptr) {
        DEBUG_WRITE_FORCE("Failed to load snapshot: %s", dlerror());
    }

    if (snapshotPtr) {
        m_startupData->data = static_cast<const char*>(dlsym(snapshotPtr, "TNSSnapshot_blob"));
        m_startupData->raw_size = *static_cast<const unsigned int*>(dlsym(snapshotPtr, "TNSSnapshot_blob_len"));
        DEBUG_WRITE_FORCE("Snapshot library read %p (%dB).", m_startupData->data, m_startupData->raw_size);
    } else if (!Constants::V8_HEAP_SNAPSHOT_BLOB.empty() || !Constants::V8_HEAP_SNAPSHOT_SCRIPT.empty()) {
        DEBUG_WRITE_FORCE("Snapshot enabled.");

        string snapshotPath;
        bool saveSnapshot = true;
        // we have a precompiled snapshot blob provided - try to load it directly
        if (Constants::V8_HEAP_SNAPSHOT_BLOB.size() > 0) {
            snapshotPath = Constants::V8_HEAP_SNAPSHOT_BLOB;
            saveSnapshot = false;
        } else {
            snapshotPath = filesPath + "/internal/snapshot.blob";
        }

        if (File::Exists(snapshotPath)) {
            m_heapSnapshotBlob = new MemoryMappedFile(MemoryMappedFile::Open(snapshotPath.c_str()));
            m_startupData->data = static_cast<const char*>(m_heapSnapshotBlob->memory);
            m_startupData->raw_size = m_heapSnapshotBlob->size;

            DEBUG_WRITE_FORCE("Snapshot read %s (%dB).", snapshotPath.c_str(), m_heapSnapshotBlob->size);
        } else if (!saveSnapshot) {
            DEBUG_WRITE_FORCE("No snapshot file found at %s", snapshotPath.c_str());

        } else {
            // This should be executed before V8::Initialize, which calls it with false.
            NativeScriptExtension::CpuFeaturesProbe(true);
            InitializeV8();
            didInitializeV8 = true;

            string customScript;

            // check for custom script to include in the snapshot
            if (Constants::V8_HEAP_SNAPSHOT_SCRIPT.size() > 0 && File::Exists(Constants::V8_HEAP_SNAPSHOT_SCRIPT)) {
                customScript = File::ReadText(Constants::V8_HEAP_SNAPSHOT_SCRIPT);
            }

            DEBUG_WRITE_FORCE("Creating heap snapshot");
            *m_startupData = V8::CreateSnapshotDataBlob(customScript.c_str());

            if (m_startupData->raw_size == 0) {
                DEBUG_WRITE_FORCE("Failed to create heap snapshot.");
            } else {
                bool writeSuccess = File::WriteBinary(snapshotPath, m_startupData->data, m_startupData->raw_size);

                if (!writeSuccess) {
                    DEBUG_WRITE_FORCE("Failed to save created snapshot.");
                } else {
                    DEBUG_WRITE_FORCE("Saved snapshot of %s (%dB) in %s (%dB)",
                                      Constants::V8_HEAP_SNAPSHOT_SCRIPT.c_str(), customScript.size(),
                                      snapshotPath.c_str(), m_startupData->raw_size);
                }
            }
        }
    }

    create_params.snapshot_blob = m_startupData;

    /*
     * Setup the V8Platform only once per process - once for the application lifetime
     * Don't execute again if main thread has already been initialized
     */
    if (!didInitializeV8 && !s_mainThreadInitialized) {
        InitializeV8();
    }

    auto isolate = Isolate::New(create_params);
    Isolate::Scope isolate_scope(isolate);
    HandleScope handleScope(isolate);

    m_objectManager->SetInstanceIsolate(isolate);

    // Sets a structure with v8 String constants on the isolate object at slot 1
    V8StringConstants::PerIsolateV8Constants* consts = new V8StringConstants::PerIsolateV8Constants(isolate);
    isolate->SetData((uint32_t)Runtime::IsolateData::CONSTANTS, consts);

    V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(), Constants::V8_STARTUP_FLAGS.size());
    V8::SetCaptureStackTraceForUncaughtExceptions(true, 100, StackTrace::kOverview);

    isolate->AddMessageListener(NativeScriptException::OnUncaughtError);

    __android_log_print(ANDROID_LOG_DEBUG, "TNS.Native", "V8 version %s", V8::GetVersion());

    auto globalFunctionTemplate = FunctionTemplate::New(isolate);
    globalFunctionTemplate->SetClassName(ArgConverter::ConvertToV8String(isolate, "NativeScriptGlobalObject"));
    auto globalTemplate = ObjectTemplate::New(isolate, globalFunctionTemplate);

    const auto readOnlyFlags = static_cast<PropertyAttribute>(PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);

    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__log"), FunctionTemplate::New(isolate, CallbackHandlers::LogMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__dumpReferenceTables"), FunctionTemplate::New(isolate, CallbackHandlers::DumpReferenceTablesMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__consoleMessage"), FunctionTemplate::New(isolate, JsV8InspectorClient::sendToFrontEndCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__enableVerboseLogging"), FunctionTemplate::New(isolate, CallbackHandlers::EnableVerboseLoggingMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__disableVerboseLogging"), FunctionTemplate::New(isolate, CallbackHandlers::DisableVerboseLoggingMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__exit"), FunctionTemplate::New(isolate, CallbackHandlers::ExitMethodCallback));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__runtimeVersion"), ArgConverter::ConvertToV8String(isolate, NATIVE_SCRIPT_RUNTIME_VERSION), readOnlyFlags);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__time"), FunctionTemplate::New(isolate, CallbackHandlers::TimeCallback));

    /*
     * Attach `Worker` object constructor only to the main thread (isolate)'s global object
     * Workers should not be created from within other Workers, for now
     */
    if (!s_mainThreadInitialized) {
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
        auto postMessageFuncTemplate = FunctionTemplate::New(isolate, CallbackHandlers::WorkerGlobalPostMessageCallback);
        globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "postMessage"), postMessageFuncTemplate);
        auto closeFuncTemplate = FunctionTemplate::New(isolate, CallbackHandlers::WorkerGlobalCloseCallback);
        globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "close"), closeFuncTemplate);
    }

    /*
     * Attach __inspector object with function callbacks that report to the Chrome DevTools frontend
     */
    if (isDebuggable) {
        JsV8InspectorClient::attachInspectorCallbacks(isolate, globalTemplate);
    }

    m_weakRef.Init(isolate, globalTemplate, m_objectManager);

    SimpleProfiler::Init(isolate, globalTemplate);

    CallbackHandlers::CreateGlobalCastFunctions(isolate, globalTemplate);

    Local<Context> context = Context::New(isolate, nullptr, globalTemplate);
    context->Enter();

    m_objectManager->Init(isolate);

    m_module.Init(isolate, callingDir);

    auto global = context->Global();

    auto gcFunc = global->Get(ConvertToV8String("gc"));
    if (!gcFunc.IsEmpty() && gcFunc->IsFunction()) {
        m_gcFunc = new Persistent<Function>(isolate, gcFunc.As<Function>());
    }

    global->ForceSet(ArgConverter::ConvertToV8String(isolate, "global"), global, readOnlyFlags);
    global->ForceSet(ArgConverter::ConvertToV8String(isolate, "__global"), global, readOnlyFlags);

    // Do not set 'self' accessor to main thread JavaScript
    if (s_mainThreadInitialized) {
        global->ForceSet(ArgConverter::ConvertToV8String(isolate, "self"), global, readOnlyFlags);
    }

    ArgConverter::Init(isolate);

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

    m_arrayBufferHelper.CreateConvertFunctions(isolate, global, m_objectManager);

    s_mainThreadInitialized = true;

    return isolate;
}

jobject Runtime::ConvertJsValueToJavaObject(JEnv& env, const Local<Value>& value, int classReturnType) {
    JsArgToArrayConverter argConverter(m_isolate, value, false/*is implementation object*/, classReturnType);
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
}

JavaVM* Runtime::s_jvm = nullptr;
jmethodID Runtime::GET_USED_MEMORY_METHOD_ID = nullptr;
map<int, Runtime*> Runtime::s_id2RuntimeCache;
map<Isolate*, Runtime*> Runtime::s_isolate2RuntimesCache;
bool Runtime::s_mainThreadInitialized = false;
v8::Platform* Runtime::platform = nullptr;
