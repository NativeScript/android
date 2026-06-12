#include "ConcurrentQueue.h"

#include <sys/eventfd.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "NativeScriptAssert.h"

namespace tns {

void ConcurrentQueue::Initialize(ALooper* looper, ALooper_callbackFunc performWork,
                                 void* data) {
    std::unique_lock<std::mutex> lock(initializationMutex_);
    if (terminated_) {
        return;
    }

    int fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (fd == -1) {
        DEBUG_WRITE_FORCE("ConcurrentQueue: eventfd failed: %s", strerror(errno));
        return;
    }

    if (ALooper_addFd(looper, fd, ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT,
                      performWork, data) != 1) {
        DEBUG_WRITE_FORCE("ConcurrentQueue: ALooper_addFd failed");
        close(fd);
        return;
    }

    this->looper_ = looper;
    ALooper_acquire(this->looper_);
    this->fd_ = fd;
}

void ConcurrentQueue::Push(std::shared_ptr<worker::Message> message) {
    // The lifecycle lock is held across the enqueue + wakeup so a concurrent
    // Terminate() can never leave a message stranded in a queue nothing will
    // ever drain.
    std::unique_lock<std::mutex> lock(initializationMutex_);
    if (terminated_) {
        // the consumer is gone - drop the message (and its backing stores)
        return;
    }

    {
        std::unique_lock<std::mutex> mlock(this->mutex_);
        this->messagesQueue_.push(message);
    }

    if (this->fd_ != -1) {
        // The eventfd counter coalesces multiple signals into one wakeup,
        // which is fine because the drain callback uses PopAll().
        uint64_t value = 1;
        write(this->fd_, &value, sizeof(value));
    }
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

    // Release anything a racing Push() enqueued before it observed
    // terminated_ - nothing will drain the queue from here on.
    {
        std::unique_lock<std::mutex> mlock(this->mutex_);
        std::queue<std::shared_ptr<worker::Message>> empty;
        this->messagesQueue_.swap(empty);
    }
}

}  // namespace tns
