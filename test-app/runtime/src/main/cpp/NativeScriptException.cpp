#include "NativeScriptException.h"

#include <algorithm>
#include <atomic>
#include <sstream>

#include "ArgConverter.h"
#include "ErrorEvents.h"
#include "Interop.h"
#include "EventLoop.h"
#include "NativeScriptAssert.h"
#include "Runtime.h"
#include "RuntimeState.h"
#include "Util.h"
#include "V8GlobalHelpers.h"
#include "V8StringConstants.h"
#include "WorkerWrapper.h"

using namespace std;
using namespace tns;
using namespace v8;

/* Defined below, next to ContainUncaughtCallbackException. */
static void MarkReportedToJs(Isolate* isolate, Local<Value> error);
static bool IsMarkedReportedToJs(Isolate* isolate, Local<Value> error);

namespace {

/*
 * Per-runtime table of JS errors handed to Java as
 * com.tns.NativeScriptException's `jsValueAddress`.
 *
 * Java used to be given the raw v8::Persistent<v8::Value>* and had no way to
 * free it, so every JS error crossing into Java pinned its Error object -- and
 * the stack it captured -- for the life of the process. It now gets an opaque
 * id into this table instead. An entry is dropped when the error is converted
 * back to JS, when the throwable holding the id is collected, and at the latest
 * when the runtime is torn down: RuntimeState destroys the table while the
 * isolate is still alive, which is what resetting a v8::Global requires.
 */
struct JsErrorHandles {
    struct Entry {
        v8::Global<v8::Value> value;
        /*
         * Weak: an id must not keep its throwable alive, and a cleared ref
         * means nothing can ever ask for this entry again.
         */
        jweak throwable = nullptr;
    };

    ~JsErrorHandles() {
        JEnv env;
        for (auto& entry : entries) {
            if (entry.second.throwable != nullptr) {
                env.DeleteWeakGlobalRef(entry.second.throwable);
            }
        }
    }

    // Drops entries whose throwable has been collected. Amortized by only
    // sweeping once the table has doubled since the last sweep.
    void PruneIfDue(JEnv& env) {
        if (entries.size() < pruneAt) {
            return;
        }

        for (auto it = entries.begin(); it != entries.end();) {
            jweak throwable = it->second.throwable;
            if (throwable != nullptr && env.isSameObject(throwable, nullptr)) {
                env.DeleteWeakGlobalRef(throwable);
                it = entries.erase(it);
            } else {
                ++it;
            }
        }

        pruneAt = std::max<size_t>(16, entries.size() * 2);
    }

    size_t pruneAt = 16;
    robin_hood::unordered_map<int64_t, Entry> entries;
};

/*
 * Ids are unique process-wide rather than per runtime. A throwable can be
 * converted back to JS on a runtime other than the one that created it (it
 * outlives its runtime, or crosses threads), and a per-runtime counter would
 * make that lookup hit an unrelated entry with the same id instead of missing.
 * Being globally unique is what makes `entries.find(id)` the ownership check:
 * a foreign id is simply absent, and the caller falls back to rebuilding the
 * error from the Java throwable.
 */
std::atomic<int64_t> g_nextJsErrorId{1};

/*
 * Files `error` in this runtime's table under a process-wide unique id.
 * Returns 0 when the runtime is tearing down, which the Java side reads as
 * "no JS value".
 */
int64_t StoreJsError(Isolate* isolate, Local<Value> error) {
    auto* handles = RuntimeState::For<JsErrorHandles>(isolate);
    if (handles == nullptr) {
        return 0;
    }

    JEnv env;
    handles->PruneIfDue(env);

    int64_t id = g_nextJsErrorId.fetch_add(1, std::memory_order_relaxed);
    JsErrorHandles::Entry entry;
    entry.value.Reset(isolate, error);
    handles->entries.emplace(id, std::move(entry));

    return id;
}

/*
 * Ties an entry's lifetime to the throwable that carries its id, so the JS
 * error is released once Java is done with the exception. Drops the entry
 * outright if the throwable could not be created.
 */
void BindJsErrorToThrowable(JEnv& env, Isolate* isolate, int64_t id,
                            jthrowable throwable) {
    if (id == 0) {
        return;
    }

    auto* handles = RuntimeState::For<JsErrorHandles>(isolate);
    if (handles == nullptr) {
        return;
    }

    auto it = handles->entries.find(id);
    if (it == handles->entries.end()) {
        return;
    }

    if (throwable == nullptr) {
        handles->entries.erase(it);
        return;
    }

    it->second.throwable = env.NewWeakGlobalRef(throwable);
}

/*
 * Removes the entry and hands back its value. False when the id belongs to
 * another runtime, has already been taken, or the runtime is tearing down --
 * the caller then rebuilds the error from the Java throwable.
 */
bool TakeJsError(Isolate* isolate, int64_t id, Local<Value>& out) {
    auto* handles = RuntimeState::For<JsErrorHandles>(isolate);
    if (handles == nullptr) {
        return false;
    }

    auto it = handles->entries.find(id);
    if (it == handles->entries.end()) {
        return false;
    }

    out = Local<Value>::New(isolate, it->second.value);
    if (it->second.throwable != nullptr) {
        JEnv env;
        env.DeleteWeakGlobalRef(it->second.throwable);
    }
    handles->entries.erase(it);

    return true;
}

std::shared_ptr<Persistent<Value>> MakeOwnedPersistent(Isolate* isolate,
                                                       Local<Value> value) {
    return std::shared_ptr<Persistent<Value>>(
        new Persistent<Value>(isolate, value), [](Persistent<Value>* handle) {
            // Released on the runtime's own thread while the isolate is alive:
            // a NativeScriptException never outlives the handling of the throw
            // that produced it.
            handle->Reset();
            delete handle;
        });
}

}  // namespace

