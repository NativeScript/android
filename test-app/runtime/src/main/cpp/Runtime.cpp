#include "Runtime.h"

#include <console/Console.h>
#include <dlfcn.h>
#include <unistd.h>

#include <chrono>
#include <mutex>
#include <sstream>
#include <thread>

#include "ArgConverter.h"
#include "ArrayHelper.h"
#include "BuiltinLoader.h"
#include "CallbackHandlers.h"
#include "Constants.h"
#include "CrashBreadcrumbs.h"
#include "ErrorEvents.h"
#include "Events.h"
#include "File.h"
#include "FrameCallbacks.h"
#include "HttpLoader.h"
#include "Interop.h"
#include "IsolateTracked.h"
#include "JType.h"
#include "JsArgConverter.h"
#include "JsArgToArrayConverter.h"
#include "ManualInstrumentation.h"
#include "MetadataNode.h"
#include "ModuleBinding.h"
#include "ModuleInternal.h"
#include "ModuleInternalCallbacks.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "NativeScriptPlatform.h"
#include "RuntimeState.h"
#include "napi/NapiEnv.h"
#include "Performance.h"
#include "SimpleAllocator.h"
#include "SimpleProfiler.h"
#include "StructuredClone.h"
#include "TraceLog.h"
#include "URLImpl.h"
#include "URLPatternImpl.h"
#include "URLSearchParamsImpl.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "Version.h"
#include "WeakRef.h"
#include "include/libplatform/libplatform.h"
#include "include/zipconf.h"
#include "libplatform/libplatform.h"
#include "sys/system_properties.h"

#ifdef APPLICATION_IN_DEBUG
// #include "NetworkDomainCallbackHandlers.h"
#include "JsV8InspectorClient.h"
#endif

using namespace v8;
using namespace std;
using namespace tns;

bool tns::LogEnabled = true;
SimpleAllocator g_allocator;

void LogAndAbortUncaught() {
  try {
    throw;  // rethrow the current unknown
  } catch (const tns::NativeScriptException& e) {
    // We only have message/stack; no safe ReThrowToJava here.
    __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                        "Uncaught NativeScriptException: %s", e.what());
  } catch (const std::exception& e) {
    __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                        "Uncaught std::exception: %s", e.what());
  } catch (...) {
    __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                        "Uncaught unknown native exception");
  }

  // Preserve default abort behavior so crashes are visible to tooling
  std::_Exit(EXIT_FAILURE);
}

void Runtime::Init(JavaVM* vm, void* reserved) {
  // Before anything worth tracing runs, so NS_DEBUG covers boot itself.
  tns::InitializeLogCategoriesFromEnvironment();

  __android_log_print(ANDROID_LOG_INFO, "TNS.Runtime",
                      "NativeScript Runtime Version %s, commit %s",
                      NATIVE_SCRIPT_RUNTIME_VERSION,
                      NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);

  if (Runtime::s_jvm == nullptr) {
    s_jvm = vm;

    JEnv::Init(s_jvm);
  }

  CrashBreadcrumbs::Install();

  // Set terminate handler for uncaught exceptions
  std::set_terminate(LogAndAbortUncaught);
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
    : m_id(id),
      m_isolate(nullptr),
      m_lastUsedMemory(0),
      m_gcFunc(nullptr),
      m_runGC(false) {
  m_runtime = env->NewGlobalRef(runtime);
  m_objectManager = new ObjectManager(m_runtime);
  m_state = std::make_unique<RuntimeState>();
  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    s_id2RuntimeCache.emplace(id, this);
  }

  if (GET_USED_MEMORY_METHOD_ID == nullptr) {
    auto RUNTIME_CLASS = env->FindClass("com/tns/Runtime");
    NS_CHECK(RUNTIME_CLASS != nullptr);

    GET_USED_MEMORY_METHOD_ID =
        env->GetMethodID(RUNTIME_CLASS, "getUsedMemory", "()J");
    NS_CHECK(GET_USED_MEMORY_METHOD_ID != nullptr);
  }
}

Runtime* Runtime::GetRuntime(int runtimeId) {
  Runtime* runtime = nullptr;
  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    auto itFound = s_id2RuntimeCache.find(runtimeId);
    runtime = (itFound != s_id2RuntimeCache.end()) ? itFound->second : nullptr;
  }

  if (runtime == nullptr) {
    stringstream ss;
    ss << "Cannot find runtime for id:" << runtimeId;
    throw NativeScriptException(ss.str());
  }

  return runtime;
}

Runtime* Runtime::GetRuntime(v8::Isolate* isolate) {
  /*
   * Hot path, called from V8 callbacks on the isolate's own thread. The slot
   * is written once in PrepareV8Runtime before the isolate runs any JS, so
   * reading it requires no lock.
   */
  auto runtime = TryGetRuntime(isolate);
  if (runtime != nullptr) {
    return runtime;
  }

  // covers the window during isolate setup before SetData has run
  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    auto it = s_isolate2RuntimesCache.find(isolate);
    runtime = (it != s_isolate2RuntimesCache.end()) ? it->second : nullptr;
  }

  if (runtime == nullptr) {
    stringstream ss;
    ss << "Cannot find runtime for isolate: " << isolate;
    throw NativeScriptException(ss.str());
  }

  return runtime;
}

