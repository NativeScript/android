#ifndef NAPITHREADSAFEFUNCTION_H_
#define NAPITHREADSAFEFUNCTION_H_

namespace tns {

class NapiEnv;

// Closes every threadsafe function still bound to `env`: queued calls are
// dropped, producer threads blocked on a full queue are woken and answered
// with napi_closing, and each function's finalizer runs here.
//
// Runs on the env's own thread with the isolate locked, at the head of env
// teardown: each function holds a reference to its JS callback, which must be
// deleted before the env finalizes its reference lists. Handles stay
// dereferenceable until their owning threads release them; they simply stop
// being able to reach JS.
void NapiAbortThreadSafeFunctions(NapiEnv* env);

}  // namespace tns

#endif /* NAPITHREADSAFEFUNCTION_H_ */
