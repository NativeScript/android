// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Runtime.h"

#include "src/inspector/protocol/Protocol.h"

namespace v8_inspector {
namespace protocol {
namespace Runtime {

// ------------- Enum values from types.

const char Metainfo::domainName[] = "Runtime";
const char Metainfo::commandPrefix[] = "Runtime.";
const char Metainfo::version[] = "1.3";

namespace UnserializableValueEnum {
const char* Infinity = "Infinity";
const char* NaN = "NaN";
const char* NegativeInfinity = "-Infinity";
const char* Negative0 = "-0";
} // namespace UnserializableValueEnum

const char* RemoteObject::TypeEnum::Object = "object";
const char* RemoteObject::TypeEnum::Function = "function";
const char* RemoteObject::TypeEnum::Undefined = "undefined";
const char* RemoteObject::TypeEnum::String = "string";
const char* RemoteObject::TypeEnum::Number = "number";
const char* RemoteObject::TypeEnum::Boolean = "boolean";
const char* RemoteObject::TypeEnum::Symbol = "symbol";

const char* RemoteObject::SubtypeEnum::Array = "array";
const char* RemoteObject::SubtypeEnum::Null = "null";
const char* RemoteObject::SubtypeEnum::Node = "node";
const char* RemoteObject::SubtypeEnum::Regexp = "regexp";
const char* RemoteObject::SubtypeEnum::Date = "date";
const char* RemoteObject::SubtypeEnum::Map = "map";
const char* RemoteObject::SubtypeEnum::Set = "set";
const char* RemoteObject::SubtypeEnum::Weakmap = "weakmap";
const char* RemoteObject::SubtypeEnum::Weakset = "weakset";
const char* RemoteObject::SubtypeEnum::Iterator = "iterator";
const char* RemoteObject::SubtypeEnum::Generator = "generator";
const char* RemoteObject::SubtypeEnum::Error = "error";
const char* RemoteObject::SubtypeEnum::Proxy = "proxy";
const char* RemoteObject::SubtypeEnum::Promise = "promise";
const char* RemoteObject::SubtypeEnum::Typedarray = "typedarray";

std::unique_ptr<RemoteObject> RemoteObject::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<RemoteObject> result(new RemoteObject());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* subtypeValue = object->get("subtype");
    if (subtypeValue) {
        errors->setName("subtype");
        result->m_subtype = ValueConversions<String>::fromValue(subtypeValue, errors);
    }
    protocol::Value* classNameValue = object->get("className");
    if (classNameValue) {
        errors->setName("className");
        result->m_className = ValueConversions<String>::fromValue(classNameValue, errors);
    }
    protocol::Value* valueValue = object->get("value");
    if (valueValue) {
        errors->setName("value");
        result->m_value = ValueConversions<protocol::Value>::fromValue(valueValue, errors);
    }
    protocol::Value* unserializableValueValue = object->get("unserializableValue");
    if (unserializableValueValue) {
        errors->setName("unserializableValue");
        result->m_unserializableValue = ValueConversions<String>::fromValue(unserializableValueValue, errors);
    }
    protocol::Value* descriptionValue = object->get("description");
    if (descriptionValue) {
        errors->setName("description");
        result->m_description = ValueConversions<String>::fromValue(descriptionValue, errors);
    }
    protocol::Value* objectIdValue = object->get("objectId");
    if (objectIdValue) {
        errors->setName("objectId");
        result->m_objectId = ValueConversions<String>::fromValue(objectIdValue, errors);
    }
    protocol::Value* previewValue = object->get("preview");
    if (previewValue) {
        errors->setName("preview");
        result->m_preview = ValueConversions<protocol::Runtime::ObjectPreview>::fromValue(previewValue, errors);
    }
    protocol::Value* customPreviewValue = object->get("customPreview");
    if (customPreviewValue) {
        errors->setName("customPreview");
        result->m_customPreview = ValueConversions<protocol::Runtime::CustomPreview>::fromValue(customPreviewValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> RemoteObject::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    if (m_subtype.isJust())
        result->setValue("subtype", ValueConversions<String>::toValue(m_subtype.fromJust()));
    if (m_className.isJust())
        result->setValue("className", ValueConversions<String>::toValue(m_className.fromJust()));
    if (m_value.isJust())
        result->setValue("value", ValueConversions<protocol::Value>::toValue(m_value.fromJust()));
    if (m_unserializableValue.isJust())
        result->setValue("unserializableValue", ValueConversions<String>::toValue(m_unserializableValue.fromJust()));
    if (m_description.isJust())
        result->setValue("description", ValueConversions<String>::toValue(m_description.fromJust()));
    if (m_objectId.isJust())
        result->setValue("objectId", ValueConversions<String>::toValue(m_objectId.fromJust()));
    if (m_preview.isJust())
        result->setValue("preview", ValueConversions<protocol::Runtime::ObjectPreview>::toValue(m_preview.fromJust()));
    if (m_customPreview.isJust())
        result->setValue("customPreview", ValueConversions<protocol::Runtime::CustomPreview>::toValue(m_customPreview.fromJust()));
    return result;
}

std::unique_ptr<RemoteObject> RemoteObject::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StringBuffer> RemoteObject::toJSONString() const
{
    String json = toValue()->serialize();
    return StringBufferImpl::adopt(json);
}

// static
std::unique_ptr<API::RemoteObject> API::RemoteObject::fromJSONString(const StringView& json)
{
    ErrorSupport errors;
    std::unique_ptr<Value> value = StringUtil::parseJSON(json);
    if (!value)
        return nullptr;
    return protocol::Runtime::RemoteObject::fromValue(value.get(), &errors);
}

std::unique_ptr<CustomPreview> CustomPreview::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CustomPreview> result(new CustomPreview());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* headerValue = object->get("header");
    errors->setName("header");
    result->m_header = ValueConversions<String>::fromValue(headerValue, errors);
    protocol::Value* hasBodyValue = object->get("hasBody");
    errors->setName("hasBody");
    result->m_hasBody = ValueConversions<bool>::fromValue(hasBodyValue, errors);
    protocol::Value* formatterObjectIdValue = object->get("formatterObjectId");
    errors->setName("formatterObjectId");
    result->m_formatterObjectId = ValueConversions<String>::fromValue(formatterObjectIdValue, errors);
    protocol::Value* bindRemoteObjectFunctionIdValue = object->get("bindRemoteObjectFunctionId");
    errors->setName("bindRemoteObjectFunctionId");
    result->m_bindRemoteObjectFunctionId = ValueConversions<String>::fromValue(bindRemoteObjectFunctionIdValue, errors);
    protocol::Value* configObjectIdValue = object->get("configObjectId");
    if (configObjectIdValue) {
        errors->setName("configObjectId");
        result->m_configObjectId = ValueConversions<String>::fromValue(configObjectIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CustomPreview::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("header", ValueConversions<String>::toValue(m_header));
    result->setValue("hasBody", ValueConversions<bool>::toValue(m_hasBody));
    result->setValue("formatterObjectId", ValueConversions<String>::toValue(m_formatterObjectId));
    result->setValue("bindRemoteObjectFunctionId", ValueConversions<String>::toValue(m_bindRemoteObjectFunctionId));
    if (m_configObjectId.isJust())
        result->setValue("configObjectId", ValueConversions<String>::toValue(m_configObjectId.fromJust()));
    return result;
}

std::unique_ptr<CustomPreview> CustomPreview::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* ObjectPreview::TypeEnum::Object = "object";
const char* ObjectPreview::TypeEnum::Function = "function";
const char* ObjectPreview::TypeEnum::Undefined = "undefined";
const char* ObjectPreview::TypeEnum::String = "string";
const char* ObjectPreview::TypeEnum::Number = "number";
const char* ObjectPreview::TypeEnum::Boolean = "boolean";
const char* ObjectPreview::TypeEnum::Symbol = "symbol";

const char* ObjectPreview::SubtypeEnum::Array = "array";
const char* ObjectPreview::SubtypeEnum::Null = "null";
const char* ObjectPreview::SubtypeEnum::Node = "node";
const char* ObjectPreview::SubtypeEnum::Regexp = "regexp";
const char* ObjectPreview::SubtypeEnum::Date = "date";
const char* ObjectPreview::SubtypeEnum::Map = "map";
const char* ObjectPreview::SubtypeEnum::Set = "set";
const char* ObjectPreview::SubtypeEnum::Weakmap = "weakmap";
const char* ObjectPreview::SubtypeEnum::Weakset = "weakset";
const char* ObjectPreview::SubtypeEnum::Iterator = "iterator";
const char* ObjectPreview::SubtypeEnum::Generator = "generator";
const char* ObjectPreview::SubtypeEnum::Error = "error";

std::unique_ptr<ObjectPreview> ObjectPreview::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ObjectPreview> result(new ObjectPreview());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* subtypeValue = object->get("subtype");
    if (subtypeValue) {
        errors->setName("subtype");
        result->m_subtype = ValueConversions<String>::fromValue(subtypeValue, errors);
    }
    protocol::Value* descriptionValue = object->get("description");
    if (descriptionValue) {
        errors->setName("description");
        result->m_description = ValueConversions<String>::fromValue(descriptionValue, errors);
    }
    protocol::Value* overflowValue = object->get("overflow");
    errors->setName("overflow");
    result->m_overflow = ValueConversions<bool>::fromValue(overflowValue, errors);
    protocol::Value* propertiesValue = object->get("properties");
    errors->setName("properties");
    result->m_properties = ValueConversions<protocol::Array<protocol::Runtime::PropertyPreview>>::fromValue(propertiesValue, errors);
    protocol::Value* entriesValue = object->get("entries");
    if (entriesValue) {
        errors->setName("entries");
        result->m_entries = ValueConversions<protocol::Array<protocol::Runtime::EntryPreview>>::fromValue(entriesValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ObjectPreview::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    if (m_subtype.isJust())
        result->setValue("subtype", ValueConversions<String>::toValue(m_subtype.fromJust()));
    if (m_description.isJust())
        result->setValue("description", ValueConversions<String>::toValue(m_description.fromJust()));
    result->setValue("overflow", ValueConversions<bool>::toValue(m_overflow));
    result->setValue("properties", ValueConversions<protocol::Array<protocol::Runtime::PropertyPreview>>::toValue(m_properties.get()));
    if (m_entries.isJust())
        result->setValue("entries", ValueConversions<protocol::Array<protocol::Runtime::EntryPreview>>::toValue(m_entries.fromJust()));
    return result;
}

std::unique_ptr<ObjectPreview> ObjectPreview::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

const char* PropertyPreview::TypeEnum::Object = "object";
const char* PropertyPreview::TypeEnum::Function = "function";
const char* PropertyPreview::TypeEnum::Undefined = "undefined";
const char* PropertyPreview::TypeEnum::String = "string";
const char* PropertyPreview::TypeEnum::Number = "number";
const char* PropertyPreview::TypeEnum::Boolean = "boolean";
const char* PropertyPreview::TypeEnum::Symbol = "symbol";
const char* PropertyPreview::TypeEnum::Accessor = "accessor";

const char* PropertyPreview::SubtypeEnum::Array = "array";
const char* PropertyPreview::SubtypeEnum::Null = "null";
const char* PropertyPreview::SubtypeEnum::Node = "node";
const char* PropertyPreview::SubtypeEnum::Regexp = "regexp";
const char* PropertyPreview::SubtypeEnum::Date = "date";
const char* PropertyPreview::SubtypeEnum::Map = "map";
const char* PropertyPreview::SubtypeEnum::Set = "set";
const char* PropertyPreview::SubtypeEnum::Weakmap = "weakmap";
const char* PropertyPreview::SubtypeEnum::Weakset = "weakset";
const char* PropertyPreview::SubtypeEnum::Iterator = "iterator";
const char* PropertyPreview::SubtypeEnum::Generator = "generator";
const char* PropertyPreview::SubtypeEnum::Error = "error";

std::unique_ptr<PropertyPreview> PropertyPreview::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PropertyPreview> result(new PropertyPreview());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* valueValue = object->get("value");
    if (valueValue) {
        errors->setName("value");
        result->m_value = ValueConversions<String>::fromValue(valueValue, errors);
    }
    protocol::Value* valuePreviewValue = object->get("valuePreview");
    if (valuePreviewValue) {
        errors->setName("valuePreview");
        result->m_valuePreview = ValueConversions<protocol::Runtime::ObjectPreview>::fromValue(valuePreviewValue, errors);
    }
    protocol::Value* subtypeValue = object->get("subtype");
    if (subtypeValue) {
        errors->setName("subtype");
        result->m_subtype = ValueConversions<String>::fromValue(subtypeValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PropertyPreview::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    if (m_value.isJust())
        result->setValue("value", ValueConversions<String>::toValue(m_value.fromJust()));
    if (m_valuePreview.isJust())
        result->setValue("valuePreview", ValueConversions<protocol::Runtime::ObjectPreview>::toValue(m_valuePreview.fromJust()));
    if (m_subtype.isJust())
        result->setValue("subtype", ValueConversions<String>::toValue(m_subtype.fromJust()));
    return result;
}

std::unique_ptr<PropertyPreview> PropertyPreview::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<EntryPreview> EntryPreview::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<EntryPreview> result(new EntryPreview());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* keyValue = object->get("key");
    if (keyValue) {
        errors->setName("key");
        result->m_key = ValueConversions<protocol::Runtime::ObjectPreview>::fromValue(keyValue, errors);
    }
    protocol::Value* valueValue = object->get("value");
    errors->setName("value");
    result->m_value = ValueConversions<protocol::Runtime::ObjectPreview>::fromValue(valueValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> EntryPreview::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_key.isJust())
        result->setValue("key", ValueConversions<protocol::Runtime::ObjectPreview>::toValue(m_key.fromJust()));
    result->setValue("value", ValueConversions<protocol::Runtime::ObjectPreview>::toValue(m_value.get()));
    return result;
}

std::unique_ptr<EntryPreview> EntryPreview::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<PropertyDescriptor> PropertyDescriptor::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<PropertyDescriptor> result(new PropertyDescriptor());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    if (valueValue) {
        errors->setName("value");
        result->m_value = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(valueValue, errors);
    }
    protocol::Value* writableValue = object->get("writable");
    if (writableValue) {
        errors->setName("writable");
        result->m_writable = ValueConversions<bool>::fromValue(writableValue, errors);
    }
    protocol::Value* getValue = object->get("get");
    if (getValue) {
        errors->setName("get");
        result->m_get = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(getValue, errors);
    }
    protocol::Value* setValue = object->get("set");
    if (setValue) {
        errors->setName("set");
        result->m_set = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(setValue, errors);
    }
    protocol::Value* configurableValue = object->get("configurable");
    errors->setName("configurable");
    result->m_configurable = ValueConversions<bool>::fromValue(configurableValue, errors);
    protocol::Value* enumerableValue = object->get("enumerable");
    errors->setName("enumerable");
    result->m_enumerable = ValueConversions<bool>::fromValue(enumerableValue, errors);
    protocol::Value* wasThrownValue = object->get("wasThrown");
    if (wasThrownValue) {
        errors->setName("wasThrown");
        result->m_wasThrown = ValueConversions<bool>::fromValue(wasThrownValue, errors);
    }
    protocol::Value* isOwnValue = object->get("isOwn");
    if (isOwnValue) {
        errors->setName("isOwn");
        result->m_isOwn = ValueConversions<bool>::fromValue(isOwnValue, errors);
    }
    protocol::Value* symbolValue = object->get("symbol");
    if (symbolValue) {
        errors->setName("symbol");
        result->m_symbol = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(symbolValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> PropertyDescriptor::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    if (m_value.isJust())
        result->setValue("value", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_value.fromJust()));
    if (m_writable.isJust())
        result->setValue("writable", ValueConversions<bool>::toValue(m_writable.fromJust()));
    if (m_get.isJust())
        result->setValue("get", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_get.fromJust()));
    if (m_set.isJust())
        result->setValue("set", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_set.fromJust()));
    result->setValue("configurable", ValueConversions<bool>::toValue(m_configurable));
    result->setValue("enumerable", ValueConversions<bool>::toValue(m_enumerable));
    if (m_wasThrown.isJust())
        result->setValue("wasThrown", ValueConversions<bool>::toValue(m_wasThrown.fromJust()));
    if (m_isOwn.isJust())
        result->setValue("isOwn", ValueConversions<bool>::toValue(m_isOwn.fromJust()));
    if (m_symbol.isJust())
        result->setValue("symbol", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_symbol.fromJust()));
    return result;
}

std::unique_ptr<PropertyDescriptor> PropertyDescriptor::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<InternalPropertyDescriptor> InternalPropertyDescriptor::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<InternalPropertyDescriptor> result(new InternalPropertyDescriptor());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* valueValue = object->get("value");
    if (valueValue) {
        errors->setName("value");
        result->m_value = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(valueValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> InternalPropertyDescriptor::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    if (m_value.isJust())
        result->setValue("value", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_value.fromJust()));
    return result;
}

std::unique_ptr<InternalPropertyDescriptor> InternalPropertyDescriptor::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CallArgument> CallArgument::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CallArgument> result(new CallArgument());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* valueValue = object->get("value");
    if (valueValue) {
        errors->setName("value");
        result->m_value = ValueConversions<protocol::Value>::fromValue(valueValue, errors);
    }
    protocol::Value* unserializableValueValue = object->get("unserializableValue");
    if (unserializableValueValue) {
        errors->setName("unserializableValue");
        result->m_unserializableValue = ValueConversions<String>::fromValue(unserializableValueValue, errors);
    }
    protocol::Value* objectIdValue = object->get("objectId");
    if (objectIdValue) {
        errors->setName("objectId");
        result->m_objectId = ValueConversions<String>::fromValue(objectIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CallArgument::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_value.isJust())
        result->setValue("value", ValueConversions<protocol::Value>::toValue(m_value.fromJust()));
    if (m_unserializableValue.isJust())
        result->setValue("unserializableValue", ValueConversions<String>::toValue(m_unserializableValue.fromJust()));
    if (m_objectId.isJust())
        result->setValue("objectId", ValueConversions<String>::toValue(m_objectId.fromJust()));
    return result;
}

std::unique_ptr<CallArgument> CallArgument::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ExecutionContextDescription> ExecutionContextDescription::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ExecutionContextDescription> result(new ExecutionContextDescription());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<int>::fromValue(idValue, errors);
    protocol::Value* originValue = object->get("origin");
    errors->setName("origin");
    result->m_origin = ValueConversions<String>::fromValue(originValue, errors);
    protocol::Value* nameValue = object->get("name");
    errors->setName("name");
    result->m_name = ValueConversions<String>::fromValue(nameValue, errors);
    protocol::Value* auxDataValue = object->get("auxData");
    if (auxDataValue) {
        errors->setName("auxData");
        result->m_auxData = ValueConversions<protocol::DictionaryValue>::fromValue(auxDataValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ExecutionContextDescription::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<int>::toValue(m_id));
    result->setValue("origin", ValueConversions<String>::toValue(m_origin));
    result->setValue("name", ValueConversions<String>::toValue(m_name));
    if (m_auxData.isJust())
        result->setValue("auxData", ValueConversions<protocol::DictionaryValue>::toValue(m_auxData.fromJust()));
    return result;
}

std::unique_ptr<ExecutionContextDescription> ExecutionContextDescription::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ExceptionDetails> ExceptionDetails::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ExceptionDetails> result(new ExceptionDetails());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* exceptionIdValue = object->get("exceptionId");
    errors->setName("exceptionId");
    result->m_exceptionId = ValueConversions<int>::fromValue(exceptionIdValue, errors);
    protocol::Value* textValue = object->get("text");
    errors->setName("text");
    result->m_text = ValueConversions<String>::fromValue(textValue, errors);
    protocol::Value* lineNumberValue = object->get("lineNumber");
    errors->setName("lineNumber");
    result->m_lineNumber = ValueConversions<int>::fromValue(lineNumberValue, errors);
    protocol::Value* columnNumberValue = object->get("columnNumber");
    errors->setName("columnNumber");
    result->m_columnNumber = ValueConversions<int>::fromValue(columnNumberValue, errors);
    protocol::Value* scriptIdValue = object->get("scriptId");
    if (scriptIdValue) {
        errors->setName("scriptId");
        result->m_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    }
    protocol::Value* urlValue = object->get("url");
    if (urlValue) {
        errors->setName("url");
        result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    }
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::fromValue(stackTraceValue, errors);
    }
    protocol::Value* exceptionValue = object->get("exception");
    if (exceptionValue) {
        errors->setName("exception");
        result->m_exception = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(exceptionValue, errors);
    }
    protocol::Value* executionContextIdValue = object->get("executionContextId");
    if (executionContextIdValue) {
        errors->setName("executionContextId");
        result->m_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ExceptionDetails::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("exceptionId", ValueConversions<int>::toValue(m_exceptionId));
    result->setValue("text", ValueConversions<String>::toValue(m_text));
    result->setValue("lineNumber", ValueConversions<int>::toValue(m_lineNumber));
    result->setValue("columnNumber", ValueConversions<int>::toValue(m_columnNumber));
    if (m_scriptId.isJust())
        result->setValue("scriptId", ValueConversions<String>::toValue(m_scriptId.fromJust()));
    if (m_url.isJust())
        result->setValue("url", ValueConversions<String>::toValue(m_url.fromJust()));
    if (m_stackTrace.isJust())
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_stackTrace.fromJust()));
    if (m_exception.isJust())
        result->setValue("exception", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_exception.fromJust()));
    if (m_executionContextId.isJust())
        result->setValue("executionContextId", ValueConversions<int>::toValue(m_executionContextId.fromJust()));
    return result;
}

