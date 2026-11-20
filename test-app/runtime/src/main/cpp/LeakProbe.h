#ifndef LEAKPROBE_H_
#define LEAKPROBE_H_

#include "v8.h"

namespace tns {

// ns:runtime's findLeakCycles(): finds the JS <-> Java cycles the canary
// wrapper lifetime mode keeps alive, which the resurrecting mode breaks.
//
// Each heap has its own collector, so neither sees a cycle that crosses into
// the other: the runtime roots a canary record's state while Java holds the
// object, and Java holds an object while a canary is alive. The probe joins
// both heaps into one graph and asks what a single collector would keep:
// - the JS heap from a V8 heap snapshot, its roots minus the runtime's own
//   handles to canary records;
// - the Java heap from an HPROF dump (android.os.Debug.dumpHprofData), its GC
//   roots minus the runtime's object table, with Reference referents skipped;
// - the runtime's cross-heap edges: a live wrapper holds its Java object, and
//   a Java object holds its canary record (state, prototype, implementation).
// A recorded Java object nothing reaches is leaked; the report gives each
// leaked group a cycle through it, with a signature free of ids.
//
// Slow by design: it suspends the process for a full heap dump and parses it.
// A diagnostic, not something to run in production.
class LeakProbe {
 public:
  // The report object, or empty with a JS exception pending.
  static v8::MaybeLocal<v8::Object> FindLeakCycles(v8::Isolate* isolate);
};

}  // namespace tns

#endif /* LEAKPROBE_H_ */
