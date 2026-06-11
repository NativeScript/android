#include "ConcurrentQueue.h"

#include <sys/eventfd.h>
#include <unistd.h>

namespace tns {

void ConcurrentQueue::Initialize(ALooper* looper, ALooper_callbackFunc performWork,
                                 void* data) {
    std::unique_lock<std::mutex> lock(initializationMutex_);
    if (terminated_) {
        return;
    }

    this->looper_ = looper;
    ALooper_acquire(this->looper_);

    this->fd_ = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    ALooper_addFd(this->looper_, this->fd_, ALOOPER_POLL_CALLBACK,
                  ALOOPER_EVENT_INPUT, performWork, data);
}

void ConcurrentQueue::Push(std::shared_ptr<worker::Message> message) {
    {
        std::unique_lock<std::mutex> mlock(this->mutex_);
        this->messagesQueue_.push(message);
    }

    this->SignalAndWakeUp();
}

std::vector<std::shared_ptr<worker::Message>> ConcurrentQueue::PopAll() {
    std::unique_lock<std::mutex> mlock(this->mutex_);
    std::vector<std::shared_ptr<worker::Message>> messages;

    while (!this->messagesQueue_.empty()) {
        messages.push_back(this->messagesQueue_.front());
        this->messagesQueue_.pop();
    }

    return messages;
}

void ConcurrentQueue::SignalAndWakeUp() {
    std::unique_lock<std::mutex> lock(initializationMutex_);
    if (this->fd_ != -1) {
        // The eventfd counter coalesces multiple signals into one wakeup,
        // which is fine because the drain callback uses PopAll().
        uint64_t value = 1;
        write(this->fd_, &value, sizeof(value));
    }
}

void ConcurrentQueue::Terminate() {
    // Must run on the looper's own thread: removing an fd concurrently with an
    // in-flight callback dispatch is racy.
    std::unique_lock<std::mutex> lock(initializationMutex_);
    terminated_ = true;

    if (this->fd_ != -1) {
        ALooper_removeFd(this->looper_, this->fd_);
        close(this->fd_);
        this->fd_ = -1;
    }

    if (this->looper_ != nullptr) {
        ALooper_release(this->looper_);
        this->looper_ = nullptr;
    }
}

}  // namespace tns
