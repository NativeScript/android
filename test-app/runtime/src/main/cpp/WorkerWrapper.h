#ifndef WORKERWRAPPER_H_
#define WORKERWRAPPER_H_

#include <jni.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "ConcurrentQueue.h"
#include "WorkerMessage.h"
#include "v8.h"

namespace tns {

class LooperTasks;
class Runtime;

/*
 * Owns a worker's native thread and its lifecycle, mirroring the iOS
 * runtime's WorkerWrapper. The thread is a std::thread that attaches to the
 * JVM, prepares a Java Looper (so worker/plugin code can keep using Android
 * Handlers) and then drives that single looper, which pumps both Java
 * messages and the worker's C++ inbox (ConcurrentQueue + eventfd).
 *
 * Messaging is done entirely in C++ with V8 ValueSerializer payloads:
 *  - parent -> worker: queue_ + eventfd wakeup on the worker looper
 *  - worker -> parent: the parent runtime's LooperTasks queue
 *
 * The parent may be the main thread or another worker (nested workers); a
 * worker's children are terminated when the worker itself shuts down.
 */
class WorkerWrapper : public std::enable_shared_from_this<WorkerWrapper> {
public:
    WorkerWrapper(v8::Isolate* parentIsolate, int workerId, std::string workerPath,
                  std::string callingDir, int priority,
                  v8::Local<v8::Object> workerObject);

    int WorkerId() const { return workerId_; }
    bool IsTerminating() const { return isTerminating_; }
    bool IsClosing() const { return isClosing_; }
    bool IsDisposed() const { return isDisposed_; }

    /*
     * Spawns the (detached) worker thread. The thread holds a shared_ptr to
     * this wrapper, keeping it alive until the thread fully shuts down.
     */
    void Start();

    /*
     * parent -> worker. Queues a serialized message and wakes the worker
     * looper. Messages posted before the worker finishes bootstrapping are
     * drained right after the worker script runs.
     */
    void PostMessage(std::shared_ptr<worker::Message> message);

    /*
     * worker -> parent. Posts a task that fires the Worker object's
     * `onmessage` on the parent runtime's thread.
     */
    void PostMessageToParent(std::shared_ptr<worker::Message> message);

    /*
     * Called on the parent's thread. Interrupts running JS (TerminateExecution
     * is the one v8 call that is legal cross-thread) and quits the worker
     * looper.
     */
    void Terminate();

    /*
     * Called on the worker thread (self.close()). Lets the current callback
     * unwind, then the looper quits and the thread shuts down gracefully.
     */
    void Close();

    /*
     * Posts the worker object's `onerror` invocation to the parent's thread.
     * Strings only - must not hold any v8 handles from the worker isolate.
     */
    void PassUncaughtExceptionFromWorkerToParent(const std::string& message,
                                                 const std::string& filename,
                                                 const std::string& stackTrace,
                                                 int lineno);

    /*
     * Registry of live workers, keyed by workerId. Replaces the old
     * CallbackHandlers::id2WorkerMap. Guarded by a mutex because the worker
     * shutdown path posts cleanup from the worker thread.
     */
    static int NextWorkerId();
    static std::shared_ptr<WorkerWrapper> GetById(int workerId);
    static void Insert(int workerId, std::shared_ptr<WorkerWrapper> wrapper);

    /*
     * Parent thread only: resets the Worker object's persistent handle and
     * removes the wrapper from the registry. Idempotent.
     */
    static void ClearWorkerOnParent(int workerId);

    /*
     * Terminates and clears all workers whose parent is the given isolate.
     * Must run on the parent's thread, before the parent isolate is disposed
     * (the children's Worker object persistents live in that isolate).
     * Cascades: each child terminates its own children during shutdown.
     */
    static void TerminateChildren(v8::Isolate* parentIsolate);

    /*
     * Resolves the wrapper of a worker isolate via its isolate data slot.
     * Returns nullptr on the main isolate.
     */
    static WorkerWrapper* FromIsolate(v8::Isolate* isolate);

    /*
     * Resolves the jclass/jmethodID handles used by the worker thread
     * bootstrap, before the first worker starts. The first call is always on
     * the main thread (nested workers require a main-thread worker first),
     * where class loading is safe.
     */
    static void EnsureJniCached();

private:
    void BackgroundLooper(std::shared_ptr<WorkerWrapper> self);
    void DrainPendingTasks();
    void QuitLooper();
    static int DrainCallback(int fd, int events, void* data);
    static void FireMessageOnParentWorkerObject(int workerId,
                                              std::shared_ptr<worker::Message> message);
    static void FireErrorOnParentWorkerObject(int workerId, const std::string& message,
                                            const std::string& stackTrace,
                                            const std::string& filename, int lineno,
                                            const std::string& threadName);

    v8::Isolate* parentIsolate_;
    // The parent runtime's task queue; weak so a child outliving its parent
    // just drops its posts instead of touching a dead runtime.
    std::weak_ptr<LooperTasks> parentTasks_;
    std::atomic<v8::Isolate*> workerIsolate_;
    Runtime* runtime_;

    const int workerId_;
    const std::string workerPath_;
    const std::string callingDir_;
    const std::string threadName_;
    const int priority_;

    v8::Persistent<v8::Object>* poWorker_;

    std::atomic_bool isClosing_;
    std::atomic_bool isTerminating_;
    std::atomic_bool isDisposed_;

    ConcurrentQueue queue_;

    std::mutex looperMutex_;
    jobject javaLooperRef_;

    static std::mutex registryMutex_;
    static std::map<int, std::shared_ptr<WorkerWrapper>> registry_;
    static std::atomic_int nextWorkerId_;

    static jclass RUNTIME_CLASS;
    static jclass LOOPER_CLASS;
    static jclass PROCESS_CLASS;
    static jmethodID INIT_WORKER_RUNTIME_METHOD_ID;
    static jmethodID RUN_WORKER_LOOP_METHOD_ID;
    static jmethodID DETACH_WORKER_RUNTIME_METHOD_ID;
    static jmethodID MY_LOOPER_METHOD_ID;
    static jmethodID LOOPER_QUIT_METHOD_ID;
    static jmethodID SET_THREAD_PRIORITY_METHOD_ID;
};

}  // namespace tns

#endif /* WORKERWRAPPER_H_ */