std::unique_ptr<ExceptionDetails> ExceptionDetails::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<CallFrame> CallFrame::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<CallFrame> result(new CallFrame());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* functionNameValue = object->get("functionName");
    errors->setName("functionName");
    result->m_functionName = ValueConversions<String>::fromValue(functionNameValue, errors);
    protocol::Value* scriptIdValue = object->get("scriptId");
    errors->setName("scriptId");
    result->m_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* urlValue = object->get("url");
    errors->setName("url");
    result->m_url = ValueConversions<String>::fromValue(urlValue, errors);
    protocol::Value* lineNumberValue = object->get("lineNumber");
    errors->setName("lineNumber");
    result->m_lineNumber = ValueConversions<int>::fromValue(lineNumberValue, errors);
    protocol::Value* columnNumberValue = object->get("columnNumber");
    errors->setName("columnNumber");
    result->m_columnNumber = ValueConversions<int>::fromValue(columnNumberValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> CallFrame::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("functionName", ValueConversions<String>::toValue(m_functionName));
    result->setValue("scriptId", ValueConversions<String>::toValue(m_scriptId));
    result->setValue("url", ValueConversions<String>::toValue(m_url));
    result->setValue("lineNumber", ValueConversions<int>::toValue(m_lineNumber));
    result->setValue("columnNumber", ValueConversions<int>::toValue(m_columnNumber));
    return result;
}

