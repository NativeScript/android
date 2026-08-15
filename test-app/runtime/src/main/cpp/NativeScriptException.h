#ifndef NATIVESCRIPTEXCEPTION_H_
#define NATIVESCRIPTEXCEPTION_H_

#include <exception>
#include <memory>
#include <vector>

#include "JEnv.h"
#include "JniLocalRef.h"
#include "ObjectManager.h"
#include "include/v8.h"
#include "v8.h"

namespace tns {
class Runtime;

class NativeScriptException : public std::exception {
 public:
  /*
   * Generates a NativeScriptException with java error from environment
   */
  NativeScriptException(JEnv& env);

  /*
   * Generates a NativeScriptException with given message
   */
  NativeScriptException(const std::string& message);

  /*
   * Generates a NativeScriptException with given message and stackTrace
   */
  NativeScriptException(const std::string& message,
                        const std::string& stackTrace);

  /*
   * Generates a NativeScriptException with javascript error from TryCatch and a
   * prepend message if any
   */
  NativeScriptException(v8::TryCatch& tc, const std::string& message = "");

  void ReThrowToV8();
  void ReThrowToJava();

  /*
   * Drops the handle to the thrown JS value, keeping the message and stack
   * strings. For an exception that has to outlive the isolate it was thrown in
   * -- a failed runtime initialization, where the isolate is disposed while the
   * exception is still in flight.
   */
  void ReleaseJsHandle() {
    m_javascriptException.reset();
  }

  std::string ToString() const;
  std::string GetErrorMessage() const;
  const char* what() const noexcept override;

  static void Init();

  /*
   * This handler is attached to v8 to handle uncaught javascript exceptions.
   */
  static void OnUncaughtError(v8::Local<v8::Message> message,
                              v8::Local<v8::Value> error);

  /*
   * Isolate-level promise rejection callback (SetPromiseRejectCallback).
   * Feeds the per-isolate PromiseRejectionTracker owned by Runtime.
   */
  static void OnPromiseRejected(v8::PromiseRejectMessage message);

  /*
   * Reports an unhandled promise rejection on a non-worker isolate:
   * dispatches `unhandledrejection` and, when unprevented, falls through to
   * ReportFatalTail with the "Unhandled promise rejection:" prefix.
   */
  static void ReportUnhandledRejection(v8::Isolate* isolate,
                                       v8::Local<v8::Promise> promise,
                                       v8::Local<v8::Value> reason,
                                       const std::string& stackTrace);

  /*
   * The terminal tail shared by reportError, listener-thrown errors, the
   * rejection path and contained callback exceptions: calls the
   * __onUncaughtError shim (or __onDiscardedError under the deprecated
   * discardUncaughtJsExceptions flag) and logs. Does NOT dispatch any event
   * (the caller already has), preventing recursion, and never crashes the
   * process itself.
   */
  static void ReportFatalTail(v8::Isolate* isolate, v8::Local<v8::Value> error,
                              const std::string& stackOverride = "",
                              const std::string& logPrefix = "",
                              jboolean isDiscarded = JNI_FALSE);

  /*
   * Policy-aware handling of an uncaught JS exception at a native->JS
   * callback boundary (overridden method invoked by native code, timer,
   * __runOnMainThread / frame callback). Returns true when the error was
   * CONTAINED - reported through the WHATWG pipeline (`error` event ->
   * legacy hook -> log) with the TryCatch reset, and the caller should
   * proceed with a default value. Returns false when the caller must
   * propagate the exception to Java: the chain is JS-initiated (a JS frame
   * is waiting for it below the boundary), the throw is a branded
   * interop.escapeException (explicit forward), or uncaughtErrorPolicy is
   * "throw".
   */
  static bool ContainUncaughtCallbackException(v8::Isolate* isolate,
                                               v8::TryCatch& tc);

  /*
   * Hands an unhandled promise rejection to the native layer under
   * uncaughtErrorPolicy: "throw": schedules a com.tns.NativeScriptException
   * throw from a clean Java frame on this thread's looper, so the thread's
   * uncaught-exception handler (and its reporting) runs exactly as for a
   * sync uncaught error.
   */
  static void ThrowUncaughtJsErrorToJava(const std::string& message,
                                         const std::string& stackTrace);

  /*
   * Calls the global "__onUncaughtError" or "__onDiscardedError" if such is
   * provided
   */
  static void CallJsFuncWithErr(v8::Local<v8::Value> errObj,
                                jboolean isDiscarded);

  /*
   * Generates string stack trace from js StackTrace
   */
  static std::string GetErrorStackTrace(
      const v8::Local<v8::StackTrace>& stackTrace);

 private:
  /*
   * Try to get native exception or NativeScriptException from js object
   */
  JniLocalRef TryGetJavaThrowableObject(JEnv& env,
                                        const v8::Local<v8::Object>& jsObj);

