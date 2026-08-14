#include "RuntimeState.h"

namespace tns {

size_t RuntimeState::NextSlotIndex() {
    // Slot indices are claimed the first time a state type is used, which can
    // happen on any runtime's thread -- two workers touching two different
    // subsystems for the first time race here.
    static std::atomic<size_t> next{0};
    return next.fetch_add(1, std::memory_order_relaxed);
}

}  // namespace tns
