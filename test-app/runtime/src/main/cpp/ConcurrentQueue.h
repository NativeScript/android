#ifndef CONCURRENTQUEUE_H_
#define CONCURRENTQUEUE_H_

#include <android/looper.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include "WorkerMessage.h"

namespace tns {

/*
 * Thread-safe message inbox attached to an ALooper.
 * Push() may be called from any thread; messages pushed before Initialize()
 * are queued and can be drained explicitly once the looper is ready.
 * Initialize()/PopAll()/Terminate() must be called on the looper's thread.
 */
struct ConcurrentQueue {
public:
    void Initialize(ALooper* looper, ALooper_callbackFunc performWork, void* data);
    void Push(std::shared_ptr<worker::Message> message);
    void Signal();
    bool IsEmpty();
    std::vector<std::shared_ptr<worker::Message>> PopAll();
    void Terminate();

private:
    std::queue<std::shared_ptr<worker::Message>> messagesQueue_;
    ALooper* looper_ = nullptr;
    int fd_ = -1;
    // Read under either lock: Terminate() sets it holding the lifecycle lock
    // and empties the queue holding the queue mutex, and a push must be turned
    // away by whichever of the two it reaches first.
    std::atomic<bool> terminated_{false};
    std::mutex mutex_;
    std::mutex initializationMutex_;
};

}  // namespace tns

#endif /* CONCURRENTQUEUE_H_ */
