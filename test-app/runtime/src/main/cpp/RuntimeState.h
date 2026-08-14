#ifndef TEST_APP_RUNTIMESTATE_H
#define TEST_APP_RUNTIMESTATE_H

#include <atomic>
#include <cstddef>
#include <memory>
#include <vector>

#include "Runtime.h"
#include "v8.h"

namespace tns {

/*
 * Per-runtime storage for subsystem state that belongs to a single isolate.
 *
 * Such state used to live in process-wide maps keyed by v8::Isolate*. Keying by
 * isolate does not make the *container* private: runtimes start and tear down on
 * their own threads, so one runtime inserting its entry while another erases its
 * own corrupts the shared container. Holding the isolate's Locker does not help
 * -- each thread holds only its own isolate's lock, so two runtimes are never
 * excluded from each other.
 *
 * Hanging the state off the Runtime removes the sharing instead of guarding it:
 * nothing is shared, so there is nothing to race on and no lock on the access
 * path (a lookup is an isolate data-slot read plus a vector index). It also
 * removes the per-isolate erase at teardown -- the whole bag is destroyed once,
 * on the runtime's own thread, while the isolate is still alive, which is what
 * v8::Persistent members require.
 *
 * A subsystem declares a state struct -- typically in its own .cpp, so nothing
 * leaks into headers -- and reaches it with:
 *
 *     auto* state = RuntimeState::For<MyState>(isolate);
 *     if (state == nullptr) return;  // runtime is tearing down
 *
 * The first call for a runtime default-constructs the state; it is destroyed
 * with the runtime.
 */
class RuntimeState {
 public:
    /*
     * This runtime's instance of T, created on first use, or null once the
     * runtime has started tearing down (callers must not resurrect state that
     * teardown has already released).
     */
    template <typename T>
    static T* For(v8::Isolate* isolate);

    /*
     * Destroys every state object. Runs on the runtime's own thread from
     * DestroyRuntime, before the isolate is disposed.
     */
    void Clear() {
        disposed_ = true;
        slots_.clear();
    }

 private:
    // One slot index per state type, handed out on first use from any thread.
    static size_t NextSlotIndex();

    template <typename T>
    static size_t SlotIndexFor() {
        static const size_t index = NextSlotIndex();
        return index;
    }

    // Type-erased so Runtime need not know any subsystem's state type; the
    // deleter restores the type at destruction.
    using Slot = std::unique_ptr<void, void (*)(void*)>;

    template <typename T>
    T* GetOrCreate() {
        if (disposed_) {
            return nullptr;
        }

        size_t index = SlotIndexFor<T>();
        while (slots_.size() <= index) {
            slots_.emplace_back(nullptr, [](void*) {});
        }

        Slot& slot = slots_[index];
        if (slot == nullptr) {
            slot = Slot(new T(), [](void* value) { delete static_cast<T*>(value); });
        }

        return static_cast<T*>(slot.get());
    }

    std::vector<Slot> slots_;
    bool disposed_ = false;
};

template <typename T>
T* RuntimeState::For(v8::Isolate* isolate) {
    Runtime* runtime = Runtime::GetRuntime(isolate);
    RuntimeState* state = runtime->GetState();
    if (state == nullptr) {
        return nullptr;
    }

    return state->GetOrCreate<T>();
}

}  // namespace tns

#endif  // TEST_APP_RUNTIMESTATE_H
