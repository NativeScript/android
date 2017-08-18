//
// Created by blagoev on 4.11.2016 г..
//

#ifndef TEST_APP_V8_THREAD_H
#define TEST_APP_V8_THREAD_H
// ----------------------------------------------------------------------------
// Thread
//
// Thread objects are used for creating and running threads. When the start()
// method is called the new thread starts running the run() method in the new
// thread. The Thread object should not be deallocated before the thread has
// terminated.

#include "stdint.h"
#include "semaphore.h"

namespace v8 {
namespace base {
class Thread {
    public:
        // Opaque data type for thread-local storage keys.
        typedef int32_t LocalStorageKey;

        class Options {
            public:
                Options() : name_("v8:<unknown>"), stack_size_(0) {
                }

                explicit Options(const char* name, int stack_size = 0)
                    : name_(name), stack_size_(stack_size) {
                }

                const char* name() const {
                    return name_;
                }

                int stack_size() const {
                    return stack_size_;
                }

            private:
                const char* name_;
                int stack_size_;
        };

        // Create new thread.
        explicit Thread(const Options& options);

        virtual ~Thread();

        // Start new thread by calling the Run() method on the new thread.
        void  Start();

        // Start new thread and wait until Run() method is called on the new thread.
        void StartSynchronously() {
            start_semaphore_ = new v8::base_copied::Semaphore(0);
            Start();
            start_semaphore_->Wait();
            delete start_semaphore_;
            start_semaphore_ = NULL;
        }

        // Wait until thread terminates.
        void Join();

        inline const char* name() const {
            return name_;
        }

        // Abstract method for run handler.
        virtual void Run() = 0;

        // Thread-local storage.
        static LocalStorageKey CreateThreadLocalKey();

        static void DeleteThreadLocalKey(LocalStorageKey key);

        static void* GetThreadLocal(LocalStorageKey key);

        static int GetThreadLocalInt(LocalStorageKey key) {
            return static_cast<int>(reinterpret_cast<intptr_t>(GetThreadLocal(key)));
        }

        static void SetThreadLocal(LocalStorageKey key, void* value);

        static void SetThreadLocalInt(LocalStorageKey key, int value) {
            SetThreadLocal(key, reinterpret_cast<void*>(static_cast<intptr_t>(value)));
        }

        static bool HasThreadLocal(LocalStorageKey key) {
            return GetThreadLocal(key) != NULL;
        }

        static inline void* GetExistingThreadLocal(LocalStorageKey key) {
            return GetThreadLocal(key);
        }

        // The thread name length is limited to 16 based on Linux's implementation of
        // prctl().
        static const int kMaxThreadNameLength = 16;

        class PlatformData;

        PlatformData* data() {
            return data_;
        }

        void NotifyStartedAndRun() {
            if (start_semaphore_) {
                start_semaphore_->Signal();
            }
            Run();
        }

    private:
        void set_name(const char* name);

        PlatformData* data_;

        char name_[kMaxThreadNameLength];
        int stack_size_;
        v8::base_copied::Semaphore* start_semaphore_;

        DISALLOW_COPY_AND_ASSIGN(Thread);
};

}  // namespace base
}  // namespace v8
#endif //TEST_APP_V8_THREAD_H
