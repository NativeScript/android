#include "Runtime.h"

#include <console/Console.h>
#include <dlfcn.h>
#include <unistd.h>
#include <unwind.h>
#include <cxxabi.h>

#include <chrono>
#include <csignal>
#include <cstring>
#include <mutex>
#include <sstream>
#include <thread>

#include "ArgConverter.h"
#include "ArrayHelper.h"
#include "CallbackHandlers.h"
#include "Constants.h"
#include "File.h"
#include "IsolateDisposer.h"
#include "JsArgConverter.h"
#include "JsArgToArrayConverter.h"
#include "ManualInstrumentation.h"
#include "MetadataNode.h"
#include "ModuleBinding.h"
#include "ModuleInternal.h"
#include "ModuleInternalCallbacks.h"
#include "NativeScriptAssert.h"
#include "NativeScriptException.h"
#include "SimpleAllocator.h"
#include "SimpleProfiler.h"
#include "URLImpl.h"
#include "HMRSupport.h"
#include "ModuleInternalCallbacks.h"
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

namespace {
struct SigHandlerBacktraceState {
  void** current;
  void** end;
};

// libunwind callback that walks the C++ stack frame-by-frame. Captures the
// PC for each frame into the passed array up to capacity. Signal-handler
// safe (does not allocate, does not call into libc beyond the unwinder).
_Unwind_Reason_Code SigHandlerUnwindCallback(struct _Unwind_Context* ctx, void* arg) {
  auto* state = static_cast<SigHandlerBacktraceState*>(arg);
  uintptr_t pc = _Unwind_GetIP(ctx);
  if (pc) {
    if (state->current == state->end) {
      return _URC_END_OF_STACK;
    }
    *state->current++ = reinterpret_cast<void*>(pc);
  }
  return _URC_NO_REASON;
}

// Format one backtrace frame to logcat. We use dladdr to resolve the shared
// object and symbol; if the symbol is mangled we attempt to demangle.
// Output looks like:
//   #07 pc 0x00000000004a8b0c  libNativeScript.so (tns::Runtime::Init+12)
// Matching the format Android's stock crash dump uses so it's familiar.
void LogBacktraceFrame(int idx, void* pc) {
  Dl_info info{};
  const char* libName = "?";
  const char* symName = "?";
  uintptr_t relPc = reinterpret_cast<uintptr_t>(pc);
  uintptr_t symOff = 0;
  char* demangled = nullptr;

  if (dladdr(pc, &info) && info.dli_fname) {
    libName = info.dli_fname;
    if (info.dli_fbase) {
      relPc = relPc - reinterpret_cast<uintptr_t>(info.dli_fbase);
    }
    if (info.dli_sname) {
      symName = info.dli_sname;
      symOff = reinterpret_cast<uintptr_t>(pc) - reinterpret_cast<uintptr_t>(info.dli_saddr);
      int status = 0;
      demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
      if (status == 0 && demangled) {
        symName = demangled;
      }
    }
  }

  __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                      "  #%02d pc 0x%016lx  %s (%s+%lu)", idx,
                      static_cast<unsigned long>(relPc), libName, symName,
                      static_cast<unsigned long>(symOff));
  if (demangled) free(demangled);
}
}  // namespace

