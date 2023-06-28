#include "WeakRef.h"
#include "ArgConverter.h"
#include "V8StringConstants.h"
#include "NativeScriptException.h"
#include "V8GlobalHelpers.h"
#include <sstream>

using namespace v8;
using namespace tns;
using namespace std;

WeakRef::WeakRef() {
}

void WeakRef::Init(v8::Isolate* isolate, Local<v8::Context> context) {
    std::string source = R"(
        // WeakRef polyfills
        global.WeakRef.prototype.get = global.WeakRef.prototype.deref;
        global.WeakRef.prototype.__hasWarnedAboutClear = false;
        global.WeakRef.prototype.clear = () => {
            if(global.WeakRef.prototype.__hasWarnedAboutClear) {
                return;
            }
            global.WeakRef.prototype.__hasWarnedAboutClear = true;
            console.warn('WeakRef.clear() is non-standard and has been deprecated. It does nothing and the call can be safely removed.');
        }
    )";

    Local<Script> script;
    bool success = Script::Compile(context, ArgConverter::ConvertToV8String(isolate, source)).ToLocal(&script);
    assert(success && !script.IsEmpty());

    Local<Value> result;
    success = script->Run(context).ToLocal(&result);
    assert(success);

}