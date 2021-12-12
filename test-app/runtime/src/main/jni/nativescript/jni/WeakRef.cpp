
#include "WeakRef.h"
#include "ArgConverter.h"

using namespace v8;

namespace tns {

    void WeakRef::Init(Local<Context> context) {
        Isolate* isolate = context->GetIsolate();

        std::string source = R"(
        global.WeakRef.prototype.get = global.WeakRef.prototype.deref;
        global.WeakRef.prototype.clear = () => {
            console.warn('WeakRef.clear() is non-standard and has been deprecated. It does nothing and the call can be safely removed.');
        }
    )";

        Local<Script> script;
        bool success = Script::Compile(context, ArgConverter::ConvertToV8String(isolate, source)).ToLocal(&script);

        Local<Value> result;
        success = script->Run(context).ToLocal(&result);
    }

}