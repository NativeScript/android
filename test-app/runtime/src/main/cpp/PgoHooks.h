#ifndef PGOHOOKS_H_
#define PGOHOOKS_H_

#include "v8.h"

namespace tns {

/*
 * Installs __pgoDump(path) on the global template when the runtime is built
 * with -fprofile-generate -DNS_PGO_GENERATE (see docs/pgo.md); a no-op in
 * every other build. __pgoDump writes the counters gathered so far to `path`
 * and returns 0 on success. Android kills app processes instead of letting
 * them exit, so the profile runtime's own at-exit write never happens and the
 * training run has to call this explicitly.
 */
void InstallPgoHooks(v8::Isolate* isolate,
                     v8::Local<v8::ObjectTemplate> globalTemplate);

}  // namespace tns

#endif /* PGOHOOKS_H_ */
