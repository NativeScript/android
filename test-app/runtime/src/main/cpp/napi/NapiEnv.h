#ifndef NAPIENV_H_
#define NAPIENV_H_

// Pins NAPI_VERSION for every consumer of this header, so napi_env__ is seen
// identically wherever it is compiled. Must precede the napi includes below.
#ifndef NAPI_EXPERIMENTAL
#define NAPI_EXPERIMENTAL
#endif
#ifndef NODE_API_EXPERIMENTAL_NO_WARNING
#define NODE_API_EXPERIMENTAL_NO_WARNING
#endif

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "js_native_api_v8.h"

namespace tns {

class EventLoop;

// The finalizer of one external buffer/arraybuffer. Its callback must run
// exactly once, on the env's thread, while the env is alive — but V8's
// backing-store deleter fires on arbitrary threads, including during isolate
// disposal after the env died. So the deleter only *posts* the callback, the
// env's teardown sweep runs whatever has not run yet, and `claimed` (flipped
// exclusively on the env's thread) arbitrates between the two.
struct NapiExternalFinalizer {
  std::atomic<bool> claimed{false};
  napi_finalize cb = nullptr;
  void* data = nullptr;
  void* hint = nullptr;
};

// The napi_env behind every Node-API call, one per runtime isolate/context.
// Node's equivalent (node_napi_env__) lives in node_api.cc, which is not
// vendored; this is its replacement.
class NapiEnv : public napi_env__ {
 public:
  // Creates the env for `context` on the runtime's home thread and hands
  // ownership to the caller, which must eventually pass it to Destroy while
  // the isolate is alive and locked.
  static NapiEnv* Create(v8::Local<v8::Context> context,
                         const std::shared_ptr<EventLoop>& eventLoop);
  static void Destroy(NapiEnv* env);

  // Null when the isolate has no runtime, or before PrepareV8Runtime reaches
  // the env, or after teardown.
  static NapiEnv* ForIsolate(v8::Isolate* isolate);

  bool can_call_into_js() const override { return !tearingDown_; }
  void CallFinalizer(napi_finalize cb, void* data, void* hint) override;
  void EnqueueFinalizer(v8impl::RefTracker* finalizer) override;
  void DeleteMe() override;

  v8::Local<v8::Private> PrivateKey(NapiPrivateKeySlot slot);

  // The thread that owns this env — identity checks only
  // (is-this-the-env's-thread); work is posted through GetEventLoop().
  std::thread::id HomeThread() const { return homeThread_; }

  // The runtime's event loop, or null once the runtime released it. Between
  // Shutdown and that release it is still returned, and posts to it are
  // silently dropped. Posted internal-lane entries run on the env's thread
  // under the loop's Locker/scopes and end with a microtask checkpoint.
  std::shared_ptr<EventLoop> GetEventLoop() const { return eventLoop_.lock(); }

  // Exports of an addon already initialized in this env, or an empty handle.
  v8::MaybeLocal<v8::Object> CachedModuleExports(const std::string& name);
  void CacheModuleExports(const std::string& name,
                          v8::Local<v8::Object> exports);

  // External-buffer finalizer registry, env thread only. Registered entries
  // are claimed+run either by a posted backing-store deleter or by the
  // teardown sweep in DeleteMe, whichever gets there first.
  void RegisterExternalFinalizer(
      const std::shared_ptr<NapiExternalFinalizer>& finalizer);
  void RunExternalFinalizer(
      const std::shared_ptr<NapiExternalFinalizer>& finalizer);

 private:
  NapiEnv(v8::Local<v8::Context> context,
          const std::shared_ptr<EventLoop>& eventLoop);
  ~NapiEnv() override;

  void DrainFinalizers();

  std::thread::id homeThread_;
  std::weak_ptr<EventLoop> eventLoop_;
  bool tearingDown_ = false;
  v8::Eternal<v8::Private> privateKeys_[2];
  std::unordered_map<std::string, v8::Global<v8::Object>> moduleExports_;
  std::unordered_set<std::shared_ptr<NapiExternalFinalizer>>
      externalFinalizers_;
};

// Runs the env's cleanup hooks, most recently added first, at the head of
// teardown. Defined in NodeApiEmbed.cpp, next to the hook registry.
void NapiRunEnvCleanupHooks(NapiEnv* env);

// The shared exception tail for every Node-API entry into JS (finalizers,
// threadsafe-function callbacks, async-work completions): routes the pending
// exception through the runtime's 9.1 containment pipeline (`error` event ->
// legacy hook -> log), and rethrows to Java when policy or a JS-initiated
// chain demands propagation. Runs on the env's thread with the isolate locked
// and a context entered.
void NapiReportModuleException(napi_env env, v8::Local<v8::Value> exception);

}  // namespace tns

#endif /* NAPIENV_H_ */
