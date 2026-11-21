#ifndef TEST_APP_RUNTIMESTATE_H
#define TEST_APP_RUNTIMESTATE_H

#include <cstddef>

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
 * path (a lookup is an isolate data-slot read plus an array index). It also
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
 * or with For<MyState>(runtime) where the Runtime is already at hand.
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

    template <typename T>
    static T* For(Runtime* runtime);

    ~RuntimeState() {
        Clear();
    }

    /*
     * Destroys every state object. Runs on the runtime's own thread from
     * DestroyRuntime, before the isolate is disposed.
     */
    void Clear();

    // More state types than this abort the library load; raise it then.
    static constexpr size_t kCapacity = 32;

 private:
    static size_t ClaimSlotIndex();

    // One slot index per state type, claimed while the library loads so a
    // lookup reads a plain constant instead of a function-local static's
    // guard. ClaimSlotIndex's counter is constant-initialized, so it is ready
    // whichever translation unit's initializers run first.
    template <typename T>
    static inline const size_t kSlotIndex = ClaimSlotIndex();

    template <typename T>
    T* Get() {
        if (void* value = slots_[kSlotIndex<T>]) [[likely]] {
            return static_cast<T*>(value);
        }
        return Create<T>();
    }

    template <typename T>
    [[gnu::noinline]] T* Create() {
        // Clear empties every slot, so a lookup during or after teardown
        // ends up here.
        if (disposed_) {
            return nullptr;
        }

        const size_t index = kSlotIndex<T>;
        T* value = new T();
        slots_[index] = value;
        deleters_[index] = [](void* p) { delete static_cast<T*>(p); };
        return value;
    }

    // Type-erased so Runtime need not know any subsystem's state type; the
    // deleter restores the type at destruction.
    void* slots_[kCapacity] = {};
    void (*deleters_[kCapacity])(void*) = {};
    bool disposed_ = false;
};

template <typename T>
T* RuntimeState::For(v8::Isolate* isolate) {
    // TryGetRuntime, not GetRuntime: this runs from GC weak callbacks, where
    // throwing is not an option.
    auto* runtime = Runtime::TryGetRuntime(isolate);
    if (runtime == nullptr) {
        return nullptr;
    }

    return For<T>(runtime);
}

template <typename T>
T* RuntimeState::For(Runtime* runtime) {
    RuntimeState* state = runtime->GetState();
    if (state == nullptr) {
        return nullptr;
    }

    return state->Get<T>();
}

}  // namespace tns

#endif  // TEST_APP_RUNTIMESTATE_H