std::unique_ptr<CallFrame> CallFrame::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StackTrace> StackTrace::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<StackTrace> result(new StackTrace());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* descriptionValue = object->get("description");
    if (descriptionValue) {
        errors->setName("description");
        result->m_description = ValueConversions<String>::fromValue(descriptionValue, errors);
    }
    protocol::Value* callFramesValue = object->get("callFrames");
    errors->setName("callFrames");
    result->m_callFrames = ValueConversions<protocol::Array<protocol::Runtime::CallFrame>>::fromValue(callFramesValue, errors);
    protocol::Value* parentValue = object->get("parent");
    if (parentValue) {
        errors->setName("parent");
        result->m_parent = ValueConversions<protocol::Runtime::StackTrace>::fromValue(parentValue, errors);
    }
    protocol::Value* parentIdValue = object->get("parentId");
    if (parentIdValue) {
        errors->setName("parentId");
        result->m_parentId = ValueConversions<protocol::Runtime::StackTraceId>::fromValue(parentIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> StackTrace::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (m_description.isJust())
        result->setValue("description", ValueConversions<String>::toValue(m_description.fromJust()));
    result->setValue("callFrames", ValueConversions<protocol::Array<protocol::Runtime::CallFrame>>::toValue(m_callFrames.get()));
    if (m_parent.isJust())
        result->setValue("parent", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_parent.fromJust()));
    if (m_parentId.isJust())
        result->setValue("parentId", ValueConversions<protocol::Runtime::StackTraceId>::toValue(m_parentId.fromJust()));
    return result;
}

