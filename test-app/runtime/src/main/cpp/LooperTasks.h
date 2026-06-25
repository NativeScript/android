#ifndef LOOPERTASKS_H_
#define LOOPERTASKS_H_

#include <android/looper.h>
#include <functional>
#include <mutex>
#include <queue>

namespace tns {

/*
 * A task queue bound to one runtime's looper - the Android analogue of the
 * iOS runtime's ExecuteOnRunLoop(runtime->RuntimeLoop(), ...).
 * Each Runtime (main or worker) owns one; child workers post their outbound
 * messages, errors and cleanup notifications to their parent runtime's queue.
 *
 * Post() may be called from any thread; tasks posted after Terminate() are
 * dropped. Initialize()/Terminate() must run on the looper's own thread.
 * Held via shared_ptr by the owning Runtime and via weak_ptr by child
 * WorkerWrappers, so a child posting to an already-destroyed parent is safe.
 */
class LooperTasks {
public:
    void Initialize(ALooper* looper);
    void Post(std::function<void()> task);
    void Terminate();

private:
    static int TasksReadyCallback(int fd, int events, void* data);
    void Drain();

    std::mutex mutex_;
    std::queue<std::function<void()>> tasks_;
    ALooper* looper_ = nullptr;
    int fd_ = -1;
    bool terminated_ = false;
};

}  // namespace tns

#endif /* LOOPERTASKS_H_ */
