//
// Created by Eduardo Speroni on 3/4/22.
//

#include "IsolateDisposer.h"
#include "ArgConverter.h"
#include "BuiltinLoader.h"
#include "JSONObjectHelper.h"
#include "MetadataNode.h"
#include "NsBuiltinModules.h"
#include "V8GlobalHelpers.h"
#include <console/Console.h>



namespace tns {
    void disposeIsolate(v8::Isolate *isolate) {
        tns::NsBuiltinModules::onDisposeIsolate(isolate);
        tns::BuiltinLoader::onDisposeIsolate(isolate);
        // clear all isolate bound objects
        std::lock_guard<std::mutex> lock(isolateBoundObjectsMutex_);
        auto it = isolateBoundObjects_.find(isolate);
        if (it != isolateBoundObjects_.end()) {
            it->second->clear();
            isolateBoundObjects_.erase(it);
        }
    }
}
