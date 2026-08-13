// napi_threadsafe_function over the runtime's EventLoop.
//
// The invariant the whole file is built around: a producer thread never enters
// the target isolate. It takes this object's mutex, appends to the queue and
// leaves; every step that touches JS runs in an entry posted to the env's own
// event loop. Taking the isolate's Locker from a foreign thread would sidestep
// the loop's ordering and deadlock against multithreaded-JS entry paths.

// Must precede every include: without NAPI_EXPERIMENTAL, NAPI_VERSION defaults
// to 8 and the version-gated declarations in node_api.h stay invisible, so the
// definitions below would silently not match anything.
#define NAPI_EXPERIMENTAL
#define NODE_API_EXPERIMENTAL_NO_WARNING

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "node_api.h"

#include "js_native_api_v8.h"

#include "EventLoop.h"
#include "NapiEnv.h"
#include "NapiThreadSafeFunction.h"

struct napi_threadsafe_function__
    : public std::enable_shared_from_this<napi_threadsafe_function__> {
  // Read from producer threads, so they are fixed at construction and only
  // cleared once the function is closed for good. The home thread id is used
  // for identity checks only: a producer may compare against it after the
  // owning thread exits, and comparing an id is fine where entering the
  // isolate would not be. Work is posted through the event loop, whose
  // weak_ptr goes null once the runtime shuts it down.
  tns::NapiEnv* env = nullptr;
  v8::Isolate* isolate = nullptr;
  std::thread::id homeThread;
  std::weak_ptr<tns::EventLoop> eventLoop;

  // Touched on the env's thread only, which is where the abort path and every
  // dispatch run.
  napi_ref callbackRef = nullptr;
  napi_threadsafe_function_call_js callJs = nullptr;
  napi_finalize finalizeCb = nullptr;
  void* finalizeData = nullptr;
  void* context = nullptr;

  std::mutex mutex;
  std::condition_variable spaceAvailable;
  std::queue<void*> queue;
  size_t maxQueueSize = 0;
  size_t threadCount = 0;
  bool closing = false;
  bool envAlive = true;
  bool dispatchPosted = false;
  bool finalized = false;
  bool refed = true;
};

