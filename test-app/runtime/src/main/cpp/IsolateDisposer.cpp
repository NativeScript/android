//
// Created by Eduardo Speroni on 3/4/22.
//

#include "IsolateDisposer.h"
#include "ArgConverter.h"
#include "MetadataNode.h"
#include "V8GlobalHelpers.h"
#include <console/Console.h>


namespace tns {
    void disposeIsolate(v8::Isolate *isolate) {
        tns::ArgConverter::onDisposeIsolate(isolate);
        tns::MetadataNode::onDisposeIsolate(isolate);
        tns::V8GlobalHelpers::onDisposeIsolate(isolate);
        tns::Console::onDisposeIsolate(isolate);
    }
}