Runtime* Runtime::GetRuntimeFromIsolateData(v8::Isolate* isolate) {
  auto runtime = TryGetRuntime(isolate);

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

Isolate* Runtime::GetIsolate() const { return m_isolate; }

jobject Runtime::GetJavaRuntime() const { return m_runtime; }

ObjectManager* Runtime::GetObjectManager() const { return m_objectManager; }

void Runtime::Init(JNIEnv* _env, jobject obj, int runtimeId, jstring filesPath,
                   jstring nativeLibDir, jboolean verboseLoggingEnabled,
                   jboolean isDebuggable, jstring packageName,
                   jobjectArray args, jstring callingDir,
                   int maxLogcatObjectSize, bool forceLog) {
  JEnv env(_env);

  auto runtime = new Runtime(env, obj, runtimeId);

  auto enableLog = verboseLoggingEnabled == JNI_TRUE;

  try {
    runtime->Init(env, filesPath, nativeLibDir, enableLog, isDebuggable,
                  packageName, args, callingDir, maxLogcatObjectSize, forceLog);
  } catch (...) {
    // The Java side rolls back its own state and rethrows; without this the
    // native half of a failed bootstrap would stay allocated and registered.
    delete runtime;
    throw;
  }
}

void Runtime::Init(JNIEnv* env, jstring filesPath, jstring nativeLibDir,
                   bool verboseLoggingEnabled, bool isDebuggable,
                   jstring packageName, jobjectArray args, jstring callingDir,
                   int maxLogcatObjectSize, bool forceLog) {
  LogEnabled = verboseLoggingEnabled;

  auto filesRoot = ArgConverter::jstringToString(filesPath);
  auto nativeLibDirStr = ArgConverter::jstringToString(nativeLibDir);
  auto packageNameStr = ArgConverter::jstringToString(packageName);
  auto callingDirStr = ArgConverter::jstringToString(callingDir);

  // Runs on this runtime's own thread, for the main runtime and for workers
  // alike, so the calling thread is the one the breadcrumb should name.
  CrashBreadcrumbs::OpenStore(filesRoot);
  CrashBreadcrumbs::RegisterRuntime(m_id);

  Constants::APP_ROOT_FOLDER_PATH = filesRoot + "/app/";
  // read config options passed from Java
  // Indices correspond to positions in the com.tns.AppConfig.KnownKeys enum
  JniLocalRef v8Flags(env->GetObjectArrayElement(args, 0));
  Constants::V8_STARTUP_FLAGS = ArgConverter::jstringToString(v8Flags);
  JniLocalRef cacheCode(env->GetObjectArrayElement(args, 1));
  Constants::V8_CACHE_COMPILED_CODE = (bool)cacheCode;
  JniLocalRef profilerOutputDir(env->GetObjectArrayElement(args, 2));

  {
    JEnv jEnv(env);
    JniLocalRef discardUncaught(env->GetObjectArrayElement(
        args, (jsize)11 /* KnownKeys.DiscardUncaughtJsExceptions */));
    if (!discardUncaught.IsNull()) {
      m_discardUncaughtJsExceptions =
          JType::BooleanValue(jEnv, discardUncaught) == JNI_TRUE;
    }

    JniLocalRef uncaughtErrorPolicy(env->GetObjectArrayElement(
        args, (jsize)14 /* KnownKeys.UncaughtErrorPolicy */));
    if (!uncaughtErrorPolicy.IsNull()) {
      auto policy = ArgConverter::jstringToString(uncaughtErrorPolicy);
      if (policy == "throw") {
        m_uncaughtErrorPolicy = UncaughtErrorPolicy::Throw;
      } else {
        // AppConfig validates and warns about unknown values; anything that
        // is not "throw" behaves as the default.
        m_uncaughtErrorPolicy = UncaughtErrorPolicy::Report;
      }
    }
  }

  DEBUG_WRITE("Initializing Telerik NativeScript");

  auto profilerOutputDirStr = ArgConverter::jstringToString(profilerOutputDir);

  NativeScriptException::Init();
  try {
    m_isolate = PrepareV8Runtime(
        filesRoot, nativeLibDirStr, packageNameStr, isDebuggable, callingDirStr,
        profilerOutputDirStr, maxLogcatObjectSize, forceLog);
  } catch (NativeScriptException& e) {
    // The isolate is about to go away and this exception outlives it, so it
    // must not carry a handle into it any further; the message and stack it
    // already extracted are what the Java side reports.
    e.ReleaseJsHandle();
    UnwindFailedInit();
    throw;
  } catch (...) {
    UnwindFailedInit();
    throw;
  }
}

Runtime::~Runtime() {
  /*
   * Idempotent backstop for the erase DestroyRuntime does: a bootstrap that
   * failed before PrepareV8Runtime ran never reached it. The isolate entry is
   * matched on value so it cannot evict a new runtime that reused the pointer.
   */
  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    s_id2RuntimeCache.erase(m_id);
    auto it = s_isolate2RuntimesCache.find(m_isolate);
    if (it != s_isolate2RuntimesCache.end() && it->second == this) {
      s_isolate2RuntimesCache.erase(it);
    }
  }

  delete this->m_objectManager;
  // idempotent backstop for the matched erase WorkerWrapper does right after
  // Isolate::Dispose (the match keeps this from evicting a new isolate that
  // reused the pointer); instance/isolate may be null when construction
  // failed before PrepareV8Runtime
  auto* platformInstance = NativeScriptPlatform::Instance();
  if (platformInstance != nullptr && m_isolate != nullptr && m_eventLoop != nullptr) {
    platformInstance->IsolateDisposed(m_isolate, m_eventLoop);
  }

  // Last: ObjectManager calls Java through this same object above, so the ref
  // has to outlive it. Without this the com.tns.Runtime instance -- and every
  // Java object the runtime ever strongly registered through it -- stays
  // reachable for the life of the process.
  if (m_runtime != nullptr) {
    JEnv env;
    env.DeleteGlobalRef(m_runtime);
    m_runtime = nullptr;
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

static void PumpPendingHttpModuleGraph(v8::Isolate* isolate) {
  if (!tns::HasPendingAsyncModuleGraphWork()) {
    return;
  }
  const auto start = std::chrono::steady_clock::now();
  while (tns::HasPendingAsyncModuleGraphWork()) {
    isolate->PerformMicrotaskCheckpoint();
    ALooper_pollOnce(10, nullptr, nullptr, nullptr);
    isolate->PerformMicrotaskCheckpoint();
    if (std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count() > 60.0) {
      DEBUG_WRITE("PumpPendingHttpModuleGraph: deadline expired with pending async module work");
      break;
    }
  }
}

void Runtime::RunModule(JNIEnv* _env, jobject obj, jstring scriptFile) {
  JEnv env(_env);

  string filePath = ArgConverter::jstringToString(scriptFile);
  auto context = this->GetContext();
  m_module.Load(context, filePath);
  PumpPendingHttpModuleGraph(m_isolate);
}

void Runtime::RunModule(const char* moduleName) {
  auto context = this->GetContext();
  m_module.Load(context, moduleName);
  PumpPendingHttpModuleGraph(m_isolate);
}

void Runtime::RunWorker(const std::string& filePath) {
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
  ScriptOrigin origin(ArgConverter::ConvertToV8String(isolate, filename));
  auto maybeScript = Script::Compile(context, source, &origin).ToLocal(&script);

  if (tc.HasCaught()) {
    throw NativeScriptException(
        tc, "Script " + filename + " contains compilation errors!");
  }

  if (!script.IsEmpty()) {
    Local<Value> result;
    auto maybeResult = script->Run(context).ToLocal(&result);

    if (tc.HasCaught()) {
      throw NativeScriptException(tc, "Error running script " + filename);
    }
    if (!result.IsEmpty()) {
      res =
          ConvertJsValueToJavaObject(env, result, static_cast<int>(Type::Null));
    } else {
      DEBUG_WRITE(">>runScript maybeResult is empty");
    }
  } else {
    DEBUG_WRITE(">>runScript maybeScript is empty");
  }

  return res;
}

jobject Runtime::CallJSMethodNative(JNIEnv* _env, jobject obj,
                                    jint javaObjectID, jstring methodName,
                                    jint retType, jboolean isConstructor,
                                    jobjectArray packagedArgs) {
  SET_PROFILER_FRAME();

  auto isolate = m_isolate;

  JEnv env(_env);

  DEBUG_WRITE("CallJSMethodNative called javaObjectID=%d", javaObjectID);

  auto jsObject = m_objectManager->GetJsObjectByJavaObject(javaObjectID);
  if (jsObject.IsEmpty()) {
    stringstream ss;
    ss << "JavaScript object for Java ID " << javaObjectID << " not found."
       << endl;
    ss << "Attempting to call method "
       << ArgConverter::jstringToString(methodName) << endl;

    throw NativeScriptException(ss.str());
  }

  if (isConstructor) {
    DEBUG_WRITE(
        "CallJSMethodNative: Updating linked instance with its real class");
    jclass instanceClass = env.GetObjectClass(obj);
    m_objectManager->SetJavaClass(jsObject, instanceClass);
  }

  DEBUG_WRITE("CallJSMethodNative called jsObject=%d",
              jsObject->GetIdentityHash());

  string method_name = ArgConverter::jstringToString(methodName);
  auto jsResult = CallbackHandlers::CallJSMethod(m_isolate, env, jsObject,
                                                 method_name, packagedArgs);

  int classReturnType = retType;
  jobject javaObject =
      ConvertJsValueToJavaObject(env, jsResult, classReturnType);
  return javaObject;
}

void Runtime::CreateJSInstanceNative(JNIEnv* _env, jobject obj,
                                     jobject javaObject, jint javaObjectID,
                                     jstring className) {
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
  jsInstance = MetadataNode::CreateExtendedJSWrapper(isolate, m_objectManager,
                                                     proxyClassName);

  if (jsInstance.IsEmpty()) {
    throw NativeScriptException(
        string("Failed to create JavaScript extend wrapper for class '" +
               proxyClassName + "'"));
  }

  implementationObject =
      MetadataNode::GetImplementationObject(isolate, jsInstance);
  if (implementationObject.IsEmpty()) {
    string msg("createJSInstanceNative: implementationObject is empty");
    throw NativeScriptException(msg);
  }
  DEBUG_WRITE("createJSInstanceNative: implementationObject :%d",
              implementationObject->GetIdentityHash());

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
    externalMemory =
        m_isolate->AdjustAmountOfExternalAllocatedMemory(changeInBytes);
  }

  DEBUG_WRITE("usedMemory=%" PRId64 " changeInBytes=%" PRId64
              " externalMemory=%" PRId64,
              usedMemory, changeInBytes, externalMemory);

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
      auto maybeResult =
          gcFunc.As<Function>()->Call(ctx, globalObject, 0, nullptr);
      DEBUG_WRITE("Induced GC runtimeId=%d", m_id);
    }
  }
  return success;
}

