// This file is generated

// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Runtime_h
#define v8_inspector_protocol_Runtime_h

#include "src/inspector/protocol/Protocol.h"
// For each imported domain we generate a ValueConversions struct instead of a full domain definition
// and include Domain::API version from there.
#include "include/inspector/Runtime.h"

namespace v8_inspector {
namespace protocol {
namespace Runtime {

// ------------- Forward and enum declarations.
// Unique script identifier.
using ScriptId = String;
// Unique object identifier.
using RemoteObjectId = String;
// Primitive value which cannot be JSON-stringified.
using UnserializableValue = String;
// Mirror object referencing original JavaScript object.
class RemoteObject;
//
class CustomPreview;
// Object containing abbreviated remote object value.
class ObjectPreview;
//
class PropertyPreview;
//
class EntryPreview;
// Object property descriptor.
class PropertyDescriptor;
// Object internal property descriptor. This property isn't normally visible in JavaScript code.
class InternalPropertyDescriptor;
// Represents function call argument. Either remote object id <code>objectId</code>, primitive <code>value</code>, unserializable primitive value or neither of (for undefined) them should be specified.
class CallArgument;
// Id of an execution context.
using ExecutionContextId = int;
// Description of an isolated world.
class ExecutionContextDescription;
// Detailed information about exception (or error) that was thrown during script compilation or execution.
class ExceptionDetails;
// Number of milliseconds since epoch.
using Timestamp = double;
// Stack entry for runtime errors and assertions.
class CallFrame;
// Call frames for assertions or error messages.
class StackTrace;

namespace UnserializableValueEnum {
extern const char* Infinity;
extern const char* NaN;
extern const char* NegativeInfinity;
extern const char* Negative0;
} // namespace UnserializableValueEnum

namespace ConsoleAPICalled {
namespace TypeEnum {
extern const char* Log;
extern const char* Debug;
extern const char* Info;
extern const char* Error;
extern const char* Warning;
extern const char* Dir;
extern const char* Dirxml;
extern const char* Table;
extern const char* Trace;
extern const char* Clear;
extern const char* StartGroup;
extern const char* StartGroupCollapsed;
extern const char* EndGroup;
extern const char* Assert;
extern const char* Profile;
extern const char* ProfileEnd;
} // TypeEnum
} // ConsoleAPICalled

// ------------- Type and builder declarations.

// Mirror object referencing original JavaScript object.
class  RemoteObject : public API::RemoteObject {
        PROTOCOL_DISALLOW_COPY(RemoteObject);
    public:
        static std::unique_ptr<RemoteObject> parse(protocol::Value* value, ErrorSupport* errors);

        ~RemoteObject() { }

        struct  TypeEnum {
            static const char* Object;
            static const char* Function;
            static const char* Undefined;
            static const char* String;
            static const char* Number;
            static const char* Boolean;
            static const char* Symbol;
        }; // TypeEnum

        String getType() {
            return m_type;
        }
        void setType(const String& value) {
            m_type = value;
        }

        struct  SubtypeEnum {
            static const char* Array;
            static const char* Null;
            static const char* Node;
            static const char* Regexp;
            static const char* Date;
            static const char* Map;
            static const char* Set;
            static const char* Iterator;
            static const char* Generator;
            static const char* Error;
            static const char* Proxy;
            static const char* Promise;
            static const char* Typedarray;
        }; // SubtypeEnum

        bool hasSubtype() {
            return m_subtype.isJust();
        }
        String getSubtype(const String& defaultValue) {
            return m_subtype.isJust() ? m_subtype.fromJust() : defaultValue;
        }
        void setSubtype(const String& value) {
            m_subtype = value;
        }

        bool hasClassName() {
            return m_className.isJust();
        }
        String getClassName(const String& defaultValue) {
            return m_className.isJust() ? m_className.fromJust() : defaultValue;
        }
        void setClassName(const String& value) {
            m_className = value;
        }

        bool hasValue() {
            return m_value.isJust();
        }
        protocol::Value* getValue(protocol::Value* defaultValue) {
            return m_value.isJust() ? m_value.fromJust() : defaultValue;
        }
        void setValue(std::unique_ptr<protocol::Value> value) {
            m_value = std::move(value);
        }

        bool hasUnserializableValue() {
            return m_unserializableValue.isJust();
        }
        String getUnserializableValue(const String& defaultValue) {
            return m_unserializableValue.isJust() ? m_unserializableValue.fromJust() : defaultValue;
        }
        void setUnserializableValue(const String& value) {
            m_unserializableValue = value;
        }

        bool hasDescription() {
            return m_description.isJust();
        }
        String getDescription(const String& defaultValue) {
            return m_description.isJust() ? m_description.fromJust() : defaultValue;
        }
        void setDescription(const String& value) {
            m_description = value;
        }

        bool hasObjectId() {
            return m_objectId.isJust();
        }
        String getObjectId(const String& defaultValue) {
            return m_objectId.isJust() ? m_objectId.fromJust() : defaultValue;
        }
        void setObjectId(const String& value) {
            m_objectId = value;
        }

        bool hasPreview() {
            return m_preview.isJust();
        }
        protocol::Runtime::ObjectPreview* getPreview(protocol::Runtime::ObjectPreview* defaultValue) {
            return m_preview.isJust() ? m_preview.fromJust() : defaultValue;
        }
        void setPreview(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
            m_preview = std::move(value);
        }

        bool hasCustomPreview() {
            return m_customPreview.isJust();
        }
        protocol::Runtime::CustomPreview* getCustomPreview(protocol::Runtime::CustomPreview* defaultValue) {
            return m_customPreview.isJust() ? m_customPreview.fromJust() : defaultValue;
        }
        void setCustomPreview(std::unique_ptr<protocol::Runtime::CustomPreview> value) {
            m_customPreview = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<RemoteObject> clone() const;
        std::unique_ptr<StringBuffer> toJSONString() const override;

        template<int STATE>
        class RemoteObjectBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TypeSet = 1 << 1,
                    AllFieldsSet = (TypeSet | 0)
                };


                RemoteObjectBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                RemoteObjectBuilder<STATE>& setSubtype(const String& value) {
                    m_result->setSubtype(value);
                    return *this;
                }

