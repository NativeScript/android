//
// Created by Eduardo Speroni on 3/4/22.
//

#ifndef TEST_APP_ISOLATEDISPOSER_H
#define TEST_APP_ISOLATEDISPOSER_H
#include "v8.h"

namespace tns {
    // Runs the builtin layers' isolate-teardown hooks. Subsystem state that
    // belongs to one runtime lives in RuntimeState instead; see RuntimeState.h.
    void disposeIsolate(v8::Isolate* isolate);
}

#endif //TEST_APP_ISOLATEDISPOSER_H