jboolean Runtime::PassExceptionToJsNative(JNIEnv* env, jobject obj,
                                          jthrowable exception, jstring message,
                                          jstring fullStackTrace,
                                          jstring jsStackTrace,
                                          jboolean isDiscarded) {
  auto isolate = m_isolate;

  string errMsg = ArgConverter::jstringToString(message);

  auto errObj =
      Exception::Error(ArgConverter::ConvertToV8String(isolate, errMsg))
          .As<Object>();

  // create a new native exception js object
  jint javaObjectID = m_objectManager->GetOrCreateObjectId((jobject)exception);
  auto nativeExceptionObject =
      m_objectManager->GetJsObjectByJavaObject(javaObjectID);

  if (nativeExceptionObject.IsEmpty()) {
    string className = m_objectManager->GetClassName((jobject)exception);
    // create proxy object that wraps the java err
    nativeExceptionObject =
        m_objectManager->CreateJSWrapper(javaObjectID, className);
    if (nativeExceptionObject.IsEmpty()) {
      nativeExceptionObject = Object::New(isolate);
    }
  }

  // create a JS error object
  auto context = isolate->GetCurrentContext();
  errObj->Set(context, V8StringConstants::GetNativeException(isolate),
              nativeExceptionObject);
  errObj->Set(context, V8StringConstants::GetStackTrace(isolate),
              ArgConverter::jstringToV8String(isolate, fullStackTrace));
  if (jsStackTrace != NULL) {
    errObj->Set(context, V8StringConstants::GetStack(isolate),
                ArgConverter::jstringToV8String(isolate, jsStackTrace));
  }

  // Give WHATWG listeners a chance first; preventDefault() fully handles the
  // report - no __on*Error shim, and the Java caller is told the exception
  // was handled. The discard path (a containment variant - the app is alive)
  // dispatches the `error` event; the uncaught path is the post-mortem
  // native-layer death notification and dispatches `nativeuncaughterror`,
  // preserving the invariant that `error` only fires while the failure is
  // still containable.
  string stackTraceStr = ArgConverter::jstringToString(fullStackTrace);
  bool prevented;
  if (isDiscarded == JNI_TRUE) {
    prevented = ErrorEvents::DispatchError(isolate, errObj, errMsg, stackTraceStr);
  } else {
    prevented = ErrorEvents::DispatchNativeUncaughtError(isolate, errObj, errMsg,
                                                         stackTraceStr);
  }
  if (prevented) {
    return JNI_TRUE;
  }

  // pass err to JS
  NativeScriptException::CallJsFuncWithErr(errObj, isDiscarded);
  return JNI_FALSE;
}