namespace {

using TsfnRef = std::shared_ptr<napi_threadsafe_function__>;

// Owns the reference that stands in for the handle the addon holds: a
// threadsafe function outlives its JS side (entries in flight keep their own
// reference) and its JS side outlives the handle, so neither can own it alone.
struct TsfnRegistry {
  std::mutex mutex;
  std::unordered_map<napi_threadsafe_function__*, TsfnRef> live;
};

TsfnRegistry& Registry() {
  static TsfnRegistry* registry = new TsfnRegistry();
  return *registry;
}

void DropHandleRef(napi_threadsafe_function__* tsfn) {
  // Declared before the lock so it outlives it: dropping the last reference
  // runs the destructor, which must not happen under the registry's mutex.
  TsfnRef dropped;

  TsfnRegistry& registry = Registry();
  std::lock_guard<std::mutex> lock(registry.mutex);
  auto it = registry.live.find(tsfn);
  if (it == registry.live.end()) {
    return;
  }

  dropped = std::move(it->second);
  registry.live.erase(it);
}

// Node deletes the function as soon as it is finalized, which leaves the
// handles of threads that have not released yet dangling. Holding on until
// every thread has released costs one map entry and makes a late
// napi_release_threadsafe_function safe.
void DropHandleRefIfDone(const TsfnRef& tsfn) {
  {
    std::lock_guard<std::mutex> lock(tsfn->mutex);
    if (!tsfn->finalized || tsfn->threadCount > 0) {
      return;
    }
  }

  DropHandleRef(tsfn.get());
}

// The env's thread, with the isolate locked and a handle scope open.
void FinalizeOnJsThread(const TsfnRef& tsfn) {
  std::queue<void*> undelivered;
  {
    std::lock_guard<std::mutex> lock(tsfn->mutex);
    if (tsfn->finalized) {
      return;
    }
    tsfn->finalized = true;
    undelivered.swap(tsfn->queue);
  }

  if (tsfn->callbackRef != nullptr) {
    napi_delete_reference(tsfn->env, tsfn->callbackRef);
    tsfn->callbackRef = nullptr;
  }

  // Node hands undelivered items back with a null env so the producer's data
  // can still be freed; there is no JS left to run for them. This must happen
  // before the finalize callback, which is where addons free the context these
  // calls receive.
  while (!undelivered.empty()) {
    if (tsfn->callJs != nullptr) {
      tsfn->callJs(nullptr, nullptr, tsfn->context, undelivered.front());
    }
    undelivered.pop();
  }

  if (tsfn->finalizeCb != nullptr) {
    tsfn->env->CallFinalizer(tsfn->finalizeCb, tsfn->finalizeData,
                             tsfn->context);
  }
}

void CallJsOnJsThread(const TsfnRef& tsfn, void* data) {
  napi_env env = tsfn->env;

  v8::HandleScope handle_scope(env->isolate);
  v8::Context::Scope context_scope(env->context());

  napi_value callback = nullptr;
  if (tsfn->callbackRef != nullptr) {
    napi_get_reference_value(env, tsfn->callbackRef, &callback);
  }

  env->CallIntoModule(
      [&](napi_env moduleEnv) {
        if (tsfn->callJs != nullptr) {
          tsfn->callJs(moduleEnv, callback, tsfn->context, data);
          return;
        }
        if (callback != nullptr) {
          napi_value recv = nullptr;
          napi_get_undefined(moduleEnv, &recv);
          napi_call_function(moduleEnv, recv, callback, 0, nullptr, nullptr);
        }
      },
      tns::NapiReportModuleException);
}

void PostDispatch(const TsfnRef& tsfn);

// A producer that pushes faster than the callback returns would otherwise keep
// one loop entry busy forever, starving timers, messages and the UI. Node
// re-arms its async handle per item; this yields every so many.
constexpr size_t kMaxCallsPerDispatch = 64;

void RunDispatch(const TsfnRef& tsfn) {
  size_t delivered = 0;

  for (;;) {
    void* data = nullptr;
    bool haveData = false;
    bool finalize = false;
    {
      std::lock_guard<std::mutex> lock(tsfn->mutex);
      if (!tsfn->envAlive) {
        tsfn->dispatchPosted = false;
        return;
      }

      if (tsfn->closing) {
        finalize = true;
      } else if (!tsfn->queue.empty()) {
        data = tsfn->queue.front();
        tsfn->queue.pop();
        haveData = true;
      } else if (tsfn->threadCount == 0) {
        tsfn->closing = true;
        finalize = true;
      } else {
        tsfn->dispatchPosted = false;
        return;
      }
    }

    tsfn->spaceAvailable.notify_all();

    if (finalize) {
      FinalizeOnJsThread(tsfn);
      {
        std::lock_guard<std::mutex> lock(tsfn->mutex);
        tsfn->dispatchPosted = false;
      }
      DropHandleRefIfDone(tsfn);
      return;
    }

    if (haveData) {
      CallJsOnJsThread(tsfn, data);

      if (++delivered >= kMaxCallsPerDispatch) {
        {
          std::lock_guard<std::mutex> lock(tsfn->mutex);
          tsfn->dispatchPosted = false;
        }
        PostDispatch(tsfn);
        return;
      }
    }
  }
}

void PostDispatch(const TsfnRef& tsfn) {
  std::weak_ptr<tns::EventLoop> weakLoop;
  {
    // Read under the mutex: this runs on producer threads, and teardown
    // closes the function as soon as the env starts tearing down.
    std::lock_guard<std::mutex> lock(tsfn->mutex);
    if (tsfn->dispatchPosted || !tsfn->envAlive) {
      return;
    }
    tsfn->dispatchPosted = true;
    weakLoop = tsfn->eventLoop;
  }

  // The entry owns a reference: the handle may be released, and the queue
  // drained by an earlier dispatch, before this one runs. It runs under the
  // loop's Locker/scopes, and EventLoop::Shutdown drops queued entries before
  // the env dies, so no liveness re-check is needed inside.
  //
  // A post that lands between IsStopped and Shutdown is silently dropped; the
  // stuck dispatchPosted flag is harmless because the abort path that follows
  // Shutdown never consults it.
  TsfnRef ref = tsfn;

  std::shared_ptr<tns::EventLoop> loop = weakLoop.lock();
  if (loop == nullptr || loop->IsStopped()) {
    std::lock_guard<std::mutex> lock(ref->mutex);
    ref->dispatchPosted = false;
    return;
  }

  loop->PostInternal([ref]() { RunDispatch(ref); });
}

}  // namespace