std::unique_ptr<StackTrace> StackTrace::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StringBuffer> StackTrace::toJSONString() const
{
    String json = toValue()->serialize();
    return StringBufferImpl::adopt(json);
}

// static
std::unique_ptr<API::StackTrace> API::StackTrace::fromJSONString(const StringView& json)
{
    ErrorSupport errors;
    std::unique_ptr<Value> value = StringUtil::parseJSON(json);
    if (!value)
        return nullptr;
    return protocol::Runtime::StackTrace::fromValue(value.get(), &errors);
}

std::unique_ptr<StackTraceId> StackTraceId::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<StackTraceId> result(new StackTraceId());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* idValue = object->get("id");
    errors->setName("id");
    result->m_id = ValueConversions<String>::fromValue(idValue, errors);
    protocol::Value* debuggerIdValue = object->get("debuggerId");
    if (debuggerIdValue) {
        errors->setName("debuggerId");
        result->m_debuggerId = ValueConversions<String>::fromValue(debuggerIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> StackTraceId::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("id", ValueConversions<String>::toValue(m_id));
    if (m_debuggerId.isJust())
        result->setValue("debuggerId", ValueConversions<String>::toValue(m_debuggerId.fromJust()));
    return result;
}

std::unique_ptr<StackTraceId> StackTraceId::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<StringBuffer> StackTraceId::toJSONString() const
{
    String json = toValue()->serialize();
    return StringBufferImpl::adopt(json);
}

// static
std::unique_ptr<API::StackTraceId> API::StackTraceId::fromJSONString(const StringView& json)
{
    ErrorSupport errors;
    std::unique_ptr<Value> value = StringUtil::parseJSON(json);
    if (!value)
        return nullptr;
    return protocol::Runtime::StackTraceId::fromValue(value.get(), &errors);
}

const char* ConsoleAPICalledNotification::TypeEnum::Log = "log";
const char* ConsoleAPICalledNotification::TypeEnum::Debug = "debug";
const char* ConsoleAPICalledNotification::TypeEnum::Info = "info";
const char* ConsoleAPICalledNotification::TypeEnum::Error = "error";
const char* ConsoleAPICalledNotification::TypeEnum::Warning = "warning";
const char* ConsoleAPICalledNotification::TypeEnum::Dir = "dir";
const char* ConsoleAPICalledNotification::TypeEnum::Dirxml = "dirxml";
const char* ConsoleAPICalledNotification::TypeEnum::Table = "table";
const char* ConsoleAPICalledNotification::TypeEnum::Trace = "trace";
const char* ConsoleAPICalledNotification::TypeEnum::Clear = "clear";
const char* ConsoleAPICalledNotification::TypeEnum::StartGroup = "startGroup";
const char* ConsoleAPICalledNotification::TypeEnum::StartGroupCollapsed = "startGroupCollapsed";
const char* ConsoleAPICalledNotification::TypeEnum::EndGroup = "endGroup";
const char* ConsoleAPICalledNotification::TypeEnum::Assert = "assert";
const char* ConsoleAPICalledNotification::TypeEnum::Profile = "profile";
const char* ConsoleAPICalledNotification::TypeEnum::ProfileEnd = "profileEnd";
const char* ConsoleAPICalledNotification::TypeEnum::Count = "count";
const char* ConsoleAPICalledNotification::TypeEnum::TimeEnd = "timeEnd";

std::unique_ptr<ConsoleAPICalledNotification> ConsoleAPICalledNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ConsoleAPICalledNotification> result(new ConsoleAPICalledNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* typeValue = object->get("type");
    errors->setName("type");
    result->m_type = ValueConversions<String>::fromValue(typeValue, errors);
    protocol::Value* argsValue = object->get("args");
    errors->setName("args");
    result->m_args = ValueConversions<protocol::Array<protocol::Runtime::RemoteObject>>::fromValue(argsValue, errors);
    protocol::Value* executionContextIdValue = object->get("executionContextId");
    errors->setName("executionContextId");
    result->m_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* stackTraceValue = object->get("stackTrace");
    if (stackTraceValue) {
        errors->setName("stackTrace");
        result->m_stackTrace = ValueConversions<protocol::Runtime::StackTrace>::fromValue(stackTraceValue, errors);
    }
    protocol::Value* contextValue = object->get("context");
    if (contextValue) {
        errors->setName("context");
        result->m_context = ValueConversions<String>::fromValue(contextValue, errors);
    }
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ConsoleAPICalledNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("type", ValueConversions<String>::toValue(m_type));
    result->setValue("args", ValueConversions<protocol::Array<protocol::Runtime::RemoteObject>>::toValue(m_args.get()));
    result->setValue("executionContextId", ValueConversions<int>::toValue(m_executionContextId));
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    if (m_stackTrace.isJust())
        result->setValue("stackTrace", ValueConversions<protocol::Runtime::StackTrace>::toValue(m_stackTrace.fromJust()));
    if (m_context.isJust())
        result->setValue("context", ValueConversions<String>::toValue(m_context.fromJust()));
    return result;
}

std::unique_ptr<ConsoleAPICalledNotification> ConsoleAPICalledNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ExceptionRevokedNotification> ExceptionRevokedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ExceptionRevokedNotification> result(new ExceptionRevokedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* reasonValue = object->get("reason");
    errors->setName("reason");
    result->m_reason = ValueConversions<String>::fromValue(reasonValue, errors);
    protocol::Value* exceptionIdValue = object->get("exceptionId");
    errors->setName("exceptionId");
    result->m_exceptionId = ValueConversions<int>::fromValue(exceptionIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ExceptionRevokedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("reason", ValueConversions<String>::toValue(m_reason));
    result->setValue("exceptionId", ValueConversions<int>::toValue(m_exceptionId));
    return result;
}

std::unique_ptr<ExceptionRevokedNotification> ExceptionRevokedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ExceptionThrownNotification> ExceptionThrownNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ExceptionThrownNotification> result(new ExceptionThrownNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* timestampValue = object->get("timestamp");
    errors->setName("timestamp");
    result->m_timestamp = ValueConversions<double>::fromValue(timestampValue, errors);
    protocol::Value* exceptionDetailsValue = object->get("exceptionDetails");
    errors->setName("exceptionDetails");
    result->m_exceptionDetails = ValueConversions<protocol::Runtime::ExceptionDetails>::fromValue(exceptionDetailsValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ExceptionThrownNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("timestamp", ValueConversions<double>::toValue(m_timestamp));
    result->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(m_exceptionDetails.get()));
    return result;
}

std::unique_ptr<ExceptionThrownNotification> ExceptionThrownNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ExecutionContextCreatedNotification> ExecutionContextCreatedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ExecutionContextCreatedNotification> result(new ExecutionContextCreatedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* contextValue = object->get("context");
    errors->setName("context");
    result->m_context = ValueConversions<protocol::Runtime::ExecutionContextDescription>::fromValue(contextValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ExecutionContextCreatedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("context", ValueConversions<protocol::Runtime::ExecutionContextDescription>::toValue(m_context.get()));
    return result;
}

std::unique_ptr<ExecutionContextCreatedNotification> ExecutionContextCreatedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<ExecutionContextDestroyedNotification> ExecutionContextDestroyedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<ExecutionContextDestroyedNotification> result(new ExecutionContextDestroyedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* executionContextIdValue = object->get("executionContextId");
    errors->setName("executionContextId");
    result->m_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> ExecutionContextDestroyedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("executionContextId", ValueConversions<int>::toValue(m_executionContextId));
    return result;
}

std::unique_ptr<ExecutionContextDestroyedNotification> ExecutionContextDestroyedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

std::unique_ptr<InspectRequestedNotification> InspectRequestedNotification::fromValue(protocol::Value* value, ErrorSupport* errors)
{
    if (!value || value->type() != protocol::Value::TypeObject) {
        errors->addError("object expected");
        return nullptr;
    }

    std::unique_ptr<InspectRequestedNotification> result(new InspectRequestedNotification());
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    errors->push();
    protocol::Value* objectValue = object->get("object");
    errors->setName("object");
    result->m_object = ValueConversions<protocol::Runtime::RemoteObject>::fromValue(objectValue, errors);
    protocol::Value* hintsValue = object->get("hints");
    errors->setName("hints");
    result->m_hints = ValueConversions<protocol::DictionaryValue>::fromValue(hintsValue, errors);
    errors->pop();
    if (errors->hasErrors())
        return nullptr;
    return result;
}

std::unique_ptr<protocol::DictionaryValue> InspectRequestedNotification::toValue() const
{
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    result->setValue("object", ValueConversions<protocol::Runtime::RemoteObject>::toValue(m_object.get()));
    result->setValue("hints", ValueConversions<protocol::DictionaryValue>::toValue(m_hints.get()));
    return result;
}

std::unique_ptr<InspectRequestedNotification> InspectRequestedNotification::clone() const
{
    ErrorSupport errors;
    return fromValue(toValue().get(), &errors);
}

// ------------- Enum values from params.


namespace ConsoleAPICalled {
namespace TypeEnum {
const char* Log = "log";
const char* Debug = "debug";
const char* Info = "info";
const char* Error = "error";
const char* Warning = "warning";
const char* Dir = "dir";
const char* Dirxml = "dirxml";
const char* Table = "table";
const char* Trace = "trace";
const char* Clear = "clear";
const char* StartGroup = "startGroup";
const char* StartGroupCollapsed = "startGroupCollapsed";
const char* EndGroup = "endGroup";
const char* Assert = "assert";
const char* Profile = "profile";
const char* ProfileEnd = "profileEnd";
const char* Count = "count";
const char* TimeEnd = "timeEnd";
} // namespace TypeEnum
} // namespace ConsoleAPICalled

// ------------- Frontend notifications.

void Frontend::consoleAPICalled(const String& type, std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> args, int executionContextId, double timestamp, Maybe<protocol::Runtime::StackTrace> stackTrace, Maybe<String> context)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ConsoleAPICalledNotification> messageData = ConsoleAPICalledNotification::create()
        .setType(type)
        .setArgs(std::move(args))
        .setExecutionContextId(executionContextId)
        .setTimestamp(timestamp)
        .build();
    if (stackTrace.isJust())
        messageData->setStackTrace(std::move(stackTrace).takeJust());
    if (context.isJust())
        messageData->setContext(std::move(context).takeJust());
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Runtime.consoleAPICalled", std::move(messageData)));
}

void Frontend::exceptionRevoked(const String& reason, int exceptionId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ExceptionRevokedNotification> messageData = ExceptionRevokedNotification::create()
        .setReason(reason)
        .setExceptionId(exceptionId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Runtime.exceptionRevoked", std::move(messageData)));
}

void Frontend::exceptionThrown(double timestamp, std::unique_ptr<protocol::Runtime::ExceptionDetails> exceptionDetails)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ExceptionThrownNotification> messageData = ExceptionThrownNotification::create()
        .setTimestamp(timestamp)
        .setExceptionDetails(std::move(exceptionDetails))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Runtime.exceptionThrown", std::move(messageData)));
}

void Frontend::executionContextCreated(std::unique_ptr<protocol::Runtime::ExecutionContextDescription> context)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ExecutionContextCreatedNotification> messageData = ExecutionContextCreatedNotification::create()
        .setContext(std::move(context))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Runtime.executionContextCreated", std::move(messageData)));
}

void Frontend::executionContextDestroyed(int executionContextId)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<ExecutionContextDestroyedNotification> messageData = ExecutionContextDestroyedNotification::create()
        .setExecutionContextId(executionContextId)
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Runtime.executionContextDestroyed", std::move(messageData)));
}

