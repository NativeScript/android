#include "WeakRef.h"
#include "NativeScriptAssert.h"
#include "ArgConverter.h"
#include "BuiltinLoader.h"
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
    bool success = !BuiltinLoader::RunBuiltin(context, BuiltinId::kWeakRef).IsEmpty();
    NS_DCHECK(success);
}