namespace tns {

void NapiAbortThreadSafeFunctions(NapiEnv* env) {
  std::vector<TsfnRef> victims;
  {
    TsfnRegistry& registry = Registry();
    std::lock_guard<std::mutex> lock(registry.mutex);
    for (auto& entry : registry.live) {
      if (entry.second->env == env) {
        victims.push_back(entry.second);
      }
    }
  }

  for (const TsfnRef& tsfn : victims) {
    {
      std::lock_guard<std::mutex> lock(tsfn->mutex);
      tsfn->closing = true;
      tsfn->envAlive = false;
    }
    // Producers blocked on a full queue have to be let go before anything
    // else: they are answered with napi_closing.
    tsfn->spaceAvailable.notify_all();

    v8::Isolate::Scope isolate_scope(env->isolate);
    v8::HandleScope handle_scope(env->isolate);
    v8::Context::Scope context_scope(env->context());
    FinalizeOnJsThread(tsfn);

    // The home thread id stays comparable forever; only `envAlive` gates
    // posting work.
    {
      std::lock_guard<std::mutex> lock(tsfn->mutex);
      tsfn->env = nullptr;
      tsfn->isolate = nullptr;
    }

    DropHandleRefIfDone(tsfn);
  }
}

}  // namespace tns

//=== Entry points =========================================================

napi_status NAPI_CDECL
napi_create_threadsafe_function(napi_env env,
                                napi_value func,
                                napi_value async_resource,
                                napi_value async_resource_name,
                                size_t max_queue_size,
                                size_t initial_thread_count,
                                void* thread_finalize_data,
                                napi_finalize thread_finalize_cb,
                                void* context,
                                napi_threadsafe_function_call_js call_js_cb,
                                napi_threadsafe_function* result) {
  CHECK_ENV(env);
  CHECK_ARG(env, async_resource_name);
  RETURN_STATUS_IF_FALSE(env, initial_thread_count > 0, napi_invalid_arg);
  CHECK_ARG(env, result);

  // There is no async_hooks here, so the resource object carries no behaviour.
  (void)async_resource;

  if (func == nullptr) {
    CHECK_ARG(env, call_js_cb);
  }

  tns::NapiEnv* tnsEnv = static_cast<tns::NapiEnv*>(env);
  RETURN_STATUS_IF_FALSE(env, tnsEnv->GetEventLoop() != nullptr,
                         napi_generic_failure);

  TsfnRef tsfn = std::make_shared<napi_threadsafe_function__>();
  tsfn->env = tnsEnv;
  tsfn->isolate = env->isolate;
  tsfn->homeThread = tnsEnv->HomeThread();
  tsfn->eventLoop = tnsEnv->GetEventLoop();
  tsfn->callJs = call_js_cb;
  tsfn->finalizeCb = thread_finalize_cb;
  tsfn->finalizeData = thread_finalize_data;
  tsfn->context = context;
  tsfn->maxQueueSize = max_queue_size;
  tsfn->threadCount = initial_thread_count;

  if (func != nullptr) {
    napi_status status =
        napi_create_reference(env, func, 1, &tsfn->callbackRef);
    if (status != napi_ok) {
      return napi_set_last_error(env, status);
    }
  }

  {
    TsfnRegistry& registry = Registry();
    std::lock_guard<std::mutex> lock(registry.mutex);
    registry.live[tsfn.get()] = tsfn;
  }

  *result = tsfn.get();
  return napi_clear_last_error(env);
}