void SIG_handler(int sigNumber, siginfo_t* sigInfo, void* /*ucontext*/) {
  // Reset all fatal signal handlers to default IMMEDIATELY. If the body of
  // this handler itself crashes (e.g. malloc is broken because the original
  // SEGV was in the allocator), the second signal will kill the process
  // cleanly and let Android write a proper tombstone, instead of looping
  // forever between handler and signal.
  ::signal(SIGABRT, SIG_DFL);
  ::signal(SIGSEGV, SIG_DFL);
  ::signal(SIGBUS,  SIG_DFL);
  ::signal(SIGFPE,  SIG_DFL);
  ::signal(SIGILL,  SIG_DFL);

  // Async-signal-handler caveats: we intentionally avoid std::stringstream
  // and other allocating code on the signal stack. __android_log_print and
  // dladdr are not strictly signal-safe but are reliable on Android in
  // practice and give us the diagnostic we need.
  const char* sigName = "UNKNOWN";
  switch (sigNumber) {
    case SIGABRT: sigName = "SIGABRT"; break;
    case SIGSEGV: sigName = "SIGSEGV"; break;
    case SIGBUS:  sigName = "SIGBUS"; break;
    case SIGFPE:  sigName = "SIGFPE"; break;
    case SIGILL:  sigName = "SIGILL"; break;
    default:      sigName = "Signal"; break;
  }

  // ── Header (must precede backtrace so it isn't lost when the handler
  //    throws and unwinds the stack). ──────────────────────────────────
  __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                      "=== Native crash caught by NativeScript runtime ===");
  __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                      "signal %d (%s), code %d, fault addr %p, tid %d",
                      sigNumber, sigName,
                      sigInfo ? sigInfo->si_code : -1,
                      sigInfo ? sigInfo->si_addr : nullptr,
                      static_cast<int>(gettid()));

  // ── C++ backtrace via _Unwind_Backtrace. Capped at 64 frames so we
  //    never run the signal stack out. ────────────────────────────────
  constexpr size_t kMaxFrames = 64;
  void* frames[kMaxFrames] = {};
  SigHandlerBacktraceState state = {frames, frames + kMaxFrames};
  _Unwind_Backtrace(&SigHandlerUnwindCallback, &state);
  const int frameCount = static_cast<int>(state.current - frames);
  __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                      "backtrace (%d frames):", frameCount);
  for (int i = 0; i < frameCount; ++i) {
    LogBacktraceFrame(i, frames[i]);
  }
  __android_log_print(ANDROID_LOG_FATAL, "TNS.Native",
                      "=== end native crash ===");

  // Existing behavior: throw so JS-side error pipeline still reports.
  // Note: throwing from a signal handler is technically UB, but the existing
  // runtime has relied on it for years and works under libgcc/libunwind+itanium-abi.
  stringstream msg;
  msg << "JNI Exception occurred (" << sigName
      << ").\n=======\nCheck the 'adb logcat' for additional information about "
         "the error.\n=======\n";
  throw NativeScriptException(msg.str());
}

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
  __android_log_print(ANDROID_LOG_INFO, "TNS.Runtime",
                      "NativeScript Runtime Version %s, commit %s",
                      NATIVE_SCRIPT_RUNTIME_VERSION,
                      NATIVE_SCRIPT_RUNTIME_COMMIT_SHA);

  if (Runtime::s_jvm == nullptr) {
    s_jvm = vm;

    JEnv::Init(s_jvm);
  }

  // handle SIGABRT/SIGSEGV only on API level > 20 as the handling is not so
  // efficient in older versions
  if (m_androidVersion > 20) {
    // Install an alternate signal stack BEFORE registering the handler so
    // that we can still produce a meaningful backtrace even when the crash
    // was caused by a stack overflow on the main JS thread (the original
    // stack is full and would deadlock the handler otherwise).
    // NDK r23+ defines SIGSTKSZ via sysconf so it isn't constexpr — use a
    // fixed 64 KiB which comfortably covers Android's MINSIGSTKSZ.
    constexpr size_t kAltStackSize = 64 * 1024;
    static thread_local char altStackBuf[kAltStackSize];
    stack_t altStack{};
    altStack.ss_sp = altStackBuf;
    altStack.ss_size = kAltStackSize;
    altStack.ss_flags = 0;
    sigaltstack(&altStack, nullptr);

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    sigemptyset(&action.sa_mask);
    // SA_SIGINFO enables the 3-arg handler so we get siginfo_t (fault addr
    // and si_code). SA_ONSTACK lets the handler run on an alternate stack
    // — important so we still produce a useful backtrace if the original
    // crash was a stack overflow.
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    action.sa_sigaction = SIG_handler;
    sigaction(SIGABRT, &action, NULL);
    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGBUS,  &action, NULL);
    sigaction(SIGFPE,  &action, NULL);
    sigaction(SIGILL,  &action, NULL);
  }
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
  m_loopTimer = new MessageLoopTimer();
  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    s_id2RuntimeCache.emplace(id, this);
  }

  if (GET_USED_MEMORY_METHOD_ID == nullptr) {
    auto RUNTIME_CLASS = env->FindClass("com/tns/Runtime");
    assert(RUNTIME_CLASS != nullptr);

    GET_USED_MEMORY_METHOD_ID =
        env->GetMethodID(RUNTIME_CLASS, "getUsedMemory", "()J");
    assert(GET_USED_MEMORY_METHOD_ID != nullptr);
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
  auto runtime = static_cast<Runtime*>(
      isolate->GetData((uint32_t)Runtime::IsolateData::RUNTIME));
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
  void* maybeRuntime =
      isolate->GetData((uint32_t)Runtime::IsolateData::RUNTIME);
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

  runtime->Init(env, filesPath, nativeLibDir, enableLog, isDebuggable,
                packageName, args, callingDir, maxLogcatObjectSize, forceLog);
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

  Constants::APP_ROOT_FOLDER_PATH = filesRoot + "/app/";
  // read config options passed from Java
  // Indices correspond to positions in the com.tns.AppConfig.KnownKeys enum
  JniLocalRef v8Flags(env->GetObjectArrayElement(args, 0));
  Constants::V8_STARTUP_FLAGS = ArgConverter::jstringToString(v8Flags);
  JniLocalRef cacheCode(env->GetObjectArrayElement(args, 1));
  Constants::V8_CACHE_COMPILED_CODE = (bool)cacheCode;
  JniLocalRef profilerOutputDir(env->GetObjectArrayElement(args, 2));

  DEBUG_WRITE("Initializing Telerik NativeScript");

  auto profilerOutputDirStr = ArgConverter::jstringToString(profilerOutputDir);

  NativeScriptException::Init();
  m_isolate = PrepareV8Runtime(
      filesRoot, nativeLibDirStr, packageNameStr, isDebuggable, callingDirStr,
      profilerOutputDirStr, maxLogcatObjectSize, forceLog);
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

void Runtime::RunModule(const char* moduleName) {
  auto context = this->GetContext();
  m_module.Load(context, moduleName);
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
  ScriptOrigin origin(isolate,
                      ArgConverter::ConvertToV8String(isolate, filename));
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

void Runtime::PassExceptionToJsNative(JNIEnv* env, jobject obj,
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

  // pass err to JS
  NativeScriptException::CallJsFuncWithErr(errObj, isDiscarded);
}

static void InitializeV8() {
  Runtime::platform = v8::platform::NewDefaultPlatform().release();
  V8::InitializePlatform(Runtime::platform);
  V8::Initialize();
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

  /*
   * Setup the V8Platform only once per process - once for the application
   * lifetime Don't execute again if main thread has already been initialized
   */
  if (!s_mainThreadInitialized) {
    InitializeV8();
  }

  tns::instrumentation::Frame isolateFrame;
  auto isolate = Isolate::New(create_params);
  // Capture start and realtime origin
  // MonotonicallyIncreasingTime returns seconds as double; store for
  // performance.now()
  m_startTime = platform->MonotonicallyIncreasingTime();
  m_realtimeOrigin = platform->CurrentClockTimeMillis();
  isolateFrame.log("Isolate.New");

  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    s_isolate2RuntimesCache[isolate] = this;
  }
  v8::Locker locker(isolate);
  Isolate::Scope isolate_scope(isolate);
  HandleScope handleScope(isolate);

  m_objectManager->SetInstanceIsolate(isolate);

  // Sets a structure with v8 String constants on the isolate object at slot 1
  auto consts = new V8StringConstants::PerIsolateV8Constants(isolate);
  isolate->SetData((uint32_t)Runtime::IsolateData::RUNTIME, this);
  isolate->SetData((uint32_t)Runtime::IsolateData::CONSTANTS, consts);

  V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(),
                         Constants::V8_STARTUP_FLAGS.size());
  isolate->SetCaptureStackTraceForUncaughtExceptions(true, 100,
                                                     StackTrace::kOverview);

  // Set up import.meta callback
  isolate->SetHostInitializeImportMetaObjectCallback(
      InitializeImportMetaObject);

  // Enable dynamic import() support
  isolate->SetHostImportModuleDynamicallyCallback(
      ImportModuleDynamicallyCallback);

  isolate->AddMessageListener(NativeScriptException::OnUncaughtError);

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

  // performance object (performance.now() + timeOrigin)
  {
    auto performanceTemplate = ObjectTemplate::New(isolate);
    auto nowFunc =
        FunctionTemplate::New(isolate, Runtime::PerformanceNowCallback);
    performanceTemplate->Set(ArgConverter::ConvertToV8String(isolate, "now"),
                             nowFunc);
    performanceTemplate->Set(
        ArgConverter::ConvertToV8String(isolate, "timeOrigin"),
        Number::New(isolate, m_realtimeOrigin));
    globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "performance"),
                        performanceTemplate);
  }
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
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__postFrameCallback"),
      FunctionTemplate::New(isolate, CallbackHandlers::PostFrameCallback));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "__removeFrameCallback"),
      FunctionTemplate::New(isolate, CallbackHandlers::RemoveFrameCallback));
  globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "URL"),
                      URLImpl::GetCtor(isolate));
  globalTemplate->Set(
      ArgConverter::ConvertToV8String(isolate, "URLSearchParams"),
      URLSearchParamsImpl::GetCtor(isolate));
  globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "URLPattern"),
                      URLPatternImpl::GetCtor(isolate));

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

    ALooper_addFd(m_mainLooper, m_mainLooper_fd[0], ALOOPER_POLL_CALLBACK,
                  ALOOPER_EVENT_INPUT,
                  CallbackHandlers::RunOnMainThreadFdCallback, nullptr);
  }
  /*
   * Emulate a `WorkerGlobalScope`
   * Attach 'postMessage', 'close' to the global object
   */
  else {
    m_isMainThread = false;
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

    // Main-thread-only HMR helper: `globalThis.__nsTerminateAllWorkers()`.
    // Returns the count of workers terminated. HMR runtimes (e.g.
    // @nativescript/vite) call this before re-bootstrapping the JS app so a
    // cycle that re-runs a Worker-constructing scope doesn't leak a live
    // worker. Workers never receive this global — a stuck worker shouldn't be
    // able to take down its peers.
    //
    // Debug/dev only: it lets any in-process JS terminate every worker, so it
    // must not ship in release. Gated on `isDebuggable` like the rest of the
    // dev-global surface installed by `InitializeHmrDevGlobals` below.
    if (isDebuggable) {
      Local<FunctionTemplate> terminateAllWorkersTemplate = FunctionTemplate::New(
          isolate, CallbackHandlers::TerminateAllWorkersCallback);
      globalTemplate->Set(ArgConverter::ConvertToV8String(isolate, "__nsTerminateAllWorkers"),
                          terminateAllWorkersTemplate);
    }
  }

  /*
   * Per-runtime task queue used by child workers to deliver messages, errors
   * and cleanup notifications to this runtime's thread. The looper exists for
   * every runtime: Java prepares it before initNativeScript on both the main
   * and worker threads.
   */
  m_looperTasks = std::make_shared<LooperTasks>();
  m_looperTasks->Initialize(ALooper_forThread());

  SimpleProfiler::Init(isolate, globalTemplate);

  CallbackHandlers::CreateGlobalCastFunctions(isolate, globalTemplate);

  Local<Context> context = Context::New(isolate, nullptr, globalTemplate);

  auto global = context->Global();

  v8::Context::Scope contextScope{context};

  // Install URL.createObjectURL / URL.revokeObjectURL / blob registry /
  // URL.searchParams accessor. The script literal lives in `URLImpl` so
  // it can be shared between runtimes.
  URLImpl::InstallBlobMethods(context);

  // Install HMR + dev-session JS-callable globals on the main-thread
  // isolate ONLY, and ONLY in a debuggable/dev build. Workers don't need
  // (and would race on) the dev-session surface — the import-map, vendor
  // registry, and per-module hot data all live on the main thread.
  //
  // The `isDebuggable` gate is required: this surface includes
  // `__nsStartDevSession` / `__nsConfigureRuntime` which mutate the
  // process-wide import map and can drive module loading, so it must be
  // absent from release binaries. The actual remote fetch is independently
  // gated by `DevFlags::IsRemoteUrlAllowed`, but the install itself should
  // not happen in release. (`isDebuggable` is the PrepareV8Runtime param.)
  if (m_isMainThread && isDebuggable) {
    tns::InitializeHmrDevGlobals(isolate, context);
  }
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
  if (s_mainThreadInitialized) {
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

void Runtime::PerformanceNowCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  auto isolate = args.GetIsolate();
  auto runtime = Runtime::GetRuntime(isolate);
  // Difference in seconds * 1000 for ms
  double ms =
      (platform->MonotonicallyIncreasingTime() - runtime->m_startTime) * 1000.0;
  args.GetReturnValue().Set(ms);
}

