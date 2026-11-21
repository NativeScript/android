#include "RuntimeState.h"

#include <atomic>

#include "NativeScriptAssert.h"

namespace tns {

size_t RuntimeState::ClaimSlotIndex() {
    // Constant-initialized: called from the dynamic initializers of
    // RuntimeState::kSlotIndex, in whatever order the library runs them.
    static std::atomic<size_t> next{0};
    size_t index = next.fetch_add(1, std::memory_order_relaxed);
    NS_CHECK(index < kCapacity);
    return index;
}

void RuntimeState::Clear() {
    disposed_ = true;

    // Every slot is emptied before any state is destroyed, so a destructor
    // that looks up state, its own included, gets null.
    void* values[kCapacity];
    for (size_t i = 0; i < kCapacity; i++) {
        values[i] = slots_[i];
        slots_[i] = nullptr;
    }

    for (size_t i = kCapacity; i-- > 0;) {
        if (values[i] != nullptr) {
            deleters_[i](values[i]);
        }
    }
}

}  // namespace tns
