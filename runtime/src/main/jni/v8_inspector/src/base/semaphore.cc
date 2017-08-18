// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "semaphore.h"

#include <errno.h>
#include "logging.h"

namespace v8
{
    namespace base_copied
    {
        Semaphore::Semaphore(int count)
        {
            DCHECK(count >= 0);
            int result = sem_init(&native_handle_, 0, count);
            DCHECK_EQ(0, result);
            USE(result);
        }


        Semaphore::~Semaphore()
        {
            int result = sem_destroy(&native_handle_);
            DCHECK_EQ(0, result);
            USE(result);
        }

        void Semaphore::Signal()
        {
            int result = sem_post(&native_handle_);
            // This check may fail with <libc-2.21, which we use on the try bots, if the
            // semaphore is destroyed while sem_post is still executed. A work around is
            // to extend the lifetime of the semaphore.
            CHECK_EQ(0, result);
        }


        void Semaphore::Wait()
        {
            while (true)
            {
                int result = sem_wait(&native_handle_);
                if (result == 0) return;  // Semaphore was signalled.
                // Signal caused spurious wakeup.
                DCHECK_EQ(-1, result);
                DCHECK_EQ(EINTR, errno);
            }
        }


    }  // namespace base
}  // namespace v8