// The throwable is held as a JNI LOCAL ref, and the message is rendered
// lazily from it (what()/ToString) - so the exception must be consumed
// before any JNI local frame enclosing this construction is popped.
NativeScriptException::NativeScriptException(JEnv& env)
    : m_javascriptException(nullptr) {
  m_javaException = JniLocalRef(env.ExceptionOccurred());
  env.ExceptionClear();
}

NativeScriptException::NativeScriptException(const string& message)
    : m_javascriptException(nullptr),
      m_javaException(JniLocalRef()),
      m_message(message) {}

NativeScriptException::NativeScriptException(const string& message,
                                             const string& stackTrace)
    : m_javascriptException(nullptr),
      m_javaException(JniLocalRef()),
      m_message(message),
      m_stackTrace(stackTrace) {}

NativeScriptException::NativeScriptException(TryCatch& tc,
                                             const string& message)
    : m_javaException(JniLocalRef()) {
  auto isolate = Isolate::GetCurrent();
  auto ex = tc.Exception();
  m_javascriptException =
      ex.IsEmpty() ? nullptr : MakeOwnedPersistent(isolate, ex);

  // A terminated isolate carries no message object and no inspectable
  // exception - every accessor below hands back an empty handle. Resetting
  // does not cancel the isolate's pending termination.
  if (tc.HasTerminated() || tc.Message().IsEmpty()) {
    m_message = message.empty() ? "Execution terminated." : message;
    tc.Reset();
    return;
  }

  m_message = GetErrorMessage(tc.Message(), ex, message);
  m_stackTrace = GetErrorStackTrace(tc.Message()->GetStackTrace());
  m_fullMessage = GetFullMessage(tc, m_message);
  tc.Reset();
}

void NativeScriptException::ReThrowToV8() {
  auto isolate = Isolate::GetCurrent();
  auto context = isolate->GetCurrentContext();
  Local<Value> errObj;

  if (m_javascriptException != nullptr) {
    errObj = Local<Value>::New(isolate, *m_javascriptException);
    if (errObj->IsObject()) {
      if (!m_fullMessage.empty()) {
        errObj.As<Object>()->Set(
            context, ArgConverter::ConvertToV8String(isolate, "fullMessage"),
            ArgConverter::ConvertToV8String(isolate, m_fullMessage));
      } else if (!m_message.empty()) {
        errObj.As<Object>()->Set(
            context, ArgConverter::ConvertToV8String(isolate, "fullMessage"),
            ArgConverter::ConvertToV8String(isolate, m_message));
      }
    }
  } else if (!m_fullMessage.empty()) {
    errObj = Exception::Error(
        ArgConverter::ConvertToV8String(isolate, m_fullMessage));
  } else if (!m_message.empty()) {
    errObj =
        Exception::Error(ArgConverter::ConvertToV8String(isolate, m_message));
  } else if (!m_javaException.IsNull()) {
    errObj = WrapJavaToJsException();
  } else {
    errObj = Exception::Error(ArgConverter::ConvertToV8String(
        isolate, "No javascript exception or message provided."));
  }

  isolate->ThrowException(errObj);
}

void NativeScriptException::ReThrowToJava() {
  jthrowable ex = nullptr;
  JEnv env;

  auto isolate = Isolate::GetCurrent();

  if (!m_javaException.IsNull()) {
    auto excClassName = ObjectManager::GetClassName((jobject)m_javaException);
    if (excClassName == "com/tns/NativeScriptException") {
      ex = m_javaException;
    } else {
      JniLocalRef msg(env.NewStringUTF("Java Error!"));
      ex = static_cast<jthrowable>(env.NewObject(
          NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID,
          (jstring)msg, (jobject)m_javaException));
    }
  } else if (m_javascriptException != nullptr) {
    /*
     * If exception is an object, then check if it has 'nativeException'
     * property and it if does then use it. If the found 'nativeException' is of
     * type different than com.tns.NativeScript then we should wrap it and keep
     * the JavaScript callstack as a message.
     * Otherwise create we have to create new exception object.
     */
    auto isolate = Isolate::GetCurrent();
    auto errObj = Local<Value>::New(isolate, *m_javascriptException);
    Interop::EscapedExceptionInfo escapeInfo;
    if (errObj->IsObject()) {
      Interop::GetEscapedExceptionInfo(env, errObj.As<Object>(), escapeInfo);
      // A branded interop.escapeException(...) throw carrying an original
      // Java throwable: rethrow it unwrapped, so a native catch of its
      // concrete type still matches (instead of receiving a
      // com.tns.NativeScriptException wrapper). The JS journey rides along as
      // a suppressed com.tns.JavaScriptStackTrace, which stack dumps and
      // crash reporters render automatically.
      if (escapeInfo.original != nullptr) {
        Interop::AttachJavaScriptStackTrace(env, escapeInfo.original,
                                            escapeInfo);
        env.Throw(escapeInfo.original);
        return;
      }
      auto exObj = TryGetJavaThrowableObject(env, errObj.As<Object>());
      ex = (jthrowable)exObj.Move();
    }

    JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
    JniLocalRef stackTrace(env.NewStringUTF(m_stackTrace.c_str()));

    if (ex == nullptr) {
      // An id into this runtime's table, not a pointer Java could never free.
      int64_t jsErrorId = StoreJsError(isolate, errObj);
      ex = static_cast<jthrowable>(env.NewObject(
          NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID,
          (jstring)msg, (jstring)stackTrace, (jlong)jsErrorId));
      BindJsErrorToThrowable(env, isolate, jsErrorId, ex);
    } else {
      auto excClassName = ObjectManager::GetClassName(ex);
      if (excClassName != "com/tns/NativeScriptException") {
        ex = static_cast<jthrowable>(
            env.NewObject(NATIVESCRIPTEXCEPTION_CLASS,
                          NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID, (jstring)msg,
                          (jstring)stackTrace, ex));
      }
    }

    // A branded escape with no underlying Java throwable takes the default
    // NativeScriptException shape, but its Java stack is the (identical) JNI
    // boundary machinery - replace it with frames synthesized from the JS
    // stack so crash reporters group these by where they actually happened.
    // The escapedFromJs mark exempts it from the deprecated
    // discardUncaughtJsExceptions handling in the Java dispatch layer.
    if (escapeInfo.branded && ex != nullptr) {
      Interop::ApplyJavaScriptFrames(env, ex, escapeInfo);
      static jfieldID escapedFromJsField =
          env.GetFieldID(NATIVESCRIPTEXCEPTION_CLASS, "escapedFromJs", "Z");
      env.SetBooleanField(ex, escapedFromJsField, JNI_TRUE);
    }

    // A `uncaughtErrorPolicy: "throw"` propagation whose report already ran
    // at the boundary: carry the mark so the post-mortem uncaught path does
    // not report the same failure a second time. `ex` is always a
    // com.tns.NativeScriptException at this point.
    if (ex != nullptr && IsMarkedReportedToJs(isolate, errObj)) {
      static jfieldID reportedToJsField =
          env.GetFieldID(NATIVESCRIPTEXCEPTION_CLASS, "reportedToJs", "Z");
      env.SetBooleanField(ex, reportedToJsField, JNI_TRUE);
    }
  } else if (!m_message.empty()) {
    JniLocalRef msg(env.NewStringUTF(m_message.c_str()));
    JniLocalRef stackTrace(env.NewStringUTF(m_stackTrace.c_str()));
    ex = static_cast<jthrowable>(env.NewObject(
        NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID,
        (jstring)msg, (jstring)stackTrace, (jlong)0));
  } else {
    JniLocalRef msg(env.NewStringUTF("No java exception or message provided."));
    ex = static_cast<jthrowable>(env.NewObject(
        NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID,
        (jstring)msg, (jstring) nullptr, (jlong)0));
  }
  env.Throw(ex);
}