void Frontend::executionContextsCleared()
{
    if (!m_frontendChannel)
        return;
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Runtime.executionContextsCleared"));
}

void Frontend::inspectRequested(std::unique_ptr<protocol::Runtime::RemoteObject> object, std::unique_ptr<protocol::DictionaryValue> hints)
{
    if (!m_frontendChannel)
        return;
    std::unique_ptr<InspectRequestedNotification> messageData = InspectRequestedNotification::create()
        .setObject(std::move(object))
        .setHints(std::move(hints))
        .build();
    m_frontendChannel->sendProtocolNotification(InternalResponse::createNotification("Runtime.inspectRequested", std::move(messageData)));
}

void Frontend::flush()
{
    m_frontendChannel->flushProtocolNotifications();
}

void Frontend::sendRawNotification(const String& notification)
{
    m_frontendChannel->sendProtocolNotification(InternalRawNotification::create(notification));
}

// --------------------- Dispatcher.

class DispatcherImpl : public protocol::DispatcherBase {
public:
    DispatcherImpl(FrontendChannel* frontendChannel, Backend* backend, bool fallThroughForNotFound)
        : DispatcherBase(frontendChannel)
        , m_backend(backend)
        , m_fallThroughForNotFound(fallThroughForNotFound) {
        m_dispatchMap["Runtime.awaitPromise"] = &DispatcherImpl::awaitPromise;
        m_dispatchMap["Runtime.callFunctionOn"] = &DispatcherImpl::callFunctionOn;
        m_dispatchMap["Runtime.compileScript"] = &DispatcherImpl::compileScript;
        m_dispatchMap["Runtime.disable"] = &DispatcherImpl::disable;
        m_dispatchMap["Runtime.discardConsoleEntries"] = &DispatcherImpl::discardConsoleEntries;
        m_dispatchMap["Runtime.enable"] = &DispatcherImpl::enable;
        m_dispatchMap["Runtime.evaluate"] = &DispatcherImpl::evaluate;
        m_dispatchMap["Runtime.getProperties"] = &DispatcherImpl::getProperties;
        m_dispatchMap["Runtime.globalLexicalScopeNames"] = &DispatcherImpl::globalLexicalScopeNames;
        m_dispatchMap["Runtime.queryObjects"] = &DispatcherImpl::queryObjects;
        m_dispatchMap["Runtime.releaseObject"] = &DispatcherImpl::releaseObject;
        m_dispatchMap["Runtime.releaseObjectGroup"] = &DispatcherImpl::releaseObjectGroup;
        m_dispatchMap["Runtime.runIfWaitingForDebugger"] = &DispatcherImpl::runIfWaitingForDebugger;
        m_dispatchMap["Runtime.runScript"] = &DispatcherImpl::runScript;
        m_dispatchMap["Runtime.setCustomObjectFormatterEnabled"] = &DispatcherImpl::setCustomObjectFormatterEnabled;
    }
    ~DispatcherImpl() override { }
    DispatchResponse::Status dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) override;
    HashMap<String, String>& redirects() { return m_redirects; }

