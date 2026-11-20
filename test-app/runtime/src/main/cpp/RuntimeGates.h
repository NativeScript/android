#ifndef RUNTIMEGATES_H_
#define RUNTIMEGATES_H_

namespace tns {

class Runtime;

/*
 * One gate per runtime, keyed by its id (ids are never reused); the iOS
 * runtime's IsolateGates. A thread entering another thread's runtime pins the
 * gate before it waits for the runtime's Locker and until it lets go of it.
 * The runtime's teardown closes the gate last, under the Locker, and disposes
 * the isolate and frees the runtime only once no pin is held, so a pinned
 * runtime and its isolate stay allocated. A missing gate reads as closed.
 */
namespace RuntimeGates {
// Before the runtime can be found by id.
void Open(int id, Runtime* runtime);
// The gate's runtime, pinned; null once the gate is closed, when the runtime
// may already be freed.
Runtime* TryPin(int id);
void Unpin(int id);
// A new object id from the runtime, or -1 once the gate is closed. Holds the
// gates' lock rather than a pin: the runtime is freed only after its gate is
// retired under that lock.
int TryGenerateObjectId(int id);
// From the teardown, holding the runtime's Locker.
void Close(int id);
// Before Isolate::Dispose and before the runtime is freed, without the
// runtime's Locker: a pinned thread may be waiting for it. Drops the gate.
void WaitUntilUnpinnedAndRetire(int id);
}  // namespace RuntimeGates

// Owns a pin for its scope. Declare it before the Locker it guards, so the
// Locker is released first.
class RuntimePin {
 public:
  explicit RuntimePin(int id) : m_id(id), m_runtime(RuntimeGates::TryPin(id)) {}
  ~RuntimePin() {
    if (m_runtime != nullptr) {
      RuntimeGates::Unpin(m_id);
    }
  }
  RuntimePin(const RuntimePin&) = delete;
  RuntimePin& operator=(const RuntimePin&) = delete;
  // Null when the pin was refused.
  Runtime* runtime() const { return m_runtime; }

 private:
  int m_id;
  Runtime* m_runtime;
};

}  // namespace tns

#endif /* RUNTIMEGATES_H_ */
