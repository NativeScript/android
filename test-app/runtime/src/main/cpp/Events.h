#ifndef EVENTS_H_
#define EVENTS_H_

#include "v8.h"

namespace tns {

class Events {
public:
    /*
     * Installs the generic WHATWG event primitives: the Event and EventTarget
     * constructors on globalThis, the EventTarget methods (addEventListener /
     * removeEventListener / dispatchEvent) bound onto globalThis, and the
     * internal EventTarget instance backing the global. Evaluated once per
     * isolate during PrepareV8Runtime, before ErrorEvents::Init, for both the
     * main and worker isolates. Stashes the backing target in
     * Runtime::GlobalEventTarget() so native layers can dispatch without
     * going through overwritable globals.
     */
    static void Init(v8::Local<v8::Context> context);
};

}  // namespace tns

#endif /* EVENTS_H_ */