void Runtime::DestroyRuntime() {
  {
    std::lock_guard<std::mutex> lock(s_runtimeCacheMutex);
    s_id2RuntimeCache.erase(m_id);
    s_isolate2RuntimesCache.erase(m_isolate);
  }
  if (m_looperTasks != nullptr) {
    // runs on this runtime's own thread; children still holding a weak_ptr
    // will have their posts dropped from now on
    m_looperTasks->Terminate();
  }
  tns::disposeIsolate(m_isolate);
}

Local<Context> Runtime::GetContext() {
  return this->m_context->Get(this->m_isolate);
}

int Runtime::GetId() { return this->m_id; }

int Runtime::GetWriter() { return m_mainLooper_fd[1]; }

int Runtime::GetReader() { return m_mainLooper_fd[0]; }

JavaVM* Runtime::s_jvm = nullptr;
jmethodID Runtime::GET_USED_MEMORY_METHOD_ID = nullptr;
robin_hood::unordered_map<int, Runtime*> Runtime::s_id2RuntimeCache;
robin_hood::unordered_map<Isolate*, Runtime*> Runtime::s_isolate2RuntimesCache;
std::mutex Runtime::s_runtimeCacheMutex;
bool Runtime::s_mainThreadInitialized = false;
v8::Platform* Runtime::platform = nullptr;
int Runtime::m_androidVersion = Runtime::GetAndroidVersion();
ALooper* Runtime::m_mainLooper = nullptr;
int Runtime::m_mainLooper_fd[2];