static void InitializeV8() {
  // V8::Initialize() freezes the flag list, and changing a flag afterwards
  // aborts the process, so the app's flags have to be applied here rather than
  // per isolate. Runtime::Init has already read them out of the Java config.
  V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(),
                         Constants::V8_STARTUP_FLAGS.size());
  // wrap the default platform so foreground tasks ride each runtime thread's
  // Java Looper instead of sitting in never-pumped libplatform queues
  Runtime::platform =
      new NativeScriptPlatform(v8::platform::NewDefaultPlatform());
  V8::InitializePlatform(Runtime::platform);
  V8::Initialize();
}

void Runtime::ElectMainRuntime() {
  std::unique_lock<std::mutex> lock(s_mainInitMutex);

  if (!s_mainRuntimeElected) {
    s_mainRuntimeElected = true;
    s_mainRuntimeFailed = false;
    m_isMainThread = true;
    // Once per process: V8::Initialize freezes the flag list, and setting a
    // flag afterwards aborts.
    InitializeV8();
    return;
  }

  m_isMainThread = false;

  s_mainInitReady.wait(lock, [] {
    return s_mainThreadInitialized.load(std::memory_order_acquire) ||
           s_mainRuntimeFailed;
  });

  if (!s_mainThreadInitialized.load(std::memory_order_acquire)) {
    throw NativeScriptException(
        "Cannot initialize a runtime: the main runtime failed to initialize");
  }
}

