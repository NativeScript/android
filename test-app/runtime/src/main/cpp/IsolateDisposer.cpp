//
// Created by Eduardo Speroni on 3/4/22.
//

#include "IsolateDisposer.h"
#include "ArgConverter.h"
#include "MetadataNode.h"
#include "V8GlobalHelpers.h"
#include <console/Console.h>



namespace tns {
    void disposeIsolate(v8::Isolate* isolate) {
        tns::ArgConverter::disposeIsolate(isolate);
        tns::MetadataNode::disposeIsolate(isolate);
        tns::disposeHelperIsolate(isolate);
        Console::disposeIsolate(isolate);
    }
}