napi_status NAPI_CDECL napi_get_threadsafe_function_context(
    napi_threadsafe_function func, void** result) {
  if (func == nullptr || result == nullptr) {
    return napi_invalid_arg;
  }

  *result = func->context;
  return napi_ok;
}

napi_status NAPI_CDECL
napi_call_threadsafe_function(napi_threadsafe_function func,
                              void* data,
                              napi_threadsafe_function_call_mode is_blocking) {
  if (func == nullptr) {
    return napi_invalid_arg;
  }

  {
    std::unique_lock<std::mutex> lock(func->mutex);
    while (!func->closing && func->maxQueueSize > 0 &&
           func->queue.size() >= func->maxQueueSize) {
      if (is_blocking == napi_tsfn_nonblocking) {
        return napi_queue_full;
      }
      // The thread that drains the queue is the one that would have to wake
      // this wait, so blocking on it there can only stall forever. Node blocks
      // regardless and leaves this status unused; wedging the event loop is
      // worse than a status an addon may not expect.
      if (std::this_thread::get_id() == func->homeThread) {
        return napi_would_deadlock;
      }
      func->spaceAvailable.wait(lock);
    }

    if (func->closing) {
      return napi_closing;
    }

    func->queue.push(data);
  }

  PostDispatch(func->shared_from_this());
  return napi_ok;
}

napi_status NAPI_CDECL
napi_acquire_threadsafe_function(napi_threadsafe_function func) {
  if (func == nullptr) {
    return napi_invalid_arg;
  }

  std::lock_guard<std::mutex> lock(func->mutex);
  if (func->closing) {
    return napi_closing;
  }

  func->threadCount++;
  return napi_ok;
}

napi_status NAPI_CDECL napi_release_threadsafe_function(
    napi_threadsafe_function func, napi_threadsafe_function_release_mode mode) {
  if (func == nullptr) {
    return napi_invalid_arg;
  }

  TsfnRef tsfn = func->shared_from_this();
  bool dispatch = false;
  {
    std::lock_guard<std::mutex> lock(tsfn->mutex);
    if (tsfn->threadCount == 0) {
      return napi_invalid_arg;
    }

    tsfn->threadCount--;
    if (!tsfn->closing &&
        (mode == napi_tsfn_abort || tsfn->threadCount == 0)) {
      tsfn->closing = (mode == napi_tsfn_abort);
      dispatch = true;
    }
  }

  // A producer waiting for queue space has to see the close, and the JS side
  // has to notice that it can finalize.
  tsfn->spaceAvailable.notify_all();
  if (dispatch) {
    PostDispatch(tsfn);
  }
  DropHandleRefIfDone(tsfn);

  return napi_ok;
}

// The looper this runtime drives has no libuv-style reference count: it
// belongs to the app (or the worker) and never exits because a Node-API addon
// asked it to. The flag is tracked so napi_ref/unref pair up, and gates
// nothing.

napi_status NAPI_CDECL napi_unref_threadsafe_function(
    node_api_basic_env env, napi_threadsafe_function func) {
  CHECK_ENV(env);
  CHECK_ARG(env, func);

  std::lock_guard<std::mutex> lock(func->mutex);
  func->refed = false;

  return napi_clear_last_error(env);
}

napi_status NAPI_CDECL napi_ref_threadsafe_function(
    node_api_basic_env env, napi_threadsafe_function func) {
  CHECK_ENV(env);
  CHECK_ARG(env, func);

  std::lock_guard<std::mutex> lock(func->mutex);
  func->refed = true;

  return napi_clear_last_error(env);
}