void Runtime::SignalMainRuntimeReady(bool failed) {
  {
    std::lock_guard<std::mutex> lock(s_mainInitMutex);
    if (failed) {
      // Hand the election back so a later bootstrap can retry.
      s_mainRuntimeElected = false;
      s_mainRuntimeFailed = true;
    } else {
      s_mainThreadInitialized.store(true, std::memory_order_release);
    }
  }
  s_mainInitReady.notify_all();
}

void Runtime::UnwindFailedInit() {
  /*
   * Reuses the two teardown windows rather than adding a third cleanup path.
   * Every step DestroyRuntime takes is null-tolerant, which is what makes it
   * usable on a runtime that never finished building; the Locker is reentrant,
   * so it does not matter whether the failure unwound past the one
   * PrepareV8Runtime holds.
   */
  if (m_isolate != nullptr) {
    {
      v8::Locker locker(m_isolate);
      Isolate::Scope isolateScope(m_isolate);
      DestroyRuntime();
    }
    m_isolate->Dispose();
    m_isolate = nullptr;
  }

  if (m_isMainThread) {
    SignalMainRuntimeReady(true /* failed */);
  }
}

Isolate* Runtime::PrepareV8Runtime(const string& filesPath,
                                   const string& nativeLibDir,
                                   const string& packageName, bool isDebuggable,
                                   const string& callingDir,
                                   const string& profilerOutputDir,
                                   const int maxLogcatObjectSize,
                                   const bool forceLog) {
  tns::instrumentation::Frame frame("Runtime.PrepareV8Runtime");

  Isolate::CreateParams create_params;

  create_params.array_buffer_allocator = &g_allocator;

  // Also initializes V8 for the process if this runtime wins the election, and
  // otherwise waits for the runtime that did.
  ElectMainRuntime();

  tns::instrumentation::Frame isolateFrame;
  auto isolate = Isolate::New(create_params);
  // MonotonicallyIncreasingTime returns seconds as a double.
  m_timeOriginMonotonic = platform->MonotonicallyIncreasingTime();
  m_timeOriginRealtimeMs = platform->CurrentClockTimeMillis();
  isolateFrame.log("Isolate.New");

  // From here on the isolate is reachable through the runtime caches and owns
  // allocations of its own, so anything that throws below has to be unwound.
  m_isolate = isolate;
  m_objectManager->SetInstanceIsolate(isolate);

  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    s_isolate2RuntimesCache[isolate] = this;
  }
  // attach the runtime's event loop to this thread's looper; v8 foreground
  // tasks buffered during Isolate::New start flowing from here on. Refresh
  // rather than Get: a reused isolate pointer may still map to the previous
  // tenant's stopped loop
  m_eventLoop = NativeScriptPlatform::Instance()->RefreshEventLoop(isolate);
  m_eventLoop->BindToCurrentThread();
  v8::Locker locker(isolate);
  Isolate::Scope isolate_scope(isolate);
  HandleScope handleScope(isolate);

  // Sets a structure with v8 String constants on the isolate object at slot 1
  auto consts = new V8StringConstants::PerIsolateV8Constants(isolate);
  isolate->SetData((uint32_t)Runtime::IsolateData::RUNTIME, this);
  isolate->SetData((uint32_t)Runtime::IsolateData::CONSTANTS, consts);

  isolate->SetCaptureStackTraceForUncaughtExceptions(true, 100,
                                                     StackTrace::kOverview);

  // Set up import.meta callback
  isolate->SetHostInitializeImportMetaObjectCallback(
      InitializeImportMetaObject);

  // Enable dynamic import() support
  isolate->SetHostImportModuleDynamicallyCallback(
      ImportModuleDynamicallyCallback);

  isolate->AddMessageListener(NativeScriptException::OnUncaughtError);
  isolate->SetPromiseRejectCallback(NativeScriptException::OnPromiseRejected);

  __android_log_print(ANDROID_LOG_DEBUG, "TNS.Runtime", "V8 version %s",
                      V8::GetVersion());

  auto globalFunctionTemplate = FunctionTemplate::New(isolate);
  globalFunctionTemplate->SetClassName(
      ArgConverter::ConvertToV8String(isolate, "NativeScriptGlobalObject"));
  tns::binding::CreateInternalBindingTemplates(isolate, globalFunctionTemplate);
  auto globalTemplate = ObjectTemplate::New(isolate, globalFunctionTemplate);

  const auto readOnlyFlags = static_cast<PropertyAttribute>(
      PropertyAttribute::DontDelete | PropertyAttribute::ReadOnly);

  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__log"),
      FunctionTemplate::New(isolate, CallbackHandlers::LogMethodCallback));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__dumpReferenceTables"),
      FunctionTemplate::New(
          isolate, CallbackHandlers::DumpReferenceTablesMethodCallback));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__drainMicrotaskQueue"),
      FunctionTemplate::New(isolate, CallbackHandlers::DrainMicrotaskCallback));
  // TODO: remove the __ns__ prefix once the event loop's ordered lane backs
  // public macrotask APIs (performance observers etc.)
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__ns__queueMacrotask"),
      FunctionTemplate::New(isolate, CallbackHandlers::QueueMacrotaskCallback));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__enableVerboseLogging"),
      FunctionTemplate::New(
          isolate, CallbackHandlers::EnableVerboseLoggingMethodCallback));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__disableVerboseLogging"),
      FunctionTemplate::New(
          isolate, CallbackHandlers::DisableVerboseLoggingMethodCallback));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__exit"),
      FunctionTemplate::New(isolate, CallbackHandlers::ExitMethodCallback));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__runtimeVersion"),
      ArgConverter::ConvertToV8String(isolate, NATIVE_SCRIPT_RUNTIME_VERSION),
      readOnlyFlags);
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__time"),
      FunctionTemplate::New(isolate, CallbackHandlers::TimeCallback));

  // queueMicrotask(callback) per spec:
  // https://developer.mozilla.org/en-US/docs/Web/API/Window/queueMicrotask
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "queueMicrotask"),
      FunctionTemplate::New(
          isolate, [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            auto* isolate = info.GetIsolate();
            if (info.Length() < 1 || !info[0]->IsFunction()) {
              isolate->ThrowException(
                  v8::Exception::TypeError(ArgConverter::ConvertToV8String(
                      isolate, "queueMicrotask: callback must be a function")));
              return;
            }
            v8::Local<v8::Function> cb = info[0].As<v8::Function>();
            isolate->EnqueueMicrotask(cb);
          }));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__releaseNativeCounterpart"),
      FunctionTemplate::New(
          isolate, CallbackHandlers::ReleaseNativeCounterpartCallback));
  globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__markingMode"),
                      Number::New(isolate, m_objectManager->GetMarkingMode()),
                      readOnlyFlags);
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__runOnMainThread"),
      FunctionTemplate::New(isolate,
                            CallbackHandlers::RunOnMainThreadCallback));
  FrameCallbacks::Init(isolate, globalTemplate);
  globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "URL"),
                      URLImpl::GetCtor(isolate));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "URLSearchParams"),
      URLSearchParamsImpl::GetCtor(isolate));
  globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "URLPattern"),
                      URLPatternImpl::GetCtor(isolate));

  if (m_isMainThread) {
    // __runOnMainThread closures from any runtime's thread land on this loop
    s_mainEventLoop = m_eventLoop;
  }
  /*
   * Emulate a `WorkerGlobalScope`
   * Attach 'postMessage', 'close' to the global object
   */
  else {
    auto postMessageFuncTemplate = FunctionTemplate::New(
        isolate, CallbackHandlers::WorkerGlobalPostMessageCallback);
    globalTemplate->Set(
        ArgConverter::ConvertToV8String(isolate, "__ns__worker"),
        Boolean::New(isolate, true));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "postMessage"),
                        postMessageFuncTemplate);
    auto closeFuncTemplate = FunctionTemplate::New(
        isolate, CallbackHandlers::WorkerGlobalCloseCallback);
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "close"),
                        closeFuncTemplate);
  }

  /*
   * Attach the `Worker` constructor to every isolate's global object - workers
   * may be created from the main thread or from within other workers
   * (mirroring the iOS runtime).
   */
  {
    Local<FunctionTemplate> workerFuncTemplate =
        FunctionTemplate::New(isolate, CallbackHandlers::NewThreadCallback);
    Local<ObjectTemplate> prototype = workerFuncTemplate->PrototypeTemplate();

    /*
     * Attach methods from the EventTarget interface (postMessage, terminate) to
     * the Worker object prototype
     */
    auto postMessageFuncTemplate = FunctionTemplate::New(
        isolate, CallbackHandlers::WorkerObjectPostMessageCallback);
    auto terminateWorkerFuncTemplate = FunctionTemplate::New(
        isolate, CallbackHandlers::WorkerObjectTerminateCallback);

    prototype->Set(ArgConverter::ConvertToV8String(isolate, "postMessage"),
                   postMessageFuncTemplate);
    prototype->Set(ArgConverter::ConvertToV8String(isolate, "terminate"),
                   terminateWorkerFuncTemplate);

    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "Worker"),
                        workerFuncTemplate);
  }

  // Unhandled-promise-rejection tracker; fed by the SetPromiseRejectCallback
  // above and drained via an event-loop task once per looper turn.
  m_promiseRejections = std::make_unique<PromiseRejectionTracker>(this);

  SimpleProfiler::Init(isolate, globalTemplate);

  CallbackHandlers::CreateGlobalCastFunctions(isolate, globalTemplate);

  Local<Context> context = Context::New(isolate, nullptr, globalTemplate);

  auto global = context->Global();

  v8::Context::Scope contextScope{context};

  BuiltinLoader::RunBuiltin(context, BuiltinId::kBlobUrl);

  // Generic WHATWG event primitives (Event/EventTarget + the globalThis
  // mixin), then the error-events layer on top (ErrorEvent/
  // PromiseRejectionEvent, reportError and the native dispatch closures) -
  // installed for both the main and worker isolates.
  Events::Init(context);
  ErrorEvents::Init(context);

  StructuredClone::Init(context);

  // The WHATWG performance surface. After StructuredClone::Init:
  // mark/measure `detail` is cloned through the structuredClone global.
  Performance::Init(context);

  // The `interop` global (interop.escapeException), mirroring iOS.
  Interop::Init(context);

  m_objectManager->Init(isolate);

  m_module.Init(isolate, callingDir);

  Local<Value> gcFunc;
  global->Get(context, ArgConverter::ConvertToV8String(isolate, "gc"))
      .ToLocal(&gcFunc);
  if (!gcFunc.IsEmpty() && gcFunc->IsFunction()) {
    m_gcFunc = new Persistent<Function>(isolate, gcFunc.As<Function>());
  }

  global->DefineOwnProperty(context,
                            ArgConverter::ConvertToV8String(isolate, "global"),
                            global, readOnlyFlags);
  global->DefineOwnProperty(
      context, ArgConverter::ConvertToV8String(isolate, "__global"), global,
      readOnlyFlags);
  m_weakRef.Init(isolate, context);

  // Do not set 'self' accessor to main thread JavaScript
  if (!m_isMainThread) {
    global->DefineOwnProperty(context,
                              ArgConverter::ConvertToV8String(isolate, "self"),
                              global, readOnlyFlags);
  }

