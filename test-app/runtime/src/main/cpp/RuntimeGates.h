#ifndef RUNTIMEGATES_H_
#define RUNTIMEGATES_H_

namespace tns {

/*
 * One gate per runtime, keyed by its id (ids are never reused); the iOS
 * runtime's IsolateGates. A thread entering another thread's runtime pins the
 * gate before it waits for the runtime's Locker and until it lets go of it.
 * The runtime's teardown closes the gate last, under the Locker, and disposes
 * the isolate only once no pin is held, so a pinned runtime and its isolate
 * stay allocated. A missing gate reads as closed.
 */
namespace RuntimeGates {
// Before the runtime can be found by id.
void Open(int id);
// False once the gate is closed: the runtime may already be freed.
bool TryPin(int id);
void Unpin(int id);
// From the teardown, holding the runtime's Locker.
void Close(int id);
// Before Isolate::Dispose, without the runtime's Locker: a pinned thread may
// be waiting for it. Drops the gate.
void WaitUntilUnpinnedAndRetire(int id);
}  // namespace RuntimeGates

// Owns a pin for its scope. Declare it before the Locker it guards, so the
// Locker is released first.
class RuntimePin {
 public:
  explicit RuntimePin(int id) : m_id(id), m_pinned(RuntimeGates::TryPin(id)) {}
  ~RuntimePin() {
    if (m_pinned) {
      RuntimeGates::Unpin(m_id);
    }
  }
  RuntimePin(const RuntimePin&) = delete;
  RuntimePin& operator=(const RuntimePin&) = delete;
  explicit operator bool() const { return m_pinned; }

 private:
  int m_id;
  bool m_pinned;
};

}  // namespace tns

#endif /* RUNTIMEGATES_H_ */