void NativeScriptException::Init() {
  JEnv env;

  RUNTIME_CLASS = env.FindClass("com/tns/Runtime");
  NS_CHECK(RUNTIME_CLASS != nullptr);

  THROWABLE_CLASS = env.FindClass("java/lang/Throwable");
  NS_CHECK(THROWABLE_CLASS != nullptr);

  NATIVESCRIPTEXCEPTION_CLASS = env.FindClass("com/tns/NativeScriptException");
  NS_CHECK(NATIVESCRIPTEXCEPTION_CLASS != nullptr);

  NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID =
      env.GetMethodID(NATIVESCRIPTEXCEPTION_CLASS, "<init>",
                      "(Ljava/lang/String;Ljava/lang/String;J)V");
  NS_CHECK(NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID != nullptr);

  NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID = env.GetMethodID(
      NATIVESCRIPTEXCEPTION_CLASS, "<init>",
      "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Throwable;)V");
  NS_CHECK(NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID != nullptr);

  NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID =
      env.GetStaticMethodID(NATIVESCRIPTEXCEPTION_CLASS,
                            "getStackTraceAsString",
                            "(Ljava/lang/Throwable;)Ljava/lang/String;");
  NS_CHECK(NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID != nullptr);

  NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID =
      env.GetStaticMethodID(NATIVESCRIPTEXCEPTION_CLASS, "getMessage",
                            "(Ljava/lang/Throwable;)Ljava/lang/String;");
  NS_CHECK(NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID != nullptr);
}

std::string NativeScriptException::ToString() const {
  std::stringstream ss;
  if (!m_javaException.IsNull()) {
    JEnv env;
    std::string message = GetExceptionMessage(env, m_javaException);
    std::string stackTrace = GetExceptionStackTrace(env, m_javaException);
    ss << "Java Exception: " << message << "\n" << stackTrace;
  } else if (m_javascriptException != nullptr) {
    ss << "JavaScript Exception: " << m_message << "\n" << m_stackTrace;
  } else if (!m_message.empty()) {
    ss << "Exception Message: " << m_message << "\n" << m_stackTrace;
  } else {
    ss << "No exception information available.";
  }
  return ss.str();
}

std::string NativeScriptException::GetErrorMessage() const {
  if (!m_javaException.IsNull()) {
    JEnv env;
    return GetExceptionMessage(env, m_javaException);
  } else if (m_javascriptException != nullptr) {
    return m_message;
  } else {
    return m_message.empty() ? "No exception message available." : m_message;
  }
}

const char* NativeScriptException::what() const noexcept {
  m_whatCache = ToString();
  return m_whatCache.c_str();
}

// ON V8 UNCAUGHT EXCEPTION
void NativeScriptException::OnUncaughtError(Local<Message> message,
                                            Local<Value> error) {
  string errorMessage = GetErrorMessage(message, error);
  string stackTrace = GetErrorStackTrace(message->GetStackTrace());

  // Give WHATWG `error` event listeners a chance first; preventDefault()
  // fully handles the report and nothing is raised to Java.
  auto isolate = v8::Isolate::GetCurrent();
  if (ErrorEvents::DispatchError(isolate, error, errorMessage, stackTrace)) {
    return;
  }

  NativeScriptException e(errorMessage, stackTrace);
  e.ReThrowToJava();
}