  /*
   * Gets java exception message from jthrowable
   */
  std::string GetExceptionMessage(JEnv& env, jthrowable exception) const;

  /*
   * Gets java exception stack trace from jthrowable
   */
  std::string GetExceptionStackTrace(JEnv& env, jthrowable exception) const;

  /*
   * Gets the member m_javaException, wraps it and creates a javascript error
   * object from it
   */
  v8::Local<v8::Value> WrapJavaToJsException();

  /*
   * Gets all the information from a java exception and puts it in a javascript
   * errror object
   */
  v8::Local<v8::Value> GetJavaExceptionFromEnv(const JniLocalRef& exc,
                                               JEnv& env);

  /*
   * Gets all the information from a js message and an js error object and puts
   * it in a string
   */
  static std::string GetErrorMessage(const v8::Local<v8::Message>& message,
                                     v8::Local<v8::Value>& error,
                                     const std::string& prependMessage = "");

  /*
   *	Adds a prepend message to the normal message process
   */
  std::string GetFullMessage(const v8::TryCatch& tc,
                             const std::string& jsExceptionMessage);

  /*
   * The thrown JS value, held strongly for as long as this exception lives.
   * Shared rather than owned outright because an exception object may be
   * copied while in flight; the last copy releases the handle.
   */
  std::shared_ptr<v8::Persistent<v8::Value>> m_javascriptException;
  JniLocalRef m_javaException;
  std::string m_message;
  std::string m_stackTrace;
  std::string m_fullMessage;
  mutable std::string m_whatCache;

  static jclass RUNTIME_CLASS;
  static jclass THROWABLE_CLASS;
  static jclass NATIVESCRIPTEXCEPTION_CLASS;
  static jmethodID NATIVESCRIPTEXCEPTION_JSVALUE_CTOR_ID;
  static jmethodID NATIVESCRIPTEXCEPTION_THROWABLE_CTOR_ID;
  static jmethodID NATIVESCRIPTEXCEPTION_GET_MESSAGE_METHOD_ID;
  static jmethodID NATIVESCRIPTEXCEPTION_GET_STACK_TRACE_AS_STRING_METHOD_ID;

  static void PrintErrorMessage(const std::string& errorMessage);
};

/*
 * Per-isolate tracker for unhandled promise rejections (ported from the iOS
 * runtime's PromiseRejectionTracker). All members are touched only while the
 * v8::Locker for the runtime's isolate is held: OnReject/OnHandlerAdded run
 * inside V8 callbacks and Drain runs in an event-loop task that acquires the
 * lock, so no extra synchronization is required.
 */
class PromiseRejectionTracker {
 public:
  explicit PromiseRejectionTracker(Runtime* runtime) : runtime_(runtime) {}

  void OnReject(v8::Local<v8::Promise> promise, v8::Local<v8::Value> reason);
  void OnHandlerAdded(v8::Local<v8::Promise> promise);
  /*
   * Reports every still-unhandled rejection and fires queued
   * `rejectionhandled` events. Runs on the runtime's own looper turn, after
   * the microtask checkpoint of the turn that produced the rejection, so a
   * handler attached in the same turn always cancels the report.
   */
  void Drain();

 private:
  /*
   * Posts a Drain task to the owning runtime's event loop (internal lane) (at most one
   * outstanding). Tasks posted during runtime teardown are dropped by
   * the event loop itself.
   */
  void ScheduleDrain();
  /* Drop weak handles the GC has already cleared. */
  void PruneReportedOutstanding();

  /* A promise that was rejected without a handler. */
  struct PendingRejection {
    v8::Global<v8::Promise> promise;
    v8::Global<v8::Value> reason;
  };

  /*
   * A rejection that was already reported (`unhandledrejection` fired,
   * prevented or not). The original reason is retained so a late
   * `rejectionhandled` event carries it per spec.
   */
  struct ReportedRejection {
    v8::Global<v8::Promise> promise;
    v8::Global<v8::Value> reason;
  };

  Runtime* runtime_;
  std::vector<PendingRejection> pending_;
  /*
   * Reported rejections still outstanding. The promise handle is weak
   * (SetWeak) so a GC'd promise drops the whole entry; a handler added later
   * moves the entry into pendingRejectionHandled_.
   */
  std::vector<ReportedRejection> reportedOutstanding_;
  /*
   * `rejectionhandled` events queued by OnHandlerAdded (which runs during a
   * microtask checkpoint) to fire as a task on the next drain, per spec. Held
   * strong so the promise survives until the event fires.
   */
  std::vector<ReportedRejection> pendingRejectionHandled_;
  bool drainScheduled_ = false;
  bool draining_ = false;
};

}  // namespace tns

#endif /* NATIVESCRIPTEXCEPTION_H_ */