#ifdef APPLICATION_IN_DEBUG
  v8::Local<v8::Object> console =
      Console::createConsole(context, JsV8InspectorClient::consoleLogCallback,
                             maxLogcatObjectSize, forceLog);
#else
  v8::Local<v8::Object> console =
      Console::createConsole(context, nullptr, maxLogcatObjectSize, forceLog);
#endif

  /*
   * Attach 'console' object to the global object
   */
  global->DefineOwnProperty(context,
                            ArgConverter::ConvertToV8String(isolate, "console"),
                            console, readOnlyFlags);

  ArgConverter::Init(context);

  CallbackHandlers::Init(isolate);

  m_profiler.Init(isolate, global, packageName, profilerOutputDir);

  // Do not build metadata (which should be static for the process) for non-main
  // threads
  if (m_isMainThread) {
    MetadataNode::BuildMetadata(filesPath);
  }

  auto enableProfiler = !profilerOutputDir.empty();
  MetadataNode::EnableProfiler(enableProfiler);

  MetadataNode::CreateTopLevelNamespaces(isolate, global);

  ArrayHelper::Init(context);

  m_arrayBufferHelper.CreateConvertFunctions(context, global, m_objectManager);

  this->m_context = new Persistent<Context>(isolate, context);

  this->m_napiEnv = NapiEnv::Create(context, m_eventLoop);
  s_currentRuntime = this;

  if (m_isMainThread) {
    // Releases any runtime waiting in ElectMainRuntime: the metadata tree and
    // the main event loop they depend on are published by now.
    SignalMainRuntimeReady(false /* failed */);
  }

  return isolate;
}