static string ToDetailString(Isolate* isolate, Local<Value> value) {
  auto context = isolate->GetCurrentContext();
  Local<String> str;
  if (!value.IsEmpty() && value->ToDetailString(context).ToLocal(&str)) {
    return ArgConverter::ConvertToString(str);
  }
  return "";
}

static string GetStackTraceOfValue(Isolate* isolate, Local<Value> value) {
  if (value.IsEmpty()) {
    return "";
  }
  auto stackTrace = Exception::GetStackTrace(value);
  if (stackTrace.IsEmpty()) {
    return "";
  }
  return NativeScriptException::GetErrorStackTrace(stackTrace);
}

/*
 * Splits a multi-line message so it survives logcat's per-message limit,
 * mirroring PrintErrorMessage but always-on (these reports must be visible
 * without verbose logging).
 */
static void LogLines(const string& text) {
  stringstream ss(text);
  string line;
  while (getline(ss, line, '\n')) {
    DEBUG_WRITE_FORCE("%s", line.c_str());
  }
}

void NativeScriptException::OnPromiseRejected(v8::PromiseRejectMessage message) {
  auto promise = message.GetPromise();
  auto isolate = v8::Isolate::GetCurrent();
  auto runtime = Runtime::TryGetRuntime(isolate);
  if (runtime == nullptr || runtime->PromiseRejections() == nullptr) {
    return;
  }

  switch (message.GetEvent()) {
    case v8::kPromiseRejectWithNoHandler:
      runtime->PromiseRejections()->OnReject(promise, message.GetValue());
      break;
    case v8::kPromiseHandlerAddedAfterReject:
      runtime->PromiseRejections()->OnHandlerAdded(promise);
      break;
    default:
      // kPromiseResolveAfterResolved / kPromiseRejectAfterResolved are not
      // relevant to unhandled-rejection tracking.
      break;
  }
}

void NativeScriptException::ReportUnhandledRejection(Isolate* isolate,
                                                     Local<Promise> promise,
                                                     Local<Value> reason,
                                                     const string& stackTrace) {
  // Populate the legacy `stackTrace` property before dispatch so event
  // listeners see the same shape the hooks do (matches the sync path).
  if (reason->IsObject() && !stackTrace.empty()) {
    auto context = isolate->GetCurrentContext();
    reason.As<Object>()
        ->Set(context, V8StringConstants::GetStackTrace(isolate),
              ArgConverter::ConvertToV8String(isolate, stackTrace))
        .FromMaybe(false);
  }

  if (ErrorEvents::DispatchUnhandledRejection(isolate, promise, reason)) {
    return;
  }

  auto runtime = Runtime::TryGetRuntime(isolate);
  bool discard =
      runtime != nullptr && runtime->GetDiscardUncaughtJsExceptions();
  ReportFatalTail(isolate, reason, stackTrace, "Unhandled promise rejection:",
                  discard ? JNI_TRUE : JNI_FALSE);

  // The report is complete (event + hook + log, exactly once). Under the
  // "throw" policy the rejection is additionally handed to the native layer;
  // the thrown exception is marked reported-to-JS so the post-mortem
  // uncaught path does not report it a second time. The deprecated discard
  // flag disables the throw, matching iOS.
  if (runtime != nullptr && !discard &&
      runtime->GetUncaughtErrorPolicy() == Runtime::UncaughtErrorPolicy::Throw) {
    string message =
        "Unhandled promise rejection: " + ToDetailString(isolate, reason);
    ThrowUncaughtJsErrorToJava(message, stackTrace);
  }
}

void NativeScriptException::ReportFatalTail(Isolate* isolate,
                                            Local<Value> error,
                                            const string& stackOverride,
                                            const string& logPrefix,
                                            jboolean isDiscarded) {
  auto context = isolate->GetCurrentContext();

  string stackTrace = stackOverride;
  if (stackTrace.empty()) {
    stackTrace = GetStackTraceOfValue(isolate, error);
  }

  // Match the shape the existing __onUncaughtError contract expects: the
  // error object carries its stack as a `stackTrace` property (see
  // Runtime::PassExceptionToJsNative).
  if (error->IsObject() && !stackTrace.empty()) {
    error.As<Object>()
        ->Set(context, V8StringConstants::GetStackTrace(isolate),
              ArgConverter::ConvertToV8String(isolate, stackTrace))
        .FromMaybe(false);
  }

  CallJsFuncWithErr(error, isDiscarded);

  if (isDiscarded == JNI_TRUE) {
    // Deprecated legacy routing: quiet, like the old Java-side discard path.
    if (!logPrefix.empty()) {
      DEBUG_WRITE_FORCE("%s", logPrefix.c_str());
    }
    DEBUG_WRITE_FORCE("NativeScript discarding uncaught JS exception!");
    return;
  }

  if (!logPrefix.empty()) {
    DEBUG_WRITE_FORCE("%s", logPrefix.c_str());
  }
  LogLines(ToDetailString(isolate, error));
  if (!stackTrace.empty()) {
    LogLines(stackTrace);
  }
}

/*
 * Marks a JS error value whose report already ran (event + hook + log), via
 * an isolate-private symbol. ReThrowToJava carries the mark over to the Java
 * com.tns.NativeScriptException so the post-mortem uncaught-exception path
 * does not report the same failure a second time.
 */
static Local<Private> GetReportedToJsBrand(Isolate* isolate) {
  return Private::ForApi(
      isolate, ArgConverter::ConvertToV8String(isolate, "tns::reportedToJs"));
}

static void MarkReportedToJs(Isolate* isolate, Local<Value> error) {
  if (!error->IsObject()) {
    return;
  }
  auto context = isolate->GetCurrentContext();
  error.As<Object>()
      ->SetPrivate(context, GetReportedToJsBrand(isolate),
                   v8::True(isolate))
      .FromMaybe(false);
}

