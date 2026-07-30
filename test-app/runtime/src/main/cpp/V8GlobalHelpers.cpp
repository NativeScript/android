#include "V8GlobalHelpers.h"
#include "ArgConverter.h"
#include "CallbackHandlers.h"
#include "include/v8.h"
#include "JEnv.h"
#include "MetadataNode.h"
#include "NativeScriptException.h"
#include "ObjectManager.h"
#include "Runtime.h"
#include <algorithm>
#include <sstream>

using namespace v8;
using namespace std;

// Metadata carries slashed JNI names; logs read better dotted.
static std::string ToDottedName(std::string name) {
    std::replace(name.begin(), name.end(), '/', '.');
    return name;
}

// A JS object linked to a Java counterpart holds a JSInstanceInfo External in
// its first internal field. The field count alone does not identify one: V8
// hands typed arrays and array buffers the same number of embedder fields.
static bool HasJavaCounterpart(Isolate* isolate, const Local<Object>& object) {
    auto objectManager = tns::Runtime::GetObjectManager(isolate);
    if (objectManager == nullptr || !objectManager->IsJsRuntimeObject(object)) {
        return false;
    }

    const int jsInfoIdx = static_cast<int>(tns::ObjectManager::MetadataNodeKeys::JsInfo);
    return object->GetInternalField(jsInfoIdx).As<Value>()->IsExternal();
}

std::string tns::GetNativeWrapperHint(Isolate* isolate, const Local<Value>& value) {
    if (isolate == nullptr || value.IsEmpty() || !value->IsObject() || value->IsFunction()) {
        return "";
    }

    auto object = value.As<Object>();
    std::string name;

    if (MetadataNode::TryGetPackageName(isolate, object, name)) {
        return "package " + ToDottedName(name);
    }

    if (MetadataNode::TryGetInstanceTypeName(isolate, object, name)) {
        return ToDottedName(name);
    }

    if (!HasJavaCounterpart(isolate, object)) {
        return "";
    }

    // TypeScript instances sit one level below the registered instance, which
    // is where the metadata lives (see the object layout notes in
    // MetadataNode.h).
    auto prototype = object->GetPrototype();
    if (prototype->IsObject() &&
            MetadataNode::TryGetInstanceTypeName(isolate, prototype.As<Object>(), name)) {
        return ToDottedName(name);
    }

    return "JavaObject";
}

bool tns::V8GetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName, Local<Value>& out) {
    auto privateKey = Private::ForApi(isolate, propName);

    auto context = obj->GetCreationContext(isolate).ToLocalChecked();
    auto hasPrivate = obj->HasPrivate(context, privateKey);

    if (hasPrivate.IsNothing()) {
        stringstream ss;
        ss << "Failed to Get Private Value for prop: " << ArgConverter::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    if (!hasPrivate.FromMaybe(false)) {
        return false;
    }

    auto res = obj->GetPrivate(context, privateKey);

    if (res.IsEmpty()) {
        stringstream ss;
        ss << "Failed to Get Private Value for prop: " << ArgConverter::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    return res.ToLocal(&out);
}

bool tns::V8SetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName, const Local<Value>& value) {
    auto privateKey = Private::ForApi(isolate, propName);
    auto context = obj->GetCreationContext(isolate).ToLocalChecked();
    auto res = obj->SetPrivate(context, privateKey, value);

    if (res.IsNothing()) {
        stringstream ss;
        ss << "Failed to Set Private Value for prop: " << ArgConverter::ConvertToString(propName).c_str() << endl;
        throw tns::NativeScriptException(ss.str());
    }

    return res.FromMaybe(false);
}