protected:
    using CallHandler = DispatchResponse::Status (DispatcherImpl::*)(int callId, std::unique_ptr<DictionaryValue> messageObject, ErrorSupport* errors);
    using DispatchMap = protocol::HashMap<String, CallHandler>;
    DispatchMap m_dispatchMap;
    HashMap<String, String> m_redirects;

    DispatchResponse::Status awaitPromise(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status callFunctionOn(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status compileScript(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status discardConsoleEntries(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status evaluate(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status getProperties(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status globalLexicalScopeNames(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status queryObjects(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status releaseObject(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status releaseObjectGroup(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status runIfWaitingForDebugger(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status runScript(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);
    DispatchResponse::Status setCustomObjectFormatterEnabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport*);

    Backend* m_backend;
    bool m_fallThroughForNotFound;
};

DispatchResponse::Status DispatcherImpl::dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject)
{
    protocol::HashMap<String, CallHandler>::iterator it = m_dispatchMap.find(method);
    if (it == m_dispatchMap.end()) {
        if (m_fallThroughForNotFound)
            return DispatchResponse::kFallThrough;
        reportProtocolError(callId, DispatchResponse::kMethodNotFound, "'" + method + "' wasn't found", nullptr);
        return DispatchResponse::kError;
    }

    protocol::ErrorSupport errors;
    return (this->*(it->second))(callId, std::move(messageObject), &errors);
}


class AwaitPromiseCallbackImpl : public Backend::AwaitPromiseCallback, public DispatcherBase::Callback {
public:
    AwaitPromiseCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, int callbackId)
        : DispatcherBase::Callback(std::move(backendImpl), callId, callbackId) { }

    void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        resultObject->setValue("result", ValueConversions<protocol::Runtime::RemoteObject>::toValue(result.get()));
        if (exceptionDetails.isJust())
            resultObject->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(exceptionDetails.fromJust()));
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

DispatchResponse::Status DispatcherImpl::awaitPromise(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* promiseObjectIdValue = object ? object->get("promiseObjectId") : nullptr;
    errors->setName("promiseObjectId");
    String in_promiseObjectId = ValueConversions<String>::fromValue(promiseObjectIdValue, errors);
    protocol::Value* returnByValueValue = object ? object->get("returnByValue") : nullptr;
    Maybe<bool> in_returnByValue;
    if (returnByValueValue) {
        errors->setName("returnByValue");
        in_returnByValue = ValueConversions<bool>::fromValue(returnByValueValue, errors);
    }
    protocol::Value* generatePreviewValue = object ? object->get("generatePreview") : nullptr;
    Maybe<bool> in_generatePreview;
    if (generatePreviewValue) {
        errors->setName("generatePreview");
        in_generatePreview = ValueConversions<bool>::fromValue(generatePreviewValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<AwaitPromiseCallbackImpl> callback(new AwaitPromiseCallbackImpl(weakPtr(), callId, nextCallbackId()));
    m_backend->awaitPromise(in_promiseObjectId, std::move(in_returnByValue), std::move(in_generatePreview), std::move(callback));
    return (weak->get() && weak->get()->lastCallbackFallThrough()) ? DispatchResponse::kFallThrough : DispatchResponse::kAsync;
}

class CallFunctionOnCallbackImpl : public Backend::CallFunctionOnCallback, public DispatcherBase::Callback {
public:
    CallFunctionOnCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, int callbackId)
        : DispatcherBase::Callback(std::move(backendImpl), callId, callbackId) { }

    void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        resultObject->setValue("result", ValueConversions<protocol::Runtime::RemoteObject>::toValue(result.get()));
        if (exceptionDetails.isJust())
            resultObject->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(exceptionDetails.fromJust()));
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

DispatchResponse::Status DispatcherImpl::callFunctionOn(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* functionDeclarationValue = object ? object->get("functionDeclaration") : nullptr;
    errors->setName("functionDeclaration");
    String in_functionDeclaration = ValueConversions<String>::fromValue(functionDeclarationValue, errors);
    protocol::Value* objectIdValue = object ? object->get("objectId") : nullptr;
    Maybe<String> in_objectId;
    if (objectIdValue) {
        errors->setName("objectId");
        in_objectId = ValueConversions<String>::fromValue(objectIdValue, errors);
    }
    protocol::Value* argumentsValue = object ? object->get("arguments") : nullptr;
    Maybe<protocol::Array<protocol::Runtime::CallArgument>> in_arguments;
    if (argumentsValue) {
        errors->setName("arguments");
        in_arguments = ValueConversions<protocol::Array<protocol::Runtime::CallArgument>>::fromValue(argumentsValue, errors);
    }
    protocol::Value* silentValue = object ? object->get("silent") : nullptr;
    Maybe<bool> in_silent;
    if (silentValue) {
        errors->setName("silent");
        in_silent = ValueConversions<bool>::fromValue(silentValue, errors);
    }
    protocol::Value* returnByValueValue = object ? object->get("returnByValue") : nullptr;
    Maybe<bool> in_returnByValue;
    if (returnByValueValue) {
        errors->setName("returnByValue");
        in_returnByValue = ValueConversions<bool>::fromValue(returnByValueValue, errors);
    }
    protocol::Value* generatePreviewValue = object ? object->get("generatePreview") : nullptr;
    Maybe<bool> in_generatePreview;
    if (generatePreviewValue) {
        errors->setName("generatePreview");
        in_generatePreview = ValueConversions<bool>::fromValue(generatePreviewValue, errors);
    }
    protocol::Value* userGestureValue = object ? object->get("userGesture") : nullptr;
    Maybe<bool> in_userGesture;
    if (userGestureValue) {
        errors->setName("userGesture");
        in_userGesture = ValueConversions<bool>::fromValue(userGestureValue, errors);
    }
    protocol::Value* awaitPromiseValue = object ? object->get("awaitPromise") : nullptr;
    Maybe<bool> in_awaitPromise;
    if (awaitPromiseValue) {
        errors->setName("awaitPromise");
        in_awaitPromise = ValueConversions<bool>::fromValue(awaitPromiseValue, errors);
    }
    protocol::Value* executionContextIdValue = object ? object->get("executionContextId") : nullptr;
    Maybe<int> in_executionContextId;
    if (executionContextIdValue) {
        errors->setName("executionContextId");
        in_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    }
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    Maybe<String> in_objectGroup;
    if (objectGroupValue) {
        errors->setName("objectGroup");
        in_objectGroup = ValueConversions<String>::fromValue(objectGroupValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<CallFunctionOnCallbackImpl> callback(new CallFunctionOnCallbackImpl(weakPtr(), callId, nextCallbackId()));
    m_backend->callFunctionOn(in_functionDeclaration, std::move(in_objectId), std::move(in_arguments), std::move(in_silent), std::move(in_returnByValue), std::move(in_generatePreview), std::move(in_userGesture), std::move(in_awaitPromise), std::move(in_executionContextId), std::move(in_objectGroup), std::move(callback));
    return (weak->get() && weak->get()->lastCallbackFallThrough()) ? DispatchResponse::kFallThrough : DispatchResponse::kAsync;
}

DispatchResponse::Status DispatcherImpl::compileScript(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* expressionValue = object ? object->get("expression") : nullptr;
    errors->setName("expression");
    String in_expression = ValueConversions<String>::fromValue(expressionValue, errors);
    protocol::Value* sourceURLValue = object ? object->get("sourceURL") : nullptr;
    errors->setName("sourceURL");
    String in_sourceURL = ValueConversions<String>::fromValue(sourceURLValue, errors);
    protocol::Value* persistScriptValue = object ? object->get("persistScript") : nullptr;
    errors->setName("persistScript");
    bool in_persistScript = ValueConversions<bool>::fromValue(persistScriptValue, errors);
    protocol::Value* executionContextIdValue = object ? object->get("executionContextId") : nullptr;
    Maybe<int> in_executionContextId;
    if (executionContextIdValue) {
        errors->setName("executionContextId");
        in_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    Maybe<String> out_scriptId;
    Maybe<protocol::Runtime::ExceptionDetails> out_exceptionDetails;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->compileScript(in_expression, in_sourceURL, in_persistScript, std::move(in_executionContextId), &out_scriptId, &out_exceptionDetails);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        if (out_scriptId.isJust())
            result->setValue("scriptId", ValueConversions<String>::toValue(out_scriptId.fromJust()));
        if (out_exceptionDetails.isJust())
            result->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(out_exceptionDetails.fromJust()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::disable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->disable();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::discardConsoleEntries(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->discardConsoleEntries();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::enable(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->enable();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

class EvaluateCallbackImpl : public Backend::EvaluateCallback, public DispatcherBase::Callback {
public:
    EvaluateCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, int callbackId)
        : DispatcherBase::Callback(std::move(backendImpl), callId, callbackId) { }

    void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        resultObject->setValue("result", ValueConversions<protocol::Runtime::RemoteObject>::toValue(result.get()));
        if (exceptionDetails.isJust())
            resultObject->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(exceptionDetails.fromJust()));
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

DispatchResponse::Status DispatcherImpl::evaluate(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* expressionValue = object ? object->get("expression") : nullptr;
    errors->setName("expression");
    String in_expression = ValueConversions<String>::fromValue(expressionValue, errors);
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    Maybe<String> in_objectGroup;
    if (objectGroupValue) {
        errors->setName("objectGroup");
        in_objectGroup = ValueConversions<String>::fromValue(objectGroupValue, errors);
    }
    protocol::Value* includeCommandLineAPIValue = object ? object->get("includeCommandLineAPI") : nullptr;
    Maybe<bool> in_includeCommandLineAPI;
    if (includeCommandLineAPIValue) {
        errors->setName("includeCommandLineAPI");
        in_includeCommandLineAPI = ValueConversions<bool>::fromValue(includeCommandLineAPIValue, errors);
    }
    protocol::Value* silentValue = object ? object->get("silent") : nullptr;
    Maybe<bool> in_silent;
    if (silentValue) {
        errors->setName("silent");
        in_silent = ValueConversions<bool>::fromValue(silentValue, errors);
    }
    protocol::Value* contextIdValue = object ? object->get("contextId") : nullptr;
    Maybe<int> in_contextId;
    if (contextIdValue) {
        errors->setName("contextId");
        in_contextId = ValueConversions<int>::fromValue(contextIdValue, errors);
    }
    protocol::Value* returnByValueValue = object ? object->get("returnByValue") : nullptr;
    Maybe<bool> in_returnByValue;
    if (returnByValueValue) {
        errors->setName("returnByValue");
        in_returnByValue = ValueConversions<bool>::fromValue(returnByValueValue, errors);
    }
    protocol::Value* generatePreviewValue = object ? object->get("generatePreview") : nullptr;
    Maybe<bool> in_generatePreview;
    if (generatePreviewValue) {
        errors->setName("generatePreview");
        in_generatePreview = ValueConversions<bool>::fromValue(generatePreviewValue, errors);
    }
    protocol::Value* userGestureValue = object ? object->get("userGesture") : nullptr;
    Maybe<bool> in_userGesture;
    if (userGestureValue) {
        errors->setName("userGesture");
        in_userGesture = ValueConversions<bool>::fromValue(userGestureValue, errors);
    }
    protocol::Value* awaitPromiseValue = object ? object->get("awaitPromise") : nullptr;
    Maybe<bool> in_awaitPromise;
    if (awaitPromiseValue) {
        errors->setName("awaitPromise");
        in_awaitPromise = ValueConversions<bool>::fromValue(awaitPromiseValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<EvaluateCallbackImpl> callback(new EvaluateCallbackImpl(weakPtr(), callId, nextCallbackId()));
    m_backend->evaluate(in_expression, std::move(in_objectGroup), std::move(in_includeCommandLineAPI), std::move(in_silent), std::move(in_contextId), std::move(in_returnByValue), std::move(in_generatePreview), std::move(in_userGesture), std::move(in_awaitPromise), std::move(callback));
    return (weak->get() && weak->get()->lastCallbackFallThrough()) ? DispatchResponse::kFallThrough : DispatchResponse::kAsync;
}

DispatchResponse::Status DispatcherImpl::getProperties(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* objectIdValue = object ? object->get("objectId") : nullptr;
    errors->setName("objectId");
    String in_objectId = ValueConversions<String>::fromValue(objectIdValue, errors);
    protocol::Value* ownPropertiesValue = object ? object->get("ownProperties") : nullptr;
    Maybe<bool> in_ownProperties;
    if (ownPropertiesValue) {
        errors->setName("ownProperties");
        in_ownProperties = ValueConversions<bool>::fromValue(ownPropertiesValue, errors);
    }
    protocol::Value* accessorPropertiesOnlyValue = object ? object->get("accessorPropertiesOnly") : nullptr;
    Maybe<bool> in_accessorPropertiesOnly;
    if (accessorPropertiesOnlyValue) {
        errors->setName("accessorPropertiesOnly");
        in_accessorPropertiesOnly = ValueConversions<bool>::fromValue(accessorPropertiesOnlyValue, errors);
    }
    protocol::Value* generatePreviewValue = object ? object->get("generatePreview") : nullptr;
    Maybe<bool> in_generatePreview;
    if (generatePreviewValue) {
        errors->setName("generatePreview");
        in_generatePreview = ValueConversions<bool>::fromValue(generatePreviewValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<protocol::Runtime::PropertyDescriptor>> out_result;
    Maybe<protocol::Array<protocol::Runtime::InternalPropertyDescriptor>> out_internalProperties;
    Maybe<protocol::Runtime::ExceptionDetails> out_exceptionDetails;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->getProperties(in_objectId, std::move(in_ownProperties), std::move(in_accessorPropertiesOnly), std::move(in_generatePreview), &out_result, &out_internalProperties, &out_exceptionDetails);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("result", ValueConversions<protocol::Array<protocol::Runtime::PropertyDescriptor>>::toValue(out_result.get()));
        if (out_internalProperties.isJust())
            result->setValue("internalProperties", ValueConversions<protocol::Array<protocol::Runtime::InternalPropertyDescriptor>>::toValue(out_internalProperties.fromJust()));
        if (out_exceptionDetails.isJust())
            result->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(out_exceptionDetails.fromJust()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::globalLexicalScopeNames(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* executionContextIdValue = object ? object->get("executionContextId") : nullptr;
    Maybe<int> in_executionContextId;
    if (executionContextIdValue) {
        errors->setName("executionContextId");
        in_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Array<String>> out_names;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->globalLexicalScopeNames(std::move(in_executionContextId), &out_names);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("names", ValueConversions<protocol::Array<String>>::toValue(out_names.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::queryObjects(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* prototypeObjectIdValue = object ? object->get("prototypeObjectId") : nullptr;
    errors->setName("prototypeObjectId");
    String in_prototypeObjectId = ValueConversions<String>::fromValue(prototypeObjectIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }
    // Declare output parameters.
    std::unique_ptr<protocol::Runtime::RemoteObject> out_objects;

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->queryObjects(in_prototypeObjectId, &out_objects);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    std::unique_ptr<protocol::DictionaryValue> result = DictionaryValue::create();
    if (response.status() == DispatchResponse::kSuccess) {
        result->setValue("objects", ValueConversions<protocol::Runtime::RemoteObject>::toValue(out_objects.get()));
    }
    if (weak->get())
        weak->get()->sendResponse(callId, response, std::move(result));
    return response.status();
}

DispatchResponse::Status DispatcherImpl::releaseObject(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* objectIdValue = object ? object->get("objectId") : nullptr;
    errors->setName("objectId");
    String in_objectId = ValueConversions<String>::fromValue(objectIdValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->releaseObject(in_objectId);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::releaseObjectGroup(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    errors->setName("objectGroup");
    String in_objectGroup = ValueConversions<String>::fromValue(objectGroupValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->releaseObjectGroup(in_objectGroup);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

DispatchResponse::Status DispatcherImpl::runIfWaitingForDebugger(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->runIfWaitingForDebugger();
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

class RunScriptCallbackImpl : public Backend::RunScriptCallback, public DispatcherBase::Callback {
public:
    RunScriptCallbackImpl(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId, int callbackId)
        : DispatcherBase::Callback(std::move(backendImpl), callId, callbackId) { }

    void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) override
    {
        std::unique_ptr<protocol::DictionaryValue> resultObject = DictionaryValue::create();
        resultObject->setValue("result", ValueConversions<protocol::Runtime::RemoteObject>::toValue(result.get()));
        if (exceptionDetails.isJust())
            resultObject->setValue("exceptionDetails", ValueConversions<protocol::Runtime::ExceptionDetails>::toValue(exceptionDetails.fromJust()));
        sendIfActive(std::move(resultObject), DispatchResponse::OK());
    }

    void fallThrough() override
    {
        fallThroughIfActive();
    }

    void sendFailure(const DispatchResponse& response) override
    {
        DCHECK(response.status() == DispatchResponse::kError);
        sendIfActive(nullptr, response);
    }
};

DispatchResponse::Status DispatcherImpl::runScript(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* scriptIdValue = object ? object->get("scriptId") : nullptr;
    errors->setName("scriptId");
    String in_scriptId = ValueConversions<String>::fromValue(scriptIdValue, errors);
    protocol::Value* executionContextIdValue = object ? object->get("executionContextId") : nullptr;
    Maybe<int> in_executionContextId;
    if (executionContextIdValue) {
        errors->setName("executionContextId");
        in_executionContextId = ValueConversions<int>::fromValue(executionContextIdValue, errors);
    }
    protocol::Value* objectGroupValue = object ? object->get("objectGroup") : nullptr;
    Maybe<String> in_objectGroup;
    if (objectGroupValue) {
        errors->setName("objectGroup");
        in_objectGroup = ValueConversions<String>::fromValue(objectGroupValue, errors);
    }
    protocol::Value* silentValue = object ? object->get("silent") : nullptr;
    Maybe<bool> in_silent;
    if (silentValue) {
        errors->setName("silent");
        in_silent = ValueConversions<bool>::fromValue(silentValue, errors);
    }
    protocol::Value* includeCommandLineAPIValue = object ? object->get("includeCommandLineAPI") : nullptr;
    Maybe<bool> in_includeCommandLineAPI;
    if (includeCommandLineAPIValue) {
        errors->setName("includeCommandLineAPI");
        in_includeCommandLineAPI = ValueConversions<bool>::fromValue(includeCommandLineAPIValue, errors);
    }
    protocol::Value* returnByValueValue = object ? object->get("returnByValue") : nullptr;
    Maybe<bool> in_returnByValue;
    if (returnByValueValue) {
        errors->setName("returnByValue");
        in_returnByValue = ValueConversions<bool>::fromValue(returnByValueValue, errors);
    }
    protocol::Value* generatePreviewValue = object ? object->get("generatePreview") : nullptr;
    Maybe<bool> in_generatePreview;
    if (generatePreviewValue) {
        errors->setName("generatePreview");
        in_generatePreview = ValueConversions<bool>::fromValue(generatePreviewValue, errors);
    }
    protocol::Value* awaitPromiseValue = object ? object->get("awaitPromise") : nullptr;
    Maybe<bool> in_awaitPromise;
    if (awaitPromiseValue) {
        errors->setName("awaitPromise");
        in_awaitPromise = ValueConversions<bool>::fromValue(awaitPromiseValue, errors);
    }
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    std::unique_ptr<RunScriptCallbackImpl> callback(new RunScriptCallbackImpl(weakPtr(), callId, nextCallbackId()));
    m_backend->runScript(in_scriptId, std::move(in_executionContextId), std::move(in_objectGroup), std::move(in_silent), std::move(in_includeCommandLineAPI), std::move(in_returnByValue), std::move(in_generatePreview), std::move(in_awaitPromise), std::move(callback));
    return (weak->get() && weak->get()->lastCallbackFallThrough()) ? DispatchResponse::kFallThrough : DispatchResponse::kAsync;
}

DispatchResponse::Status DispatcherImpl::setCustomObjectFormatterEnabled(int callId, std::unique_ptr<DictionaryValue> requestMessageObject, ErrorSupport* errors)
{
    // Prepare input parameters.
    protocol::DictionaryValue* object = DictionaryValue::cast(requestMessageObject->get("params"));
    errors->push();
    protocol::Value* enabledValue = object ? object->get("enabled") : nullptr;
    errors->setName("enabled");
    bool in_enabled = ValueConversions<bool>::fromValue(enabledValue, errors);
    errors->pop();
    if (errors->hasErrors()) {
        reportProtocolError(callId, DispatchResponse::kInvalidParams, kInvalidParamsString, errors);
        return DispatchResponse::kError;
    }

    std::unique_ptr<DispatcherBase::WeakPtr> weak = weakPtr();
    DispatchResponse response = m_backend->setCustomObjectFormatterEnabled(in_enabled);
    if (response.status() == DispatchResponse::kFallThrough)
        return response.status();
    if (weak->get())
        weak->get()->sendResponse(callId, response);
    return response.status();
}

// static
void Dispatcher::wire(UberDispatcher* uber, Backend* backend)
{
    std::unique_ptr<DispatcherImpl> dispatcher(new DispatcherImpl(uber->channel(), backend, uber->fallThroughForNotFound()));
    uber->setupRedirects(dispatcher->redirects());
    uber->registerBackend("Runtime", std::move(dispatcher));
}

} // Runtime
} // namespace v8_inspector
} // namespace protocol
