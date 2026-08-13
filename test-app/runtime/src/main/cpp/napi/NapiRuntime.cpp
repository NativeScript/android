#include "NapiRuntime.h"

#include "Runtime.h"

extern "C" napi_env NativeScriptNapiEnv(void) {
  // The thread-local can go stale when a Runtime is destroyed on a different
  // thread than the one that created it, so the env is resolved through the
  // registry without dereferencing the pointer outside its lock.
  return tns::Runtime::GetNapiEnvIfAlive(tns::Runtime::GetCurrentRuntime());
}
