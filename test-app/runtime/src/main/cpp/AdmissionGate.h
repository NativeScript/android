#ifndef ADMISSIONGATE_H_
#define ADMISSIONGATE_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include "v8-locker.h"

namespace tns {

/*
 * Orders the Java threads calling into one runtime before they take its
 * v8::Locker. V8's mutex spins only briefly before sleeping, and waking a
 * sleeper takes far longer than a call usually holds the isolate, so with two
 * callers every handoff left the isolate idle for a whole wake-up. A waiter
 * here spins for a while first, staying on its core to take over at once.
 *
 * Held for exactly as long as the Locker it precedes. Releasing the isolate
 * with a v8::Unlocker while keeping the gate would block every other Java
 * caller on an idle isolate.
 */
class AdmissionGate {
 public:
  void Enter() {
    // A woken sleeper spins again rather than getting one try, or under steady
    // load the spinners would keep beating it to the gate.
    for (;;) {
      if (TryTake() || Spin()) {
        return;
      }
      std::unique_lock<std::mutex> lock(m_mutex);
      m_sleepers.fetch_add(1);
      while (m_busy.load()) {
        m_wakeup.wait(lock);
      }
      m_sleepers.fetch_sub(1);
    }
  }

  void Exit() {
    m_busy.store(false);
    // Pairs with a sleeper's increment before it checks m_busy: either it sees
    // the gate free, or this sees it waiting.
    if (m_sleepers.load() > 0) {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_wakeup.notify_one();
    }
  }

  // How long a caller spins before sleeping; negative bypasses the gate.
  inline static std::atomic<int> s_spinMicros{50};

 private:
  bool Spin() {
    int budget = s_spinMicros.load(std::memory_order_relaxed);
    if (budget <= 0) {
      return false;
    }
    auto deadline = std::chrono::steady_clock::now() + std::chrono::microseconds(budget);
    for (unsigned i = 1;; i++) {
      CpuRelax();
      if (TryTake()) {
        return true;
      }
      if ((i & 63) == 0 && std::chrono::steady_clock::now() >= deadline) {
        return false;
      }
    }
  }

  bool TryTake() {
    bool expected = false;
    return !m_busy.load(std::memory_order_relaxed) &&
           m_busy.compare_exchange_weak(expected, true, std::memory_order_acquire);
  }

  static void CpuRelax() {
#if defined(__aarch64__) || defined(__arm__)
    asm volatile("yield");
#elif defined(__x86_64__) || defined(__i386__)
    asm volatile("pause");
#endif
  }

  std::atomic<bool> m_busy{false};
  std::atomic<int> m_sleepers{0};
  std::mutex m_mutex;
  std::condition_variable m_wakeup;
};

// Holds the gate for a call into JS from Java, unless this thread already holds
// the isolate (a nested call), where waiting on the gate could deadlock.
class AdmissionScope {
 public:
  AdmissionScope(AdmissionGate& gate, v8::Isolate* isolate) {
    if (AdmissionGate::s_spinMicros.load(std::memory_order_relaxed) >= 0 &&
        !v8::Locker::IsLocked(isolate)) {
      gate.Enter();
      m_gate = &gate;
    }
  }
  ~AdmissionScope() {
    if (m_gate != nullptr) {
      m_gate->Exit();
    }
  }
  AdmissionScope(const AdmissionScope&) = delete;
  AdmissionScope& operator=(const AdmissionScope&) = delete;

 private:
  AdmissionGate* m_gate = nullptr;
};

}  // namespace tns

#endif  // ADMISSIONGATE_H_