jobject Runtime::ConvertJsValueToJavaObject(JEnv& env,
                                            const Local<Value>& value,
                                            int classReturnType) {
  auto context = this->GetContext();
  JsArgToArrayConverter argConverter(
      context, value, false /*is implementation object*/, classReturnType);
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

double Runtime::PerformanceNowMillis() {
  return (platform->MonotonicallyIncreasingTime() - m_timeOriginMonotonic) *
         1000.0;
}

void Runtime::DestroyRuntime() {
  CrashBreadcrumbs::UnregisterRuntime(m_id);
  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    s_id2RuntimeCache.erase(m_id);
    s_isolate2RuntimesCache.erase(m_isolate);
  }
  if (m_eventLoop != nullptr) {
    // runs on this runtime's own thread; children still holding a weak_ptr
    // and v8 teardown posts have their work dropped from now on
    m_eventLoop->Shutdown();
  }
  if (m_napiEnv != nullptr) {
    // After Shutdown so no queued Node-API entry can run against a dying env.
    // The env's reference lists hold v8::Globals, so its teardown needs the
    // isolate alive and locked; the Locker is reentrant for the worker path,
    // which already holds it here.
    v8::Locker locker(m_isolate);
    NapiEnv::Destroy(static_cast<NapiEnv*>(m_napiEnv));
    m_napiEnv = nullptr;
  }
  if (s_currentRuntime == this) {
    s_currentRuntime = nullptr;
  }
  // The events state holds v8::Global handles (backing event target, dispatch
  // closures and tracked promise rejections) - reset them while the isolate
  // is still alive.
  m_promiseRejections.reset();
  m_globalEventTarget.Reset();
  m_dispatchErrorEventFunc.Reset();
  m_dispatchUnhandledRejectionFunc.Reset();
  m_dispatchRejectionHandledFunc.Reset();
  m_dispatchNativeUncaughtErrorFunc.Reset();
  // Both hold v8::Global handles to JS callbacks, so their entries must be
  // dropped here rather than in ~Runtime, which runs after Isolate::Dispose --
  // resetting a Global then writes into a freed handle table. Doing it here
  // also closes a window in which the main thread could take a Locker on this
  // isolate (RunMainThreadEntry) after it had already been disposed.
  CallbackHandlers::RemoveIsolateEntries(m_isolate);
  FrameCallbacks::RemoveIsolateEntries(m_isolate);

  // Flag this isolate's in-flight async graph loads dead and Reset their
  // context Globals while the isolate is still alive, so fetch completions
  // still queued on background threads become no-ops. The rest of the loader
  // state (registries, waiters, loader vocabulary) lives in a RuntimeState
  // slot and is destroyed with it below. Worker isolates quiesce the same way.
  tns::QuiesceModuleLoadsForIsolate(m_isolate);
  // The transport's process-wide state (cache-bust marks, dev-boot flag) is
  // shared across isolates; only the main isolate may clear it (worker
  // teardown must not wipe the main isolate's session).
  if (m_isMainThread) {
    tns::CleanupHttpLoaderGlobals();
  }

  // V8 does not run weak callbacks when an isolate is disposed, so anything
  // still bound to one has to be deleted explicitly, here, while the isolate
  // is alive and its destructors can still touch v8::Global handles.
  IsolateTracked::SweepAll(m_isolate);

  // Same reason: every still-linked Java<->JS wrapper holds a Persistent, a
  // JSInstanceInfo and its weak-callback state, and those finalizers will
  // never run now. The JNI half of ObjectManager is released later, in its
  // destructor, once the isolate is gone.
  if (m_objectManager != nullptr) {
    m_objectManager->ReleaseAllRegistered();
  }

  // Everything below still needs the isolate alive -- the caller disposes it
  // only after this returns -- but runs after the hooks above so nothing they
  // touch is pulled out from under them.

  // Cached per-isolate string constants; its destructor resets 19 handles. The
  // slot is cleared so a stray lookup during the rest of teardown reads null
  // rather than freed memory.
  auto* consts = static_cast<V8StringConstants::PerIsolateV8Constants*>(
      m_isolate->GetData((uint32_t)Runtime::IsolateData::CONSTANTS));
  delete consts;
  m_isolate->SetData((uint32_t)Runtime::IsolateData::CONSTANTS, nullptr);

  if (m_gcFunc != nullptr) {
    m_gcFunc->Reset();
    delete m_gcFunc;
    m_gcFunc = nullptr;
  }
  if (m_context != nullptr) {
    m_context->Reset();
    delete m_context;
    m_context = nullptr;
  }

  // Last, so anything above still finds its state: this state holds
  // v8::Persistents, which have to be released while the isolate is alive.
  if (m_state != nullptr) {
    m_state->Clear();
  }
}

