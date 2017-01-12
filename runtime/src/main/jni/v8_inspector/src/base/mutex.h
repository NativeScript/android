// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_PLATFORM_MUTEX_H_
#define V8_BASE_PLATFORM_MUTEX_H_

#include "logging.h"
#include "macros.h"
#include <pthread.h>

namespace v8 {
namespace base {

// ----------------------------------------------------------------------------
// Mutex
//
// This class is a synchronization primitive that can be used to protect shared
// data from being simultaneously accessed by multiple threads. A mutex offers
// exclusive, non-recursive ownership semantics:
// - A calling thread owns a mutex from the time that it successfully calls
//   either |Lock()| or |TryLock()| until it calls |Unlock()|.
// - When a thread owns a mutex, all other threads will block (for calls to
//   |Lock()|) or receive a |false| return value (for |TryLock()|) if they
//   attempt to claim ownership of the mutex.
// A calling thread must not own the mutex prior to calling |Lock()| or
// |TryLock()|. The behavior of a program is undefined if a mutex is destroyed
// while still owned by some thread. The Mutex class is non-copyable.

class Mutex final {
    public:
        Mutex();

        ~Mutex();

        // Locks the given mutex. If the mutex is currently unlocked, it becomes
        // locked and owned by the calling thread, and immediately. If the mutex
        // is already locked by another thread, suspends the calling thread until
        // the mutex is unlocked.
        void Lock();

        // Unlocks the given mutex. The mutex is assumed to be locked and owned by
        // the calling thread on entrance.
        void Unlock();

        // Tries to lock the given mutex. Returns whether the mutex was
        // successfully locked.
        bool TryLock();

        // The implementation-defined native handle type.
        typedef pthread_mutex_t NativeHandle;

        NativeHandle& native_handle() {
            return native_handle_;
        }

        const NativeHandle& native_handle() const {
            return native_handle_;
        }

    private:
        NativeHandle native_handle_;

        void AssertHeldAndUnmark() {
        }

        void AssertUnheldAndMark() {
        }

        friend class ConditionVariable;

        DISALLOW_COPY_AND_ASSIGN(Mutex);
};



// -----------------------------------------------------------------------------
// RecursiveMutex
//
// This class is a synchronization primitive that can be used to protect shared
// data from being simultaneously accessed by multiple threads. A recursive
// mutex offers exclusive, recursive ownership semantics:
// - A calling thread owns a recursive mutex for a period of time that starts
//   when it successfully calls either |Lock()| or |TryLock()|. During this
//   period, the thread may make additional calls to |Lock()| or |TryLock()|.
//   The period of ownership ends when the thread makes a matching number of
//   calls to |Unlock()|.
// - When a thread owns a recursive mutex, all other threads will block (for
//   calls to |Lock()|) or receive a |false| return value (for |TryLock()|) if
//   they attempt to claim ownership of the recursive mutex.
// - The maximum number of times that a recursive mutex may be locked is
//   unspecified, but after that number is reached, calls to |Lock()| will
//   probably abort the process and calls to |TryLock()| return false.
// The behavior of a program is undefined if a recursive mutex is destroyed
// while still owned by some thread. The RecursiveMutex class is non-copyable.

class RecursiveMutex final {
    public:
        RecursiveMutex();

        ~RecursiveMutex();

        // Locks the mutex. If another thread has already locked the mutex, a call to
        // |Lock()| will block execution until the lock is acquired. A thread may call
        // |Lock()| on a recursive mutex repeatedly. Ownership will only be released
        // after the thread makes a matching number of calls to |Unlock()|.
        // The behavior is undefined if the mutex is not unlocked before being
        // destroyed, i.e. some thread still owns it.
        void Lock();

        // Unlocks the mutex if its level of ownership is 1 (there was exactly one
        // more call to |Lock()| than there were calls to unlock() made by this
        // thread), reduces the level of ownership by 1 otherwise. The mutex must be
        // locked by the current thread of execution, otherwise, the behavior is
        // undefined.
        void Unlock();

        // Tries to lock the given mutex. Returns whether the mutex was
        // successfully locked.
        bool TryLock();

        // The implementation-defined native handle type.
        typedef Mutex::NativeHandle NativeHandle;

        NativeHandle& native_handle() {
            return native_handle_;
        }

        const NativeHandle& native_handle() const {
            return native_handle_;
        }

    private:
        NativeHandle native_handle_;
        DISALLOW_COPY_AND_ASSIGN(RecursiveMutex);
};




// -----------------------------------------------------------------------------
// LockGuard
//
// This class is a mutex wrapper that provides a convenient RAII-style mechanism
// for owning a mutex for the duration of a scoped block.
// When a LockGuard object is created, it attempts to take ownership of the
// mutex it is given. When control leaves the scope in which the LockGuard
// object was created, the LockGuard is destructed and the mutex is released.
// The LockGuard class is non-copyable.

template<typename Mutex>
class LockGuard final {
    public:
        explicit LockGuard(Mutex* mutex) : mutex_(mutex) {
            mutex_->Lock();
        }

        ~LockGuard() {
            mutex_->Unlock();
        }

    private:
        Mutex* mutex_;

        DISALLOW_COPY_AND_ASSIGN(LockGuard);
};

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_PLATFORM_MUTEX_H_
