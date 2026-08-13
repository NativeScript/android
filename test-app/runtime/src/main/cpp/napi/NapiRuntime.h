#ifndef NAPIRUNTIME_H_
#define NAPIRUNTIME_H_

// The NativeScript-specific piece of the Node-API surface: obtaining the env
// outside a Node-API callback. Everything else an addon needs is the standard
// <node_api.h>, which ships next to this header.
#include "node_api.h"

#ifdef __cplusplus
extern "C" {
#endif

// The Node-API environment of the runtime on the calling thread, or NULL when
// this thread has no runtime (or its runtime has torn down). Each runtime —
// the main one and every Worker — owns a separate env.
napi_env NativeScriptNapiEnv(void);

#ifdef __cplusplus
}
#endif

#endif /* NAPIRUNTIME_H_ */