Local<Context> Runtime::GetContext() {
  return this->m_context->Get(this->m_isolate);
}

int Runtime::GetId() { return this->m_id; }


JavaVM* Runtime::s_jvm = nullptr;
jmethodID Runtime::GET_USED_MEMORY_METHOD_ID = nullptr;
robin_hood::unordered_map<int, Runtime*> Runtime::s_id2RuntimeCache;
robin_hood::unordered_map<Isolate*, Runtime*> Runtime::s_isolate2RuntimesCache;
std::mutex Runtime::s_runtimeCacheMutex;
std::atomic<bool> Runtime::s_mainThreadInitialized{false};
std::mutex Runtime::s_mainInitMutex;
std::condition_variable Runtime::s_mainInitReady;
bool Runtime::s_mainRuntimeElected = false;
bool Runtime::s_mainRuntimeFailed = false;
v8::Platform* Runtime::platform = nullptr;
int Runtime::m_androidVersion = Runtime::GetAndroidVersion();
std::shared_ptr<EventLoop> Runtime::s_mainEventLoop;

thread_local Runtime* Runtime::s_currentRuntime = nullptr;

napi_env Runtime::GetNapiEnvIfAlive(const Runtime* runtime) {
  if (runtime == nullptr) {
    return nullptr;
  }

  std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
  for (const auto& entry : s_isolate2RuntimesCache) {
    Runtime* candidate = entry.second;
    // The home-thread comparison closes the allocator-reuse (ABA) hole: a
    // stale thread-local can only exist on the dead runtime's home thread,
    // and a recycled same-address Runtime homed on this thread would have
    // overwritten that thread-local — so an address match homed on a foreign
    // thread can only be a recycled pointer.
    if (candidate == runtime && candidate->m_napiEnv != nullptr &&
        static_cast<NapiEnv*>(candidate->m_napiEnv)->HomeThread() ==
            std::this_thread::get_id()) {
      return candidate->m_napiEnv;
    }
  }
  return nullptr;
}