static bool IsMarkedReportedToJs(Isolate* isolate, Local<Value> error) {
  if (!error->IsObject()) {
    return false;
  }
  auto context = isolate->GetCurrentContext();
  return error.As<Object>()
      ->HasPrivate(context, GetReportedToJsBrand(isolate))
      .FromMaybe(false);
}

bool NativeScriptException::ContainUncaughtCallbackException(Isolate* isolate,
                                                             v8::TryCatch& tc) {
  auto runtime = Runtime::TryGetRuntime(isolate);
  if (runtime == nullptr) {
    return false;
  }

  // JS-initiated chain (JS -> Java -> JS): a JS frame below this boundary is
  // waiting for the exception - propagate so it reaches the outer JS catch
  // (the boundary machinery restores the original JS error object there).
  if (runtime->JavaCallDepth() > 0) {
    return false;
  }

  Local<Value> error = tc.Exception();
  if (error.IsEmpty()) {
    return false;
  }

  // Branded interop.escapeException: an explicit forward to the native
  // caller - never contained, never reported here.
  if (error->IsObject()) {
    JEnv env;
    Interop::EscapedExceptionInfo escapeInfo;
    if (Interop::GetEscapedExceptionInfo(env, error.As<Object>(), escapeInfo)) {
      if (escapeInfo.original != nullptr) {
        env.DeleteLocalRef(escapeInfo.original);
      }
      return false;
    }
  }

  // Report through the WHATWG pipeline at the decision point, for both
  // policies (same sequence as iOS): `error` event first - preventDefault()
  // fully contains the error even under the "throw" policy - then the legacy
  // hook and log.
  string errorMessage;
  string stackTrace;
  auto message = tc.Message();
  if (!message.IsEmpty()) {
    errorMessage = GetErrorMessage(message, error);
    stackTrace = GetErrorStackTrace(message->GetStackTrace());
  } else {
    errorMessage = ToDetailString(isolate, error);
    stackTrace = GetStackTraceOfValue(isolate, error);
  }

  // Populate the legacy `stackTrace` property before dispatch so event
  // listeners see the same shape the hooks do.
  if (error->IsObject() && !stackTrace.empty()) {
    auto context = isolate->GetCurrentContext();
    error.As<Object>()
        ->Set(context, V8StringConstants::GetStackTrace(isolate),
              ArgConverter::ConvertToV8String(isolate, stackTrace))
        .FromMaybe(false);
  }

  bool discard = runtime->GetDiscardUncaughtJsExceptions();
  {
    // A throwing listener or hook must not escape into the caller.
    v8::TryCatch reportTc(isolate);
    if (ErrorEvents::DispatchError(isolate, error, errorMessage, stackTrace)) {
      tc.Reset();
      return true;
    }
    ReportFatalTail(isolate, error, stackTrace, "",
                    discard ? JNI_TRUE : JNI_FALSE);
    if (reportTc.HasCaught()) {
      DEBUG_WRITE_FORCE(
          "ContainUncaughtCallbackException: exception while reporting");
    }
  }

  // The report is complete (exactly once). Under the "throw" policy the
  // exception is additionally propagated to the Java caller, marked so the
  // post-mortem uncaught path skips re-reporting. The deprecated discard
  // flag disables the throw, matching iOS.
  if (!discard &&
      runtime->GetUncaughtErrorPolicy() == Runtime::UncaughtErrorPolicy::Throw) {
    MarkReportedToJs(isolate, error);
    return false;
  }

  tc.Reset();
  return true;
}

void NativeScriptException::ThrowUncaughtJsErrorToJava(const string& message,
                                                       const string& stackTrace) {
  JEnv env;
  static jmethodID mid = env.GetStaticMethodID(
      RUNTIME_CLASS, "throwUncaughtJsErrorOnCurrentThread",
      "(Ljava/lang/String;Ljava/lang/String;)V");
  JniLocalRef msg(env.NewStringUTF(message.c_str()));
  JniLocalRef stack(env.NewStringUTF(stackTrace.c_str()));
  env.CallStaticVoidMethod(RUNTIME_CLASS, mid, (jstring)msg, (jstring)stack);
}

void PromiseRejectionTracker::OnReject(Local<Promise> promise,
                                       Local<Value> reason) {
  auto isolate = v8::Isolate::GetCurrent();
  for (auto& entry : pending_) {
    if (entry.promise.Get(isolate)->SameValue(promise)) {
      // Already tracked; refresh the reason for the latest rejection.
      entry.reason.Reset(isolate, reason);
      return;
    }
  }

  PendingRejection entry;
  entry.promise.Reset(isolate, promise);
  entry.reason.Reset(isolate, reason);
  pending_.push_back(std::move(entry));
  ScheduleDrain();
}

void PromiseRejectionTracker::OnHandlerAdded(Local<Promise> promise) {
  auto isolate = v8::Isolate::GetCurrent();

  // A handler attached before the rejection was drained cancels the report.
  for (auto it = pending_.begin(); it != pending_.end(); ++it) {
    if (it->promise.Get(isolate)->SameValue(promise)) {
      pending_.erase(it);
      return;
    }
  }

  // Otherwise, if the rejection was already reported and the promise is
  // still outstanding, queue a `rejectionhandled` event. OnHandlerAdded runs
  // during a microtask checkpoint, but spec fires rejectionhandled as a
  // task, so defer to the next drain instead of dispatching synchronously.
  for (auto it = reportedOutstanding_.begin(); it != reportedOutstanding_.end();
       ++it) {
    if (it->promise.IsEmpty()) {
      continue;
    }
    if (it->promise.Get(isolate)->SameValue(promise)) {
      ReportedRejection queued;
      // Re-anchor the promise strongly (the outstanding handle is weak) and
      // carry the original reason so the event reports it per spec.
      queued.promise.Reset(isolate, promise);
      queued.reason = std::move(it->reason);
      reportedOutstanding_.erase(it);
      pendingRejectionHandled_.push_back(std::move(queued));
      PruneReportedOutstanding();
      ScheduleDrain();
      return;
    }
  }
  PruneReportedOutstanding();
}

