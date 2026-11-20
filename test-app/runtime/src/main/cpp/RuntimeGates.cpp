#include "RuntimeGates.h"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>

#include "Runtime.h"
#include "robin_hood.h"

namespace tns {
namespace RuntimeGates {

// The pin count shifted left by one, and kClosed. Pins are taken and the gate
// closed under the gates' lock; a pin is let go of with one atomic decrement,
// after which the unpinning thread never touches the gate: a waiting teardown
// may free it as soon as it reads no pins.
constexpr uint32_t kClosed = 1;
constexpr uint32_t kPin = 2;

struct Gate {
  Runtime* runtime = nullptr;
  std::atomic<uint32_t> state{0};
};

namespace {

std::mutex& GatesMutex() {
  static auto* mutex = new std::mutex();
  return *mutex;
}

std::condition_variable& Unpinned() {
  static auto* unpinned = new std::condition_variable();
  return *unpinned;
}

// Never destroyed: process exit runs static destructors while other threads
// may still be pinning.
// Boxed: a pin holds its gate's address across rehashes.
robin_hood::unordered_map<int, std::unique_ptr<Gate>>& Gates() {
  static auto* gates = new robin_hood::unordered_map<int, std::unique_ptr<Gate>>();
  return *gates;
}

}  // namespace

void Open(int id, Runtime* runtime) {
  auto gate = std::make_unique<Gate>();
  gate->runtime = runtime;
  std::lock_guard<std::mutex> lock(GatesMutex());
  Gates()[id] = std::move(gate);
}

Gate* TryPin(int id, Runtime** runtime) {
  std::lock_guard<std::mutex> lock(GatesMutex());
  auto it = Gates().find(id);
  if (it == Gates().end() || (it->second->state.load(std::memory_order_relaxed) & kClosed) != 0) {
    return nullptr;
  }
  Gate* gate = it->second.get();
  gate->state.fetch_add(kPin, std::memory_order_relaxed);
  *runtime = gate->runtime;
  return gate;
}

int TryGenerateObjectId(int id) {
  std::lock_guard<std::mutex> lock(GatesMutex());
  auto it = Gates().find(id);
  if (it == Gates().end() || (it->second->state.load(std::memory_order_relaxed) & kClosed) != 0) {
    return -1;
  }
  return it->second->runtime->GenerateNewObjectId(nullptr, nullptr);
}

void Unpin(Gate* gate) {
  // Release: what the pinned thread did happens before the teardown that reads no pins.
  if (gate->state.fetch_sub(kPin, std::memory_order_acq_rel) == (kPin | kClosed)) {
    // Taking the lock orders the notify after a waiter's check of the state.
    std::lock_guard<std::mutex> lock(GatesMutex());
    Unpinned().notify_all();
  }
}

void Close(int id) {
  std::lock_guard<std::mutex> lock(GatesMutex());
  auto it = Gates().find(id);
  if (it != Gates().end()) {
    it->second->state.fetch_or(kClosed, std::memory_order_relaxed);
  }
}

void WaitUntilUnpinnedAndRetire(int id) {
  std::unique_lock<std::mutex> lock(GatesMutex());
  auto it = Gates().find(id);
  if (it == Gates().end()) {
    return;
  }
  Gate* gate = it->second.get();
  gate->state.fetch_or(kClosed, std::memory_order_relaxed);
  Unpinned().wait(lock, [gate]() {
    return gate->state.load(std::memory_order_acquire) == kClosed;
  });
  Gates().erase(id);
}

}  // namespace RuntimeGates
}  // namespace tns