                RemoteObjectBuilder<STATE>& setClassName(const String& value) {
                    m_result->setClassName(value);
                    return *this;
                }

                RemoteObjectBuilder<STATE>& setValue(std::unique_ptr<protocol::Value> value) {
                    m_result->setValue(std::move(value));
                    return *this;
                }

                RemoteObjectBuilder<STATE>& setUnserializableValue(const String& value) {
                    m_result->setUnserializableValue(value);
                    return *this;
                }

                RemoteObjectBuilder<STATE>& setDescription(const String& value) {
                    m_result->setDescription(value);
                    return *this;
                }

                RemoteObjectBuilder<STATE>& setObjectId(const String& value) {
                    m_result->setObjectId(value);
                    return *this;
                }

                RemoteObjectBuilder<STATE>& setPreview(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
                    m_result->setPreview(std::move(value));
                    return *this;
                }

                RemoteObjectBuilder<STATE>& setCustomPreview(std::unique_ptr<protocol::Runtime::CustomPreview> value) {
                    m_result->setCustomPreview(std::move(value));
                    return *this;
                }

                std::unique_ptr<RemoteObject> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class RemoteObject;
                RemoteObjectBuilder() : m_result(new RemoteObject()) { }

                template<int STEP> RemoteObjectBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<RemoteObjectBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::RemoteObject> m_result;
        };

        static RemoteObjectBuilder<0> create() {
            return RemoteObjectBuilder<0>();
        }

    private:
        RemoteObject() {
        }

        String m_type;
        Maybe<String> m_subtype;
        Maybe<String> m_className;
        Maybe<protocol::Value> m_value;
        Maybe<String> m_unserializableValue;
        Maybe<String> m_description;
        Maybe<String> m_objectId;
        Maybe<protocol::Runtime::ObjectPreview> m_preview;
        Maybe<protocol::Runtime::CustomPreview> m_customPreview;
};


//
class  CustomPreview {
        PROTOCOL_DISALLOW_COPY(CustomPreview);
    public:
        static std::unique_ptr<CustomPreview> parse(protocol::Value* value, ErrorSupport* errors);

        ~CustomPreview() { }

        String getHeader() {
            return m_header;
        }
        void setHeader(const String& value) {
            m_header = value;
        }

        bool getHasBody() {
            return m_hasBody;
        }
        void setHasBody(bool value) {
            m_hasBody = value;
        }

        String getFormatterObjectId() {
            return m_formatterObjectId;
        }
        void setFormatterObjectId(const String& value) {
            m_formatterObjectId = value;
        }

        String getBindRemoteObjectFunctionId() {
            return m_bindRemoteObjectFunctionId;
        }
        void setBindRemoteObjectFunctionId(const String& value) {
            m_bindRemoteObjectFunctionId = value;
        }

        bool hasConfigObjectId() {
            return m_configObjectId.isJust();
        }
        String getConfigObjectId(const String& defaultValue) {
            return m_configObjectId.isJust() ? m_configObjectId.fromJust() : defaultValue;
        }
        void setConfigObjectId(const String& value) {
            m_configObjectId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<CustomPreview> clone() const;

        template<int STATE>
        class CustomPreviewBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    HeaderSet = 1 << 1,
                    HasBodySet = 1 << 2,
                    FormatterObjectIdSet = 1 << 3,
                    BindRemoteObjectFunctionIdSet = 1 << 4,
                    AllFieldsSet = (HeaderSet | HasBodySet | FormatterObjectIdSet | BindRemoteObjectFunctionIdSet | 0)
                };


                CustomPreviewBuilder<STATE | HeaderSet>& setHeader(const String& value) {
                    static_assert(!(STATE & HeaderSet), "property header should not be set yet");
                    m_result->setHeader(value);
                    return castState<HeaderSet>();
                }

                CustomPreviewBuilder<STATE | HasBodySet>& setHasBody(bool value) {
                    static_assert(!(STATE & HasBodySet), "property hasBody should not be set yet");
                    m_result->setHasBody(value);
                    return castState<HasBodySet>();
                }

                CustomPreviewBuilder<STATE | FormatterObjectIdSet>& setFormatterObjectId(const String& value) {
                    static_assert(!(STATE & FormatterObjectIdSet), "property formatterObjectId should not be set yet");
                    m_result->setFormatterObjectId(value);
                    return castState<FormatterObjectIdSet>();
                }

                CustomPreviewBuilder<STATE | BindRemoteObjectFunctionIdSet>& setBindRemoteObjectFunctionId(const String& value) {
                    static_assert(!(STATE & BindRemoteObjectFunctionIdSet), "property bindRemoteObjectFunctionId should not be set yet");
                    m_result->setBindRemoteObjectFunctionId(value);
                    return castState<BindRemoteObjectFunctionIdSet>();
                }

                CustomPreviewBuilder<STATE>& setConfigObjectId(const String& value) {
                    m_result->setConfigObjectId(value);
                    return *this;
                }

                std::unique_ptr<CustomPreview> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CustomPreview;
                CustomPreviewBuilder() : m_result(new CustomPreview()) { }

                template<int STEP> CustomPreviewBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CustomPreviewBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::CustomPreview> m_result;
        };

        static CustomPreviewBuilder<0> create() {
            return CustomPreviewBuilder<0>();
        }

    private:
        CustomPreview() {
            m_hasBody = false;
        }

        String m_header;
        bool m_hasBody;
        String m_formatterObjectId;
        String m_bindRemoteObjectFunctionId;
        Maybe<String> m_configObjectId;
};


// Object containing abbreviated remote object value.
class  ObjectPreview {
        PROTOCOL_DISALLOW_COPY(ObjectPreview);
    public:
        static std::unique_ptr<ObjectPreview> parse(protocol::Value* value, ErrorSupport* errors);

        ~ObjectPreview() { }

        struct  TypeEnum {
            static const char* Object;
            static const char* Function;
            static const char* Undefined;
            static const char* String;
            static const char* Number;
            static const char* Boolean;
            static const char* Symbol;
        }; // TypeEnum