void PromiseRejectionTracker::PruneReportedOutstanding() {
  reportedOutstanding_.erase(
      std::remove_if(
          reportedOutstanding_.begin(), reportedOutstanding_.end(),
          [](const ReportedRejection& r) { return r.promise.IsEmpty(); }),
      reportedOutstanding_.end());
}

void PromiseRejectionTracker::ScheduleDrain() {
  if (drainScheduled_) {
    return;
  }
  drainScheduled_ = true;

  auto eventLoop = runtime_->GetEventLoop();
  if (eventLoop == nullptr) {
    drainScheduled_ = false;
    return;
  }

  // The task runs on the runtime's own looper thread, strictly after the
  // microtask checkpoint of the turn that produced the rejection. It is
  // dropped (never runs) once the runtime's event loop is shut down, so
  // capturing the raw Runtime pointer is safe.
  Runtime* runtime = runtime_;
  eventLoop->PostInternal([runtime]() {
    auto isolate = runtime->GetIsolate();
    v8::Locker locker(isolate);
    Isolate::Scope isolateScope(isolate);
    HandleScope handleScope(isolate);
    auto context = runtime->GetContext();
    Context::Scope contextScope(context);

    auto tracker = runtime->PromiseRejections();
    if (tracker != nullptr) {
      tracker->Drain();
    }
  });
}

/*
 * Gives a worker's global `onerror` a chance to handle a rejected reason,
 * mirroring CallbackHandlers::CallWorkerScopeOnErrorHandle (which passes the
 * message as a string). Returns true when the handler signalled it consumed
 * the error (truthy return).
 */
static bool GiveWorkerOnErrorAChance(Isolate* isolate, Local<Context> context,
                                     const string& message) {
  auto global = context->Global();
  Local<Value> onErrorVal;
  if (!global->Get(context, ArgConverter::ConvertToV8String(isolate, "onerror"))
           .ToLocal(&onErrorVal) ||
      onErrorVal.IsEmpty() || !onErrorVal->IsFunction()) {
    return false;
  }

  auto onError = onErrorVal.As<Function>();
  Local<Value> args[] = {ArgConverter::ConvertToV8String(isolate, message)};
  Local<Value> result;
  TryCatch tc(isolate);
  bool success =
      onError->Call(context, Undefined(isolate), 1, args).ToLocal(&result);
  return success && !result.IsEmpty() && result->BooleanValue(isolate);
}

void PromiseRejectionTracker::Drain() {
  drainScheduled_ = false;
  if (draining_) {
    return;
  }
  draining_ = true;

  auto isolate = runtime_->GetIsolate();
  auto context = isolate->GetCurrentContext();

  // Fire queued rejectionhandled events first (they were deferred from a
  // microtask checkpoint to run as a task on this drain turn), carrying the
  // retained original rejection reason.
  std::vector<ReportedRejection> handledSnapshot;
  handledSnapshot.swap(pendingRejectionHandled_);

  // Rejections that arrive while a drain is in progress accumulate into a
  // fresh vector and re-schedule their own drain.
  std::vector<PendingRejection> snapshot;
  snapshot.swap(pending_);

  for (auto& queued : handledSnapshot) {
    if (queued.promise.IsEmpty()) {
      continue;
    }
    try {
      auto promise = queued.promise.Get(isolate);
      Local<Value> reason = queued.reason.IsEmpty()
                                ? Undefined(isolate).As<Value>()
                                : queued.reason.Get(isolate);
      ErrorEvents::DispatchRejectionHandled(isolate, promise, reason);
    } catch (NativeScriptException& ex) {
      DEBUG_WRITE_FORCE(
          "PromiseRejectionTracker: exception while firing rejectionhandled: %s",
          ex.GetErrorMessage().c_str());
    }
  }

  auto workerWrapper = WorkerWrapper::FromIsolate(isolate);

  for (auto& entry : snapshot) {
    try {
      auto promise = entry.promise.Get(isolate);
      Local<Value> reason = entry.reason.IsEmpty()
                                ? Undefined(isolate).As<Value>()
                                : entry.reason.Get(isolate);

      string stackTrace = GetStackTraceOfValue(isolate, reason);

      // Populate the legacy `stackTrace` property before dispatch so event
      // listeners see it - covers both the worker and main branches below
      // (ReportUnhandledRejection's own set is idempotent).
      if (reason->IsObject() && !stackTrace.empty()) {
        reason.As<Object>()
            ->Set(context, V8StringConstants::GetStackTrace(isolate),
                  ArgConverter::ConvertToV8String(isolate, stackTrace))
            .FromMaybe(false);
      }

      if (workerWrapper != nullptr) {
        // Dispatch the rejection event on the worker's own global first;
        // preventDefault() there fully handles it. Only when unprevented fall
        // through to the existing worker channel (worker-global onerror ->
        // the parent Worker object's onerror).
        if (!ErrorEvents::DispatchUnhandledRejection(isolate, promise,
                                                     reason)) {
          string message =
              "Unhandled promise rejection: " + ToDetailString(isolate, reason);
          if (!GiveWorkerOnErrorAChance(isolate, context, message) &&
              !workerWrapper->IsTerminating() && !workerWrapper->IsDisposed()) {
            workerWrapper->PassUncaughtExceptionFromWorkerToParent(
                message, "", stackTrace, 0);
          }
        }
      } else {
        NativeScriptException::ReportUnhandledRejection(isolate, promise,
                                                        reason, stackTrace);
      }

      // The rejection has now been reported (unhandledrejection fired,
      // prevented or not). Keep the promise as a weak outstanding entry so a
      // handler attached later fires rejectionhandled; a GC'd promise drops
      // out on its own.
      ReportedRejection outstanding;
      outstanding.promise = std::move(entry.promise);
      outstanding.reason = std::move(entry.reason);
      reportedOutstanding_.push_back(std::move(outstanding));
      reportedOutstanding_.back().promise.SetWeak();
    } catch (NativeScriptException& ex) {
      DEBUG_WRITE_FORCE(
          "PromiseRejectionTracker: exception while reporting rejection: %s",
          ex.GetErrorMessage().c_str());
    }
  }

  PruneReportedOutstanding();

  draining_ = false;
}

