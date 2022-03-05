//
// Created by Eduardo Speroni on 3/4/22.
//

#ifndef TEST_APP_ISOLATEDISPOSER_H
#define TEST_APP_ISOLATEDISPOSER_H
#include "v8.h"

namespace tns {
    void disposeIsolate(v8::Isolate* isolate);
}

#endif //TEST_APP_ISOLATEDISPOSER_H
