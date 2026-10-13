#ifndef JSIRUNTIME_H_
#define JSIRUNTIME_H_

#include <memory>

#include "jsi/jsi.h"

#ifdef __cplusplus
extern "C" {
#endif

// The JSI Runtime of the NativeScript runtime on the calling thread, or nullptr
// when this thread has no active runtime (or its runtime has torn down). Each
// runtime — the main one and every Worker — owns a separate JSI Runtime bound
// to its V8 isolate.
__attribute__((visibility("default"))) facebook::jsi::Runtime*
NativeScriptJSIRuntime(void);

#ifdef __cplusplus
}

namespace tns {
__attribute__((visibility("default"))) std::shared_ptr<facebook::jsi::Runtime>
GetJSIRuntime();
}  // namespace tns
#endif

#endif /* JSIRUNTIME_H_ */