void NativeScriptException::CallJsFuncWithErr(Local<Value> errObj,
                                              jboolean isDiscarded) {
  auto isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  auto context = isolate->GetCurrentContext();
  auto globalHandle = context->Global();

  Local<Value> handler;
  if (isDiscarded) {
    globalHandle->Get(context, V8StringConstants::GetDiscardedError(isolate))
        .ToLocal(&handler);
  } else {
    globalHandle->Get(context, V8StringConstants::GetUncaughtError(isolate))
        .ToLocal(&handler);
  }
  auto isEmpty = handler.IsEmpty();
  auto isFunction = handler->IsFunction();

  if (!isEmpty && isFunction) {
    auto thiz = Object::New(isolate);
    auto func = handler.As<Function>();

    func->Call(context, thiz, 1, &errObj);
  }
}

Local<Value> NativeScriptException::WrapJavaToJsException() {
  Local<Value> errObj;

  JEnv env;

  auto isolate = Isolate::GetCurrent();

  string excClassName = ObjectManager::GetClassName((jobject)m_javaException);
  if (excClassName == "com/tns/NativeScriptException") {
    jfieldID fieldID = env.GetFieldID(env.GetObjectClass(m_javaException),
                                      "jsValueAddress", "J");
    jlong addr = env.GetLongField(m_javaException, fieldID);

    Local<Value> stored;
    if (addr != 0 && TakeJsError(isolate, addr, stored)) {
      errObj = stored;
      // The id is spent; clearing it keeps a second conversion from picking up
      // a recycled entry.
      env.SetLongField(m_javaException, fieldID, 0);
    } else {
      errObj = GetJavaExceptionFromEnv(m_javaException, env);
    }
  } else {
    errObj = GetJavaExceptionFromEnv(m_javaException, env);
  }

  return errObj;
}

Local<Value> NativeScriptException::GetJavaExceptionFromEnv(
    const JniLocalRef& exc, JEnv& env) {
  auto errMsg = GetExceptionMessage(env, exc);
  auto stackTrace = GetExceptionStackTrace(env, exc);
  DEBUG_WRITE("Error during java interop errorMessage: %s\n stackTrace:\n %s",
              errMsg.c_str(), stackTrace.c_str());

  auto isolate = Isolate::GetCurrent();
  auto objectManager = Runtime::GetObjectManager(isolate);

  auto msg = ArgConverter::ConvertToV8String(isolate, errMsg);
  auto errObj = Exception::Error(msg).As<Object>();

  jint javaObjectID = objectManager->GetOrCreateObjectId((jobject)exc);
  auto nativeExceptionObject =
      objectManager->GetJsObjectByJavaObject(javaObjectID);

  if (nativeExceptionObject.IsEmpty()) {
    string className = objectManager->GetClassName((jobject)exc);
    nativeExceptionObject =
        objectManager->CreateJSWrapper(javaObjectID, className);
  }

  auto context = isolate->GetCurrentContext();
  errObj->Set(context, V8StringConstants::GetNativeException(isolate),
              nativeExceptionObject);

  string jsStackTraceMessage =
      GetErrorStackTrace(Exception::GetStackTrace(errObj));
  errObj->Set(context, V8StringConstants::GetStack(isolate),
              ArgConverter::ConvertToV8String(isolate, jsStackTraceMessage));
  errObj->Set(context, V8StringConstants::GetStackTrace(isolate),
              ArgConverter::ConvertToV8String(
                  isolate, jsStackTraceMessage + stackTrace));

  return errObj;
}

string NativeScriptException::GetFullMessage(const TryCatch& tc,
                                             const string& jsExceptionMessage) {
  auto ex = tc.Exception();

  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();

  auto message = tc.Message();

  stringstream ss;
  ss << jsExceptionMessage;

  // get script name
  auto scriptResName = message->GetScriptResourceName();

  // get stack trace
  string stackTraceMessage = GetErrorStackTrace(message->GetStackTrace());

  if (!scriptResName.IsEmpty() && scriptResName->IsString()) {
    ss << endl
       << "File: ("
       << ArgConverter::ConvertToString(scriptResName.As<String>());
  } else {
    ss << endl << "File: (<unknown>";
  }
  // An isolate that started terminating while this message was being
  // formatted answers Nothing to every query below - a missing line number
  // must degrade the report, not abort the process.
  ss << ":" << message->GetLineNumber(context).FromMaybe(0) << ":"
     << message->GetStartColumn() << ")" << endl
     << endl;
  ss << "StackTrace: " << endl << stackTraceMessage << endl;

  string loggedMessage = ss.str();

  PrintErrorMessage(loggedMessage);

  if (!tc.CanContinue()) {
    stringstream errM;
    errM << endl
         << "An uncaught error has occurred and V8's TryCatch block CAN'T be "
            "continued. ";
    loggedMessage = errM.str() + loggedMessage;
  }

  return loggedMessage;
}

