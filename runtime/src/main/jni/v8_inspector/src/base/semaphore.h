// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_BASE_PLATFORM_SEMAPHORE_H_
#define V8_BASE_PLATFORM_SEMAPHORE_H_

#include <semaphore.h>
#include "macros.h"

namespace v8 {
namespace base_copied {

// Forward declarations.
class TimeDelta;

// ----------------------------------------------------------------------------
// Semaphore
//
// A semaphore object is a synchronization object that maintains a count. The
// count is decremented each time a thread completes a wait for the semaphore
// object and incremented each time a thread signals the semaphore. When the
// count reaches zero,  threads waiting for the semaphore blocks until the
// count becomes non-zero.

class Semaphore final {
    public:
        Semaphore(int count);

        ~Semaphore();

        // Increments the semaphore counter.
        void Signal();

        // Decrements the semaphore counter if it is positive, or blocks until it
        // becomes positive and then decrements the counter.
        void Wait();

        // Like Wait() but returns after rel_time time has passed. If the timeout
        // happens the return value is false and the counter is unchanged. Otherwise
        // the semaphore counter is decremented and true is returned.
        bool WaitFor(const TimeDelta& rel_time);


        typedef sem_t NativeHandle;

        NativeHandle& native_handle() {
            return native_handle_;
        }

        const NativeHandle& native_handle() const {
            return native_handle_;
        }

    private:
        NativeHandle native_handle_;

        DISALLOW_COPY_AND_ASSIGN(Semaphore);
};

}  // namespace base
}  // namespace v8

#endif  // V8_BASE_PLATFORM_SEMAPHORE_H_