        String getType() {
            return m_type;
        }
        void setType(const String& value) {
            m_type = value;
        }

        struct  SubtypeEnum {
            static const char* Array;
            static const char* Null;
            static const char* Node;
            static const char* Regexp;
            static const char* Date;
            static const char* Map;
            static const char* Set;
            static const char* Iterator;
            static const char* Generator;
            static const char* Error;
        }; // SubtypeEnum

        bool hasSubtype() {
            return m_subtype.isJust();
        }
        String getSubtype(const String& defaultValue) {
            return m_subtype.isJust() ? m_subtype.fromJust() : defaultValue;
        }
        void setSubtype(const String& value) {
            m_subtype = value;
        }

        bool hasDescription() {
            return m_description.isJust();
        }
        String getDescription(const String& defaultValue) {
            return m_description.isJust() ? m_description.fromJust() : defaultValue;
        }
        void setDescription(const String& value) {
            m_description = value;
        }

        bool getOverflow() {
            return m_overflow;
        }
        void setOverflow(bool value) {
            m_overflow = value;
        }

        protocol::Array<protocol::Runtime::PropertyPreview>* getProperties() {
            return m_properties.get();
        }
        void setProperties(std::unique_ptr<protocol::Array<protocol::Runtime::PropertyPreview>> value) {
            m_properties = std::move(value);
        }

        bool hasEntries() {
            return m_entries.isJust();
        }
        protocol::Array<protocol::Runtime::EntryPreview>* getEntries(protocol::Array<protocol::Runtime::EntryPreview>* defaultValue) {
            return m_entries.isJust() ? m_entries.fromJust() : defaultValue;
        }
        void setEntries(std::unique_ptr<protocol::Array<protocol::Runtime::EntryPreview>> value) {
            m_entries = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<ObjectPreview> clone() const;

        template<int STATE>
        class ObjectPreviewBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TypeSet = 1 << 1,
                    OverflowSet = 1 << 2,
                    PropertiesSet = 1 << 3,
                    AllFieldsSet = (TypeSet | OverflowSet | PropertiesSet | 0)
                };


                ObjectPreviewBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                ObjectPreviewBuilder<STATE>& setSubtype(const String& value) {
                    m_result->setSubtype(value);
                    return *this;
                }

                ObjectPreviewBuilder<STATE>& setDescription(const String& value) {
                    m_result->setDescription(value);
                    return *this;
                }

                ObjectPreviewBuilder<STATE | OverflowSet>& setOverflow(bool value) {
                    static_assert(!(STATE & OverflowSet), "property overflow should not be set yet");
                    m_result->setOverflow(value);
                    return castState<OverflowSet>();
                }

                ObjectPreviewBuilder<STATE | PropertiesSet>& setProperties(std::unique_ptr<protocol::Array<protocol::Runtime::PropertyPreview>> value) {
                    static_assert(!(STATE & PropertiesSet), "property properties should not be set yet");
                    m_result->setProperties(std::move(value));
                    return castState<PropertiesSet>();
                }

                ObjectPreviewBuilder<STATE>& setEntries(std::unique_ptr<protocol::Array<protocol::Runtime::EntryPreview>> value) {
                    m_result->setEntries(std::move(value));
                    return *this;
                }

                std::unique_ptr<ObjectPreview> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ObjectPreview;
                ObjectPreviewBuilder() : m_result(new ObjectPreview()) { }

                template<int STEP> ObjectPreviewBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ObjectPreviewBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ObjectPreview> m_result;
        };

        static ObjectPreviewBuilder<0> create() {
            return ObjectPreviewBuilder<0>();
        }

    private:
        ObjectPreview() {
            m_overflow = false;
        }

        String m_type;
        Maybe<String> m_subtype;
        Maybe<String> m_description;
        bool m_overflow;
        std::unique_ptr<protocol::Array<protocol::Runtime::PropertyPreview>> m_properties;
        Maybe<protocol::Array<protocol::Runtime::EntryPreview>> m_entries;
};


//
class  PropertyPreview {
        PROTOCOL_DISALLOW_COPY(PropertyPreview);
    public:
        static std::unique_ptr<PropertyPreview> parse(protocol::Value* value, ErrorSupport* errors);

        ~PropertyPreview() { }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        struct  TypeEnum {
            static const char* Object;
            static const char* Function;
            static const char* Undefined;
            static const char* String;
            static const char* Number;
            static const char* Boolean;
            static const char* Symbol;
            static const char* Accessor;
        }; // TypeEnum

        String getType() {
            return m_type;
        }
        void setType(const String& value) {
            m_type = value;
        }

        bool hasValue() {
            return m_value.isJust();
        }
        String getValue(const String& defaultValue) {
            return m_value.isJust() ? m_value.fromJust() : defaultValue;
        }
        void setValue(const String& value) {
            m_value = value;
        }

        bool hasValuePreview() {
            return m_valuePreview.isJust();
        }
        protocol::Runtime::ObjectPreview* getValuePreview(protocol::Runtime::ObjectPreview* defaultValue) {
            return m_valuePreview.isJust() ? m_valuePreview.fromJust() : defaultValue;
        }
        void setValuePreview(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
            m_valuePreview = std::move(value);
        }

        struct  SubtypeEnum {
            static const char* Array;
            static const char* Null;
            static const char* Node;
            static const char* Regexp;
            static const char* Date;
            static const char* Map;
            static const char* Set;
            static const char* Iterator;
            static const char* Generator;
            static const char* Error;
        }; // SubtypeEnum

        bool hasSubtype() {
            return m_subtype.isJust();
        }
        String getSubtype(const String& defaultValue) {
            return m_subtype.isJust() ? m_subtype.fromJust() : defaultValue;
        }
        void setSubtype(const String& value) {
            m_subtype = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<PropertyPreview> clone() const;

        template<int STATE>
        class PropertyPreviewBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    TypeSet = 1 << 2,
                    AllFieldsSet = (NameSet | TypeSet | 0)
                };


                PropertyPreviewBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                PropertyPreviewBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                PropertyPreviewBuilder<STATE>& setValue(const String& value) {
                    m_result->setValue(value);
                    return *this;
                }

                PropertyPreviewBuilder<STATE>& setValuePreview(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
                    m_result->setValuePreview(std::move(value));
                    return *this;
                }

                PropertyPreviewBuilder<STATE>& setSubtype(const String& value) {
                    m_result->setSubtype(value);
                    return *this;
                }

                std::unique_ptr<PropertyPreview> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class PropertyPreview;
                PropertyPreviewBuilder() : m_result(new PropertyPreview()) { }

                template<int STEP> PropertyPreviewBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<PropertyPreviewBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::PropertyPreview> m_result;
        };

        static PropertyPreviewBuilder<0> create() {
            return PropertyPreviewBuilder<0>();
        }

    private:
        PropertyPreview() {
        }

        String m_name;
        String m_type;
        Maybe<String> m_value;
        Maybe<protocol::Runtime::ObjectPreview> m_valuePreview;
        Maybe<String> m_subtype;
};


//
class  EntryPreview {
        PROTOCOL_DISALLOW_COPY(EntryPreview);
    public:
        static std::unique_ptr<EntryPreview> parse(protocol::Value* value, ErrorSupport* errors);

        ~EntryPreview() { }

        bool hasKey() {
            return m_key.isJust();
        }
        protocol::Runtime::ObjectPreview* getKey(protocol::Runtime::ObjectPreview* defaultValue) {
            return m_key.isJust() ? m_key.fromJust() : defaultValue;
        }
        void setKey(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
            m_key = std::move(value);
        }

        protocol::Runtime::ObjectPreview* getValue() {
            return m_value.get();
        }
        void setValue(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
            m_value = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<EntryPreview> clone() const;

        template<int STATE>
        class EntryPreviewBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ValueSet = 1 << 1,
                    AllFieldsSet = (ValueSet | 0)
                };


                EntryPreviewBuilder<STATE>& setKey(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
                    m_result->setKey(std::move(value));
                    return *this;
                }

                EntryPreviewBuilder<STATE | ValueSet>& setValue(std::unique_ptr<protocol::Runtime::ObjectPreview> value) {
                    static_assert(!(STATE & ValueSet), "property value should not be set yet");
                    m_result->setValue(std::move(value));
                    return castState<ValueSet>();
                }

                std::unique_ptr<EntryPreview> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class EntryPreview;
                EntryPreviewBuilder() : m_result(new EntryPreview()) { }

                template<int STEP> EntryPreviewBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<EntryPreviewBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::EntryPreview> m_result;
        };

        static EntryPreviewBuilder<0> create() {
            return EntryPreviewBuilder<0>();
        }

    private:
        EntryPreview() {
        }

        Maybe<protocol::Runtime::ObjectPreview> m_key;
        std::unique_ptr<protocol::Runtime::ObjectPreview> m_value;
};


// Object property descriptor.
class  PropertyDescriptor {
        PROTOCOL_DISALLOW_COPY(PropertyDescriptor);
    public:
        static std::unique_ptr<PropertyDescriptor> parse(protocol::Value* value, ErrorSupport* errors);

        ~PropertyDescriptor() { }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        bool hasValue() {
            return m_value.isJust();
        }
        protocol::Runtime::RemoteObject* getValue(protocol::Runtime::RemoteObject* defaultValue) {
            return m_value.isJust() ? m_value.fromJust() : defaultValue;
        }
        void setValue(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_value = std::move(value);
        }

        bool hasWritable() {
            return m_writable.isJust();
        }
        bool getWritable(bool defaultValue) {
            return m_writable.isJust() ? m_writable.fromJust() : defaultValue;
        }
        void setWritable(bool value) {
            m_writable = value;
        }

        bool hasGet() {
            return m_get.isJust();
        }
        protocol::Runtime::RemoteObject* getGet(protocol::Runtime::RemoteObject* defaultValue) {
            return m_get.isJust() ? m_get.fromJust() : defaultValue;
        }
        void setGet(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_get = std::move(value);
        }

        bool hasSet() {
            return m_set.isJust();
        }
        protocol::Runtime::RemoteObject* getSet(protocol::Runtime::RemoteObject* defaultValue) {
            return m_set.isJust() ? m_set.fromJust() : defaultValue;
        }
        void setSet(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_set = std::move(value);
        }

        bool getConfigurable() {
            return m_configurable;
        }
        void setConfigurable(bool value) {
            m_configurable = value;
        }

        bool getEnumerable() {
            return m_enumerable;
        }
        void setEnumerable(bool value) {
            m_enumerable = value;
        }

        bool hasWasThrown() {
            return m_wasThrown.isJust();
        }
        bool getWasThrown(bool defaultValue) {
            return m_wasThrown.isJust() ? m_wasThrown.fromJust() : defaultValue;
        }
        void setWasThrown(bool value) {
            m_wasThrown = value;
        }

        bool hasIsOwn() {
            return m_isOwn.isJust();
        }
        bool getIsOwn(bool defaultValue) {
            return m_isOwn.isJust() ? m_isOwn.fromJust() : defaultValue;
        }
        void setIsOwn(bool value) {
            m_isOwn = value;
        }

        bool hasSymbol() {
            return m_symbol.isJust();
        }
        protocol::Runtime::RemoteObject* getSymbol(protocol::Runtime::RemoteObject* defaultValue) {
            return m_symbol.isJust() ? m_symbol.fromJust() : defaultValue;
        }
        void setSymbol(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_symbol = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<PropertyDescriptor> clone() const;

        template<int STATE>
        class PropertyDescriptorBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    ConfigurableSet = 1 << 2,
                    EnumerableSet = 1 << 3,
                    AllFieldsSet = (NameSet | ConfigurableSet | EnumerableSet | 0)
                };


                PropertyDescriptorBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                PropertyDescriptorBuilder<STATE>& setValue(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    m_result->setValue(std::move(value));
                    return *this;
                }

                PropertyDescriptorBuilder<STATE>& setWritable(bool value) {
                    m_result->setWritable(value);
                    return *this;
                }

                PropertyDescriptorBuilder<STATE>& setGet(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    m_result->setGet(std::move(value));
                    return *this;
                }

                PropertyDescriptorBuilder<STATE>& setSet(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    m_result->setSet(std::move(value));
                    return *this;
                }

                PropertyDescriptorBuilder<STATE | ConfigurableSet>& setConfigurable(bool value) {
                    static_assert(!(STATE & ConfigurableSet), "property configurable should not be set yet");
                    m_result->setConfigurable(value);
                    return castState<ConfigurableSet>();
                }

                PropertyDescriptorBuilder<STATE | EnumerableSet>& setEnumerable(bool value) {
                    static_assert(!(STATE & EnumerableSet), "property enumerable should not be set yet");
                    m_result->setEnumerable(value);
                    return castState<EnumerableSet>();
                }

                PropertyDescriptorBuilder<STATE>& setWasThrown(bool value) {
                    m_result->setWasThrown(value);
                    return *this;
                }

                PropertyDescriptorBuilder<STATE>& setIsOwn(bool value) {
                    m_result->setIsOwn(value);
                    return *this;
                }

                PropertyDescriptorBuilder<STATE>& setSymbol(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    m_result->setSymbol(std::move(value));
                    return *this;
                }

                std::unique_ptr<PropertyDescriptor> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class PropertyDescriptor;
                PropertyDescriptorBuilder() : m_result(new PropertyDescriptor()) { }

                template<int STEP> PropertyDescriptorBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<PropertyDescriptorBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::PropertyDescriptor> m_result;
        };

        static PropertyDescriptorBuilder<0> create() {
            return PropertyDescriptorBuilder<0>();
        }

    private:
        PropertyDescriptor() {
            m_configurable = false;
            m_enumerable = false;
        }

        String m_name;
        Maybe<protocol::Runtime::RemoteObject> m_value;
        Maybe<bool> m_writable;
        Maybe<protocol::Runtime::RemoteObject> m_get;
        Maybe<protocol::Runtime::RemoteObject> m_set;
        bool m_configurable;
        bool m_enumerable;
        Maybe<bool> m_wasThrown;
        Maybe<bool> m_isOwn;
        Maybe<protocol::Runtime::RemoteObject> m_symbol;
};


// Object internal property descriptor. This property isn't normally visible in JavaScript code.
class  InternalPropertyDescriptor {
        PROTOCOL_DISALLOW_COPY(InternalPropertyDescriptor);
    public:
        static std::unique_ptr<InternalPropertyDescriptor> parse(protocol::Value* value, ErrorSupport* errors);

        ~InternalPropertyDescriptor() { }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        bool hasValue() {
            return m_value.isJust();
        }
        protocol::Runtime::RemoteObject* getValue(protocol::Runtime::RemoteObject* defaultValue) {
            return m_value.isJust() ? m_value.fromJust() : defaultValue;
        }
        void setValue(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_value = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<InternalPropertyDescriptor> clone() const;

        template<int STATE>
        class InternalPropertyDescriptorBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    AllFieldsSet = (NameSet | 0)
                };


                InternalPropertyDescriptorBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                InternalPropertyDescriptorBuilder<STATE>& setValue(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    m_result->setValue(std::move(value));
                    return *this;
                }

                std::unique_ptr<InternalPropertyDescriptor> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class InternalPropertyDescriptor;
                InternalPropertyDescriptorBuilder() : m_result(new InternalPropertyDescriptor()) { }

                template<int STEP> InternalPropertyDescriptorBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<InternalPropertyDescriptorBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::InternalPropertyDescriptor> m_result;
        };

        static InternalPropertyDescriptorBuilder<0> create() {
            return InternalPropertyDescriptorBuilder<0>();
        }

    private:
        InternalPropertyDescriptor() {
        }

        String m_name;
        Maybe<protocol::Runtime::RemoteObject> m_value;
};


// Represents function call argument. Either remote object id <code>objectId</code>, primitive <code>value</code>, unserializable primitive value or neither of (for undefined) them should be specified.
class  CallArgument {
        PROTOCOL_DISALLOW_COPY(CallArgument);
    public:
        static std::unique_ptr<CallArgument> parse(protocol::Value* value, ErrorSupport* errors);

        ~CallArgument() { }

        bool hasValue() {
            return m_value.isJust();
        }
        protocol::Value* getValue(protocol::Value* defaultValue) {
            return m_value.isJust() ? m_value.fromJust() : defaultValue;
        }
        void setValue(std::unique_ptr<protocol::Value> value) {
            m_value = std::move(value);
        }

        bool hasUnserializableValue() {
            return m_unserializableValue.isJust();
        }
        String getUnserializableValue(const String& defaultValue) {
            return m_unserializableValue.isJust() ? m_unserializableValue.fromJust() : defaultValue;
        }
        void setUnserializableValue(const String& value) {
            m_unserializableValue = value;
        }

        bool hasObjectId() {
            return m_objectId.isJust();
        }
        String getObjectId(const String& defaultValue) {
            return m_objectId.isJust() ? m_objectId.fromJust() : defaultValue;
        }
        void setObjectId(const String& value) {
            m_objectId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<CallArgument> clone() const;

        template<int STATE>
        class CallArgumentBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    AllFieldsSet = (0)
                };


                CallArgumentBuilder<STATE>& setValue(std::unique_ptr<protocol::Value> value) {
                    m_result->setValue(std::move(value));
                    return *this;
                }

                CallArgumentBuilder<STATE>& setUnserializableValue(const String& value) {
                    m_result->setUnserializableValue(value);
                    return *this;
                }

                CallArgumentBuilder<STATE>& setObjectId(const String& value) {
                    m_result->setObjectId(value);
                    return *this;
                }

                std::unique_ptr<CallArgument> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CallArgument;
                CallArgumentBuilder() : m_result(new CallArgument()) { }

                template<int STEP> CallArgumentBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CallArgumentBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::CallArgument> m_result;
        };

        static CallArgumentBuilder<0> create() {
            return CallArgumentBuilder<0>();
        }

    private:
        CallArgument() {
        }

        Maybe<protocol::Value> m_value;
        Maybe<String> m_unserializableValue;
        Maybe<String> m_objectId;
};


// Description of an isolated world.
class  ExecutionContextDescription {
        PROTOCOL_DISALLOW_COPY(ExecutionContextDescription);
    public:
        static std::unique_ptr<ExecutionContextDescription> parse(protocol::Value* value, ErrorSupport* errors);

        ~ExecutionContextDescription() { }

        int getId() {
            return m_id;
        }
        void setId(int value) {
            m_id = value;
        }

        String getOrigin() {
            return m_origin;
        }
        void setOrigin(const String& value) {
            m_origin = value;
        }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        bool hasAuxData() {
            return m_auxData.isJust();
        }
        protocol::DictionaryValue* getAuxData(protocol::DictionaryValue* defaultValue) {
            return m_auxData.isJust() ? m_auxData.fromJust() : defaultValue;
        }
        void setAuxData(std::unique_ptr<protocol::DictionaryValue> value) {
            m_auxData = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<ExecutionContextDescription> clone() const;

        template<int STATE>
        class ExecutionContextDescriptionBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    IdSet = 1 << 1,
                    OriginSet = 1 << 2,
                    NameSet = 1 << 3,
                    AllFieldsSet = (IdSet | OriginSet | NameSet | 0)
                };


                ExecutionContextDescriptionBuilder<STATE | IdSet>& setId(int value) {
                    static_assert(!(STATE & IdSet), "property id should not be set yet");
                    m_result->setId(value);
                    return castState<IdSet>();
                }

                ExecutionContextDescriptionBuilder<STATE | OriginSet>& setOrigin(const String& value) {
                    static_assert(!(STATE & OriginSet), "property origin should not be set yet");
                    m_result->setOrigin(value);
                    return castState<OriginSet>();
                }

                ExecutionContextDescriptionBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                ExecutionContextDescriptionBuilder<STATE>& setAuxData(std::unique_ptr<protocol::DictionaryValue> value) {
                    m_result->setAuxData(std::move(value));
                    return *this;
                }

                std::unique_ptr<ExecutionContextDescription> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ExecutionContextDescription;
                ExecutionContextDescriptionBuilder() : m_result(new ExecutionContextDescription()) { }

                template<int STEP> ExecutionContextDescriptionBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ExecutionContextDescriptionBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ExecutionContextDescription> m_result;
        };

        static ExecutionContextDescriptionBuilder<0> create() {
            return ExecutionContextDescriptionBuilder<0>();
        }

    private:
        ExecutionContextDescription() {
            m_id = 0;
        }

        int m_id;
        String m_origin;
        String m_name;
        Maybe<protocol::DictionaryValue> m_auxData;
};


// Detailed information about exception (or error) that was thrown during script compilation or execution.
class  ExceptionDetails {
        PROTOCOL_DISALLOW_COPY(ExceptionDetails);
    public:
        static std::unique_ptr<ExceptionDetails> parse(protocol::Value* value, ErrorSupport* errors);

        ~ExceptionDetails() { }

        int getExceptionId() {
            return m_exceptionId;
        }
        void setExceptionId(int value) {
            m_exceptionId = value;
        }

        String getText() {
            return m_text;
        }
        void setText(const String& value) {
            m_text = value;
        }

        int getLineNumber() {
            return m_lineNumber;
        }
        void setLineNumber(int value) {
            m_lineNumber = value;
        }

        int getColumnNumber() {
            return m_columnNumber;
        }
        void setColumnNumber(int value) {
            m_columnNumber = value;
        }

        bool hasScriptId() {
            return m_scriptId.isJust();
        }
        String getScriptId(const String& defaultValue) {
            return m_scriptId.isJust() ? m_scriptId.fromJust() : defaultValue;
        }
        void setScriptId(const String& value) {
            m_scriptId = value;
        }

        bool hasUrl() {
            return m_url.isJust();
        }
        String getUrl(const String& defaultValue) {
            return m_url.isJust() ? m_url.fromJust() : defaultValue;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        bool hasStackTrace() {
            return m_stackTrace.isJust();
        }
        protocol::Runtime::StackTrace* getStackTrace(protocol::Runtime::StackTrace* defaultValue) {
            return m_stackTrace.isJust() ? m_stackTrace.fromJust() : defaultValue;
        }
        void setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
            m_stackTrace = std::move(value);
        }

        bool hasException() {
            return m_exception.isJust();
        }
        protocol::Runtime::RemoteObject* getException(protocol::Runtime::RemoteObject* defaultValue) {
            return m_exception.isJust() ? m_exception.fromJust() : defaultValue;
        }
        void setException(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_exception = std::move(value);
        }

        bool hasExecutionContextId() {
            return m_executionContextId.isJust();
        }
        int getExecutionContextId(int defaultValue) {
            return m_executionContextId.isJust() ? m_executionContextId.fromJust() : defaultValue;
        }
        void setExecutionContextId(int value) {
            m_executionContextId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<ExceptionDetails> clone() const;

        template<int STATE>
        class ExceptionDetailsBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ExceptionIdSet = 1 << 1,
                    TextSet = 1 << 2,
                    LineNumberSet = 1 << 3,
                    ColumnNumberSet = 1 << 4,
                    AllFieldsSet = (ExceptionIdSet | TextSet | LineNumberSet | ColumnNumberSet | 0)
                };


                ExceptionDetailsBuilder<STATE | ExceptionIdSet>& setExceptionId(int value) {
                    static_assert(!(STATE & ExceptionIdSet), "property exceptionId should not be set yet");
                    m_result->setExceptionId(value);
                    return castState<ExceptionIdSet>();
                }

                ExceptionDetailsBuilder<STATE | TextSet>& setText(const String& value) {
                    static_assert(!(STATE & TextSet), "property text should not be set yet");
                    m_result->setText(value);
                    return castState<TextSet>();
                }

                ExceptionDetailsBuilder<STATE | LineNumberSet>& setLineNumber(int value) {
                    static_assert(!(STATE & LineNumberSet), "property lineNumber should not be set yet");
                    m_result->setLineNumber(value);
                    return castState<LineNumberSet>();
                }

                ExceptionDetailsBuilder<STATE | ColumnNumberSet>& setColumnNumber(int value) {
                    static_assert(!(STATE & ColumnNumberSet), "property columnNumber should not be set yet");
                    m_result->setColumnNumber(value);
                    return castState<ColumnNumberSet>();
                }

                ExceptionDetailsBuilder<STATE>& setScriptId(const String& value) {
                    m_result->setScriptId(value);
                    return *this;
                }

                ExceptionDetailsBuilder<STATE>& setUrl(const String& value) {
                    m_result->setUrl(value);
                    return *this;
                }

                ExceptionDetailsBuilder<STATE>& setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setStackTrace(std::move(value));
                    return *this;
                }

                ExceptionDetailsBuilder<STATE>& setException(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    m_result->setException(std::move(value));
                    return *this;
                }

                ExceptionDetailsBuilder<STATE>& setExecutionContextId(int value) {
                    m_result->setExecutionContextId(value);
                    return *this;
                }

                std::unique_ptr<ExceptionDetails> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ExceptionDetails;
                ExceptionDetailsBuilder() : m_result(new ExceptionDetails()) { }

                template<int STEP> ExceptionDetailsBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ExceptionDetailsBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ExceptionDetails> m_result;
        };

        static ExceptionDetailsBuilder<0> create() {
            return ExceptionDetailsBuilder<0>();
        }

    private:
        ExceptionDetails() {
            m_exceptionId = 0;
            m_lineNumber = 0;
            m_columnNumber = 0;
        }

        int m_exceptionId;
        String m_text;
        int m_lineNumber;
        int m_columnNumber;
        Maybe<String> m_scriptId;
        Maybe<String> m_url;
        Maybe<protocol::Runtime::StackTrace> m_stackTrace;
        Maybe<protocol::Runtime::RemoteObject> m_exception;
        Maybe<int> m_executionContextId;
};


// Stack entry for runtime errors and assertions.
class  CallFrame {
        PROTOCOL_DISALLOW_COPY(CallFrame);
    public:
        static std::unique_ptr<CallFrame> parse(protocol::Value* value, ErrorSupport* errors);

        ~CallFrame() { }

        String getFunctionName() {
            return m_functionName;
        }
        void setFunctionName(const String& value) {
            m_functionName = value;
        }

        String getScriptId() {
            return m_scriptId;
        }
        void setScriptId(const String& value) {
            m_scriptId = value;
        }

        String getUrl() {
            return m_url;
        }
        void setUrl(const String& value) {
            m_url = value;
        }

        int getLineNumber() {
            return m_lineNumber;
        }
        void setLineNumber(int value) {
            m_lineNumber = value;
        }

        int getColumnNumber() {
            return m_columnNumber;
        }
        void setColumnNumber(int value) {
            m_columnNumber = value;
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<CallFrame> clone() const;

        template<int STATE>
        class CallFrameBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    FunctionNameSet = 1 << 1,
                    ScriptIdSet = 1 << 2,
                    UrlSet = 1 << 3,
                    LineNumberSet = 1 << 4,
                    ColumnNumberSet = 1 << 5,
                    AllFieldsSet = (FunctionNameSet | ScriptIdSet | UrlSet | LineNumberSet | ColumnNumberSet | 0)
                };


                CallFrameBuilder<STATE | FunctionNameSet>& setFunctionName(const String& value) {
                    static_assert(!(STATE & FunctionNameSet), "property functionName should not be set yet");
                    m_result->setFunctionName(value);
                    return castState<FunctionNameSet>();
                }

                CallFrameBuilder<STATE | ScriptIdSet>& setScriptId(const String& value) {
                    static_assert(!(STATE & ScriptIdSet), "property scriptId should not be set yet");
                    m_result->setScriptId(value);
                    return castState<ScriptIdSet>();
                }

                CallFrameBuilder<STATE | UrlSet>& setUrl(const String& value) {
                    static_assert(!(STATE & UrlSet), "property url should not be set yet");
                    m_result->setUrl(value);
                    return castState<UrlSet>();
                }

                CallFrameBuilder<STATE | LineNumberSet>& setLineNumber(int value) {
                    static_assert(!(STATE & LineNumberSet), "property lineNumber should not be set yet");
                    m_result->setLineNumber(value);
                    return castState<LineNumberSet>();
                }

                CallFrameBuilder<STATE | ColumnNumberSet>& setColumnNumber(int value) {
                    static_assert(!(STATE & ColumnNumberSet), "property columnNumber should not be set yet");
                    m_result->setColumnNumber(value);
                    return castState<ColumnNumberSet>();
                }

                std::unique_ptr<CallFrame> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class CallFrame;
                CallFrameBuilder() : m_result(new CallFrame()) { }

                template<int STEP> CallFrameBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<CallFrameBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::CallFrame> m_result;
        };

        static CallFrameBuilder<0> create() {
            return CallFrameBuilder<0>();
        }

    private:
        CallFrame() {
            m_lineNumber = 0;
            m_columnNumber = 0;
        }

        String m_functionName;
        String m_scriptId;
        String m_url;
        int m_lineNumber;
        int m_columnNumber;
};


// Call frames for assertions or error messages.
class  StackTrace : public API::StackTrace {
        PROTOCOL_DISALLOW_COPY(StackTrace);
    public:
        static std::unique_ptr<StackTrace> parse(protocol::Value* value, ErrorSupport* errors);

        ~StackTrace() { }

        bool hasDescription() {
            return m_description.isJust();
        }
        String getDescription(const String& defaultValue) {
            return m_description.isJust() ? m_description.fromJust() : defaultValue;
        }
        void setDescription(const String& value) {
            m_description = value;
        }

        protocol::Array<protocol::Runtime::CallFrame>* getCallFrames() {
            return m_callFrames.get();
        }
        void setCallFrames(std::unique_ptr<protocol::Array<protocol::Runtime::CallFrame>> value) {
            m_callFrames = std::move(value);
        }

        bool hasParent() {
            return m_parent.isJust();
        }
        protocol::Runtime::StackTrace* getParent(protocol::Runtime::StackTrace* defaultValue) {
            return m_parent.isJust() ? m_parent.fromJust() : defaultValue;
        }
        void setParent(std::unique_ptr<protocol::Runtime::StackTrace> value) {
            m_parent = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> serialize() const;
        std::unique_ptr<StackTrace> clone() const;
        std::unique_ptr<StringBuffer> toJSONString() const override;

        template<int STATE>
        class StackTraceBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    CallFramesSet = 1 << 1,
                    AllFieldsSet = (CallFramesSet | 0)
                };