JniLocalRef NativeScriptException::TryGetJavaThrowableObject(
    JEnv& env, const Local<Object>& jsObj) {
  JniLocalRef javaThrowableObject;

  auto isolate = Isolate::GetCurrent();
  auto objectManager = Runtime::GetObjectManager(isolate);

  auto javaObj = objectManager->GetJavaObjectByJsObject(jsObj);
  JniLocalRef objClass;

  if (!javaObj.IsNull()) {
    objClass = JniLocalRef(env.GetObjectClass(javaObj));
  } else {
    auto isolate = v8::Isolate::GetCurrent();
    auto context = isolate->GetCurrentContext();
    Local<Value> nativeEx;
    jsObj->Get(context, V8StringConstants::GetNativeException(isolate))
        .ToLocal(&nativeEx);
    if (!nativeEx.IsEmpty() && nativeEx->IsObject()) {
      javaObj = objectManager->GetJavaObjectByJsObject(nativeEx.As<Object>());
      objClass = JniLocalRef(env.GetObjectClass(javaObj));
    }
  }

  auto isThrowable = !objClass.IsNull()
                         ? env.IsAssignableFrom(objClass, THROWABLE_CLASS)
                         : JNI_FALSE;

  if (isThrowable == JNI_TRUE) {
    javaThrowableObject = JniLocalRef(env.NewLocalRef(javaObj));
  }

  return javaThrowableObject;
}

void NativeScriptException::PrintErrorMessage(const string& errorMessage) {
  // split the message by new lines to workaround the LogCat's maximum
  // characters in a single message
  stringstream ss(errorMessage);
  string line;
  while (getline(ss, line, '\n')) {
    // TODO: Log in the V8's Console as well?
    DEBUG_WRITE("%s", line.c_str());
  }
}

string NativeScriptException::GetErrorMessage(const Local<Message>& message,
                                              Local<Value>& error,
                                              const string& prependMessage) {
  Local<String> message_text_string = message->Get();
  auto mes = ArgConverter::ConvertToString(message_text_string);

  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();

  // get whole error message from previous stack
  stringstream ss;

  if (prependMessage != "") {
    ss << prependMessage << endl;
  }

  string errMessage;
  bool hasFullErrorMessage = false;
  auto v8FullMessage = ArgConverter::ConvertToV8String(isolate, "fullMessage");
  if (error->IsObject() &&
      error.As<Object>()->Has(context, v8FullMessage).FromMaybe(false)) {
    hasFullErrorMessage = true;
    Local<Value> errMsgVal;
    error.As<Object>()->Get(context, v8FullMessage).ToLocal(&errMsgVal);
    if (!errMsgVal.IsEmpty()) {
      errMessage = ArgConverter::ConvertToString(errMsgVal.As<String>());
    } else {
      errMessage = "";
    }
    ss << errMessage;
  }

  auto str = error->ToDetailString(context);
  if (!str.IsEmpty()) {
    String::Utf8Value utfError(isolate, str.FromMaybe(Local<String>()));
    if (hasFullErrorMessage) {
      ss << endl;
    }
    ss << *utfError;
  }

  return ss.str();
}

string NativeScriptException::GetErrorStackTrace(
    const Local<StackTrace>& stackTrace) {
  stringstream ss;

  auto isolate = Isolate::GetCurrent();
  HandleScope handleScope(isolate);

  int frameCount = stackTrace->GetFrameCount();

  for (int i = 0; i < frameCount; i++) {
    auto frame = stackTrace->GetFrame(isolate, i);
    auto funcName = ArgConverter::ConvertToString(frame->GetFunctionName());
    auto srcName = ArgConverter::ConvertToString(frame->GetScriptName());
    auto lineNumber = frame->GetLineNumber();
    auto column = frame->GetColumn();

    auto startString = i == 0 ? "" : "\t";

    ss << startString << (i > 0 ? "at " : "") << funcName.c_str() << "("
       << srcName.c_str() << ":" << lineNumber << ":" << column << ")" << endl;
  }

  return ss.str();
}

string NativeScriptException::GetExceptionMessage(JEnv& env,
                                                  jthrowable exception) const {
  string errMsg;
  JniLocalRef msg(env.CallStaticObjectMethod(
      NATIVESCRIPTEXCEPTION_CLASS, NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID,
      exception));

  const char* msgStr = env.GetStringUTFChars(msg, nullptr);

  errMsg.append(msgStr);

  env.ReleaseStringUTFChars(msg, msgStr);

  return errMsg;
}

string NativeScriptException::GetExceptionStackTrace(
    JEnv& env, jthrowable exception) const {
  string errStackTrace;
  JniLocalRef msg(env.CallStaticObjectMethod(
      NATIVESCRIPTEXCEPTION_CLASS,
      NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID, exception));

  const char* msgStr = env.GetStringUTFChars(msg, nullptr);

  errStackTrace.append(msgStr);

  env.ReleaseStringUTFChars(msg, msgStr);

  return errStackTrace;
}

jclass NativeScriptException::RUNTIME_CLASS = nullptr;
jclass NativeScriptException::THROWABLE_CLASS = nullptr;
jclass NativeScriptException::NATIVESCRIPTEXCEPTION_CLASS = nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID =
    nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID =
    nullptr;
jmethodID NativeScriptException::NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID =
    nullptr;
jmethodID NativeScriptException::
    NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID = nullptr;