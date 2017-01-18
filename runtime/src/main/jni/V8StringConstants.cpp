#include "V8StringConstants.h"
#include "Runtime.h"

using namespace v8;
using namespace std;

namespace tns {
V8StringConstants::PerIsolateV8Constants* V8StringConstants::GetConstantsForIsolate(v8::Isolate* isolate) {
    auto data = isolate->GetData((uint32_t)Runtime::IsolateData::CONSTANTS);
    auto consts = reinterpret_cast<V8StringConstants::PerIsolateV8Constants*>(data);

    // assert that the structure which contains the constants is not null for the current Isolate
    assert(consts != nullptr);

    return consts;
}

Local<String> V8StringConstants::GetClassImplementationObject(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->CLASS_IMPLEMENTATION_OBJECT_PERSISTENT);
}

Local<String> V8StringConstants::GetExtend(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->EXTEND_PERSISTENT);
}

Local<String> V8StringConstants::GetNullObject(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->NULL_OBJECT_PERSISTENT);
}

Local<String> V8StringConstants::GetNullNodeName(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->NULL_NODE_NAME_PERSISTENT);
}

Local<String> V8StringConstants::GetIsPrototypeImplementationObject(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->IS_PROTOTYPE_IMPLEMENTATION_OBJECT_PERSISTENT);
}

Local<String> V8StringConstants::GetNativeException(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->NATIVE_EXCEPTION_PERSISTENT);
}

Local<String> V8StringConstants::GetStackTrace(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->STACK_TRACE_PERSISTENT);
}

Local<String> V8StringConstants::GetLongNumber(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *(consts->LONG_NUMBER_PERSISTENT));
}

Local<String> V8StringConstants::GetPrototype(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->PROTOTYPE_PERSISTENT);
}

Local<String> V8StringConstants::GetSuper(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->SUPER_PERSISTENT);
}

Local<String> V8StringConstants::GetTarget(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->TARGET_PERSISTENT);
}

Local<String> V8StringConstants::GetToString(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->TO_STRING_PERSISTENT);
}

Local<String> V8StringConstants::GetJavaLong(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->JAVA_LONG_PERSISTENT);
}

Local<String> V8StringConstants::GetValue(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->VALUE_PERSISTENT);
}

Local<String> V8StringConstants::GetValueOf(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->VALUE_OF_PERSISTENT);
}

Local<String> V8StringConstants::GetUncaughtError(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->UNCAUGHT_ERROR_PERSISTENT);
}

Local<String> V8StringConstants::GetImplementationObject(Isolate* isolate) {
    auto consts = GetConstantsForIsolate(isolate);

    return Local<String>::New(isolate, *consts->IMPLEMENTATION_OBJECT_PERSISTENT);
}

const string V8StringConstants::CLASS_IMPLEMENTATION_OBJECT = "t::ClassImplementationObject";
const string V8StringConstants::EXTEND = "extend";
const string V8StringConstants::NULL_OBJECT = "null";
const string V8StringConstants::NULL_NODE_NAME = "nullNode";
const string V8StringConstants::IS_PROTOTYPE_IMPLEMENTATION_OBJECT = "__isPrototypeImplementationObject";
const string V8StringConstants::NATIVE_EXCEPTION = "nativeException";
const string V8StringConstants::STACK_TRACE = "stackTrace";
const string V8StringConstants::LONG_NUMBER = "NativeScriptLongNumber";
const string V8StringConstants::PROTOTYPE = "prototype";
const string V8StringConstants::SUPER = "super";
const string V8StringConstants::TARGET = "target";
const string V8StringConstants::TO_STRING = "toString";
const string V8StringConstants::JAVA_LONG = "t::JavaLong";
const string V8StringConstants::VALUE = "value";
const string V8StringConstants::VALUE_OF = "valueOf";
const string V8StringConstants::UNCAUGHT_ERROR = "__onUncaughtError";
const string V8StringConstants::IMPLEMENTATION_OBJECT = "t::implObj";
}
