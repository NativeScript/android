#include "RuntimeGates.h"

#include <condition_variable>
#include <cstdint>
#include <mutex>

#include "robin_hood.h"

namespace tns {
namespace RuntimeGates {

namespace {

struct Gate {
  uint32_t pins = 0;
  bool closed = false;
};

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
robin_hood::unordered_map<int, Gate>& Gates() {
  static auto* gates = new robin_hood::unordered_map<int, Gate>();
  return *gates;
}

}  // namespace

void Open(int id) {
  std::lock_guard<std::mutex> lock(GatesMutex());
  Gates()[id] = Gate();
}

bool TryPin(int id) {
  std::lock_guard<std::mutex> lock(GatesMutex());
  auto it = Gates().find(id);
  if (it == Gates().end() || it->second.closed) {
    return false;
  }
  it->second.pins++;
  return true;
}

void Unpin(int id) {
  bool last = false;
  {
    std::lock_guard<std::mutex> lock(GatesMutex());
    auto it = Gates().find(id);
    if (it != Gates().end()) {
      last = --it->second.pins == 0 && it->second.closed;
    }
  }
  if (last) {
    Unpinned().notify_all();
  }
}

void Close(int id) {
  std::lock_guard<std::mutex> lock(GatesMutex());
  auto it = Gates().find(id);
  if (it != Gates().end()) {
    it->second.closed = true;
  }
}

void WaitUntilUnpinnedAndRetire(int id) {
  std::unique_lock<std::mutex> lock(GatesMutex());
  auto it = Gates().find(id);
  if (it == Gates().end()) {
    return;
  }
  it->second.closed = true;
  Unpinned().wait(lock, [id]() {
    auto found = Gates().find(id);
    return found == Gates().end() || found->second.pins == 0;
  });
  Gates().erase(id);
}

}  // namespace RuntimeGates
}  // namespace tns