                StackTraceBuilder<STATE>& setDescription(const String& value) {
                    m_result->setDescription(value);
                    return *this;
                }

                StackTraceBuilder<STATE | CallFramesSet>& setCallFrames(std::unique_ptr<protocol::Array<protocol::Runtime::CallFrame>> value) {
                    static_assert(!(STATE & CallFramesSet), "property callFrames should not be set yet");
                    m_result->setCallFrames(std::move(value));
                    return castState<CallFramesSet>();
                }

                StackTraceBuilder<STATE>& setParent(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setParent(std::move(value));
                    return *this;
                }

                std::unique_ptr<StackTrace> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class StackTrace;
                StackTraceBuilder() : m_result(new StackTrace()) { }

                template<int STEP> StackTraceBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<StackTraceBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::StackTrace> m_result;
        };

        static StackTraceBuilder<0> create() {
            return StackTraceBuilder<0>();
        }

    private:
        StackTrace() {
        }

        Maybe<String> m_description;
        std::unique_ptr<protocol::Array<protocol::Runtime::CallFrame>> m_callFrames;
        Maybe<protocol::Runtime::StackTrace> m_parent;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        class  EvaluateCallback : public BackendCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, const Maybe<protocol::Runtime::ExceptionDetails>& exceptionDetails) = 0;
        };
        virtual void evaluate(const String& in_expression, const Maybe<String>& in_objectGroup, const Maybe<bool>& in_includeCommandLineAPI, const Maybe<bool>& in_silent, const Maybe<int>& in_contextId, const Maybe<bool>& in_returnByValue, const Maybe<bool>& in_generatePreview, const Maybe<bool>& in_userGesture, const Maybe<bool>& in_awaitPromise, std::unique_ptr<EvaluateCallback> callback) = 0;
        class  AwaitPromiseCallback : public BackendCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, const Maybe<protocol::Runtime::ExceptionDetails>& exceptionDetails) = 0;
        };
        virtual void awaitPromise(const String& in_promiseObjectId, const Maybe<bool>& in_returnByValue, const Maybe<bool>& in_generatePreview, std::unique_ptr<AwaitPromiseCallback> callback) = 0;
        class  CallFunctionOnCallback : public BackendCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, const Maybe<protocol::Runtime::ExceptionDetails>& exceptionDetails) = 0;
        };
        virtual void callFunctionOn(const String& in_objectId, const String& in_functionDeclaration, const Maybe<protocol::Array<protocol::Runtime::CallArgument>>& in_arguments, const Maybe<bool>& in_silent, const Maybe<bool>& in_returnByValue, const Maybe<bool>& in_generatePreview, const Maybe<bool>& in_userGesture, const Maybe<bool>& in_awaitPromise, std::unique_ptr<CallFunctionOnCallback> callback) = 0;
        virtual void getProperties(ErrorString*, const String& in_objectId, const Maybe<bool>& in_ownProperties, const Maybe<bool>& in_accessorPropertiesOnly, const Maybe<bool>& in_generatePreview, std::unique_ptr<protocol::Array<protocol::Runtime::PropertyDescriptor>>* out_result, Maybe<protocol::Array<protocol::Runtime::InternalPropertyDescriptor>>* out_internalProperties, Maybe<protocol::Runtime::ExceptionDetails>* out_exceptionDetails) = 0;
        virtual void releaseObject(ErrorString*, const String& in_objectId) = 0;
        virtual void releaseObjectGroup(ErrorString*, const String& in_objectGroup) = 0;
        virtual void runIfWaitingForDebugger(ErrorString*) = 0;
        virtual void enable(ErrorString*) = 0;
        virtual void disable(ErrorString*) = 0;
        virtual void discardConsoleEntries(ErrorString*) = 0;
        virtual void setCustomObjectFormatterEnabled(ErrorString*, bool in_enabled) = 0;
        virtual void compileScript(ErrorString*, const String& in_expression, const String& in_sourceURL, bool in_persistScript, const Maybe<int>& in_executionContextId, Maybe<String>* out_scriptId, Maybe<protocol::Runtime::ExceptionDetails>* out_exceptionDetails) = 0;
        class  RunScriptCallback : public BackendCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, const Maybe<protocol::Runtime::ExceptionDetails>& exceptionDetails) = 0;
        };
        virtual void runScript(const String& in_scriptId, const Maybe<int>& in_executionContextId, const Maybe<String>& in_objectGroup, const Maybe<bool>& in_silent, const Maybe<bool>& in_includeCommandLineAPI, const Maybe<bool>& in_returnByValue, const Maybe<bool>& in_generatePreview, const Maybe<bool>& in_awaitPromise, std::unique_ptr<RunScriptCallback> callback) = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void executionContextCreated(std::unique_ptr<protocol::Runtime::ExecutionContextDescription> context);
        void executionContextDestroyed(int executionContextId);
        void executionContextsCleared();
        void exceptionThrown(double timestamp, std::unique_ptr<protocol::Runtime::ExceptionDetails> exceptionDetails);
        void exceptionRevoked(const String& reason, int exceptionId);
        void consoleAPICalled(const String& type, std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> args, int executionContextId, double timestamp, const Maybe<protocol::Runtime::StackTrace>& stackTrace = Maybe<protocol::Runtime::StackTrace>());
        void inspectRequested(std::unique_ptr<protocol::Runtime::RemoteObject> object, std::unique_ptr<protocol::DictionaryValue> hints);

        void flush();
    private:
        FrontendChannel* m_frontendChannel;
};

// ------------- Dispatcher.

class  Dispatcher {
    public:
        static void wire(UberDispatcher*, Backend*);

    private:
        Dispatcher() { }
};

// ------------- Metainfo.

class  Metainfo {
    public:
        using BackendClass = Backend;
        using FrontendClass = Frontend;
        using DispatcherClass = Dispatcher;
        static const char domainName[];
        static const char commandPrefix[];
        static const char version[];
};

} // namespace Runtime
} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Runtime_h)
