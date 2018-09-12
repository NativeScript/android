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
using ScriptId = String;
using RemoteObjectId = String;
using UnserializableValue = String;
class RemoteObject;
class CustomPreview;
class ObjectPreview;
class PropertyPreview;
class EntryPreview;
class PropertyDescriptor;
class InternalPropertyDescriptor;
class CallArgument;
using ExecutionContextId = int;
class ExecutionContextDescription;
class ExceptionDetails;
using Timestamp = double;
using TimeDelta = double;
class CallFrame;
class StackTrace;
using UniqueDebuggerId = String;
class StackTraceId;
class BindingCalledNotification;
class ConsoleAPICalledNotification;
class ExceptionRevokedNotification;
class ExceptionThrownNotification;
class ExecutionContextCreatedNotification;
class ExecutionContextDestroyedNotification;
using ExecutionContextsClearedNotification = Object;
class InspectRequestedNotification;

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
extern const char* Count;
extern const char* TimeEnd;
} // TypeEnum
} // ConsoleAPICalled

// ------------- Type and builder declarations.

class  RemoteObject : public Serializable, public API::RemoteObject {
        PROTOCOL_DISALLOW_COPY(RemoteObject);
    public:
        static std::unique_ptr<RemoteObject> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~RemoteObject() override { }

        struct  TypeEnum {
            static const char* Object;
            static const char* Function;
            static const char* Undefined;
            static const char* String;
            static const char* Number;
            static const char* Boolean;
            static const char* Symbol;
            static const char* Bigint;
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
            static const char* Weakmap;
            static const char* Weakset;
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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  CustomPreview : public Serializable {
        PROTOCOL_DISALLOW_COPY(CustomPreview);
    public:
        static std::unique_ptr<CustomPreview> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CustomPreview() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  ObjectPreview : public Serializable {
        PROTOCOL_DISALLOW_COPY(ObjectPreview);
    public:
        static std::unique_ptr<ObjectPreview> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ObjectPreview() override { }

        struct  TypeEnum {
            static const char* Object;
            static const char* Function;
            static const char* Undefined;
            static const char* String;
            static const char* Number;
            static const char* Boolean;
            static const char* Symbol;
            static const char* Bigint;
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
            static const char* Weakmap;
            static const char* Weakset;
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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  PropertyPreview : public Serializable {
        PROTOCOL_DISALLOW_COPY(PropertyPreview);
    public:
        static std::unique_ptr<PropertyPreview> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~PropertyPreview() override { }

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
            static const char* Bigint;
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
            static const char* Weakmap;
            static const char* Weakset;
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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  EntryPreview : public Serializable {
        PROTOCOL_DISALLOW_COPY(EntryPreview);
    public:
        static std::unique_ptr<EntryPreview> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~EntryPreview() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  PropertyDescriptor : public Serializable {
        PROTOCOL_DISALLOW_COPY(PropertyDescriptor);
    public:
        static std::unique_ptr<PropertyDescriptor> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~PropertyDescriptor() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  InternalPropertyDescriptor : public Serializable {
        PROTOCOL_DISALLOW_COPY(InternalPropertyDescriptor);
    public:
        static std::unique_ptr<InternalPropertyDescriptor> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~InternalPropertyDescriptor() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  CallArgument : public Serializable {
        PROTOCOL_DISALLOW_COPY(CallArgument);
    public:
        static std::unique_ptr<CallArgument> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CallArgument() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  ExecutionContextDescription : public Serializable {
        PROTOCOL_DISALLOW_COPY(ExecutionContextDescription);
    public:
        static std::unique_ptr<ExecutionContextDescription> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ExecutionContextDescription() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  ExceptionDetails : public Serializable {
        PROTOCOL_DISALLOW_COPY(ExceptionDetails);
    public:
        static std::unique_ptr<ExceptionDetails> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ExceptionDetails() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  CallFrame : public Serializable {
        PROTOCOL_DISALLOW_COPY(CallFrame);
    public:
        static std::unique_ptr<CallFrame> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~CallFrame() override { }

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

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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


class  StackTrace : public Serializable, public API::StackTrace {
        PROTOCOL_DISALLOW_COPY(StackTrace);
    public:
        static std::unique_ptr<StackTrace> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~StackTrace() override { }

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

        bool hasParentId() {
            return m_parentId.isJust();
        }
        protocol::Runtime::StackTraceId* getParentId(protocol::Runtime::StackTraceId* defaultValue) {
            return m_parentId.isJust() ? m_parentId.fromJust() : defaultValue;
        }
        void setParentId(std::unique_ptr<protocol::Runtime::StackTraceId> value) {
            m_parentId = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
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

                StackTraceBuilder<STATE>& setParentId(std::unique_ptr<protocol::Runtime::StackTraceId> value) {
                    m_result->setParentId(std::move(value));
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
        Maybe<protocol::Runtime::StackTraceId> m_parentId;
};


class  StackTraceId : public Serializable, public API::StackTraceId {
        PROTOCOL_DISALLOW_COPY(StackTraceId);
    public:
        static std::unique_ptr<StackTraceId> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~StackTraceId() override { }

        String getId() {
            return m_id;
        }
        void setId(const String& value) {
            m_id = value;
        }

        bool hasDebuggerId() {
            return m_debuggerId.isJust();
        }
        String getDebuggerId(const String& defaultValue) {
            return m_debuggerId.isJust() ? m_debuggerId.fromJust() : defaultValue;
        }
        void setDebuggerId(const String& value) {
            m_debuggerId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<StackTraceId> clone() const;
        std::unique_ptr<StringBuffer> toJSONString() const override;

        template<int STATE>
        class StackTraceIdBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    IdSet = 1 << 1,
                    AllFieldsSet = (IdSet | 0)
                };


                StackTraceIdBuilder<STATE | IdSet>& setId(const String& value) {
                    static_assert(!(STATE & IdSet), "property id should not be set yet");
                    m_result->setId(value);
                    return castState<IdSet>();
                }

                StackTraceIdBuilder<STATE>& setDebuggerId(const String& value) {
                    m_result->setDebuggerId(value);
                    return *this;
                }

                std::unique_ptr<StackTraceId> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class StackTraceId;
                StackTraceIdBuilder() : m_result(new StackTraceId()) { }

                template<int STEP> StackTraceIdBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<StackTraceIdBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::StackTraceId> m_result;
        };

        static StackTraceIdBuilder<0> create() {
            return StackTraceIdBuilder<0>();
        }

    private:
        StackTraceId() {
        }

        String m_id;
        Maybe<String> m_debuggerId;
};


class  BindingCalledNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(BindingCalledNotification);
    public:
        static std::unique_ptr<BindingCalledNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~BindingCalledNotification() override { }

        String getName() {
            return m_name;
        }
        void setName(const String& value) {
            m_name = value;
        }

        String getPayload() {
            return m_payload;
        }
        void setPayload(const String& value) {
            m_payload = value;
        }

        int getExecutionContextId() {
            return m_executionContextId;
        }
        void setExecutionContextId(int value) {
            m_executionContextId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<BindingCalledNotification> clone() const;

        template<int STATE>
        class BindingCalledNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    NameSet = 1 << 1,
                    PayloadSet = 1 << 2,
                    ExecutionContextIdSet = 1 << 3,
                    AllFieldsSet = (NameSet | PayloadSet | ExecutionContextIdSet | 0)
                };


                BindingCalledNotificationBuilder<STATE | NameSet>& setName(const String& value) {
                    static_assert(!(STATE & NameSet), "property name should not be set yet");
                    m_result->setName(value);
                    return castState<NameSet>();
                }

                BindingCalledNotificationBuilder<STATE | PayloadSet>& setPayload(const String& value) {
                    static_assert(!(STATE & PayloadSet), "property payload should not be set yet");
                    m_result->setPayload(value);
                    return castState<PayloadSet>();
                }

                BindingCalledNotificationBuilder<STATE | ExecutionContextIdSet>& setExecutionContextId(int value) {
                    static_assert(!(STATE & ExecutionContextIdSet), "property executionContextId should not be set yet");
                    m_result->setExecutionContextId(value);
                    return castState<ExecutionContextIdSet>();
                }

                std::unique_ptr<BindingCalledNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class BindingCalledNotification;
                BindingCalledNotificationBuilder() : m_result(new BindingCalledNotification()) { }

                template<int STEP> BindingCalledNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<BindingCalledNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::BindingCalledNotification> m_result;
        };

        static BindingCalledNotificationBuilder<0> create() {
            return BindingCalledNotificationBuilder<0>();
        }

    private:
        BindingCalledNotification() {
            m_executionContextId = 0;
        }

        String m_name;
        String m_payload;
        int m_executionContextId;
};


class  ConsoleAPICalledNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(ConsoleAPICalledNotification);
    public:
        static std::unique_ptr<ConsoleAPICalledNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ConsoleAPICalledNotification() override { }

        struct  TypeEnum {
            static const char* Log;
            static const char* Debug;
            static const char* Info;
            static const char* Error;
            static const char* Warning;
            static const char* Dir;
            static const char* Dirxml;
            static const char* Table;
            static const char* Trace;
            static const char* Clear;
            static const char* StartGroup;
            static const char* StartGroupCollapsed;
            static const char* EndGroup;
            static const char* Assert;
            static const char* Profile;
            static const char* ProfileEnd;
            static const char* Count;
            static const char* TimeEnd;
        }; // TypeEnum

        String getType() {
            return m_type;
        }
        void setType(const String& value) {
            m_type = value;
        }

        protocol::Array<protocol::Runtime::RemoteObject>* getArgs() {
            return m_args.get();
        }
        void setArgs(std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> value) {
            m_args = std::move(value);
        }

        int getExecutionContextId() {
            return m_executionContextId;
        }
        void setExecutionContextId(int value) {
            m_executionContextId = value;
        }

        double getTimestamp() {
            return m_timestamp;
        }
        void setTimestamp(double value) {
            m_timestamp = value;
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

        bool hasContext() {
            return m_context.isJust();
        }
        String getContext(const String& defaultValue) {
            return m_context.isJust() ? m_context.fromJust() : defaultValue;
        }
        void setContext(const String& value) {
            m_context = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ConsoleAPICalledNotification> clone() const;

        template<int STATE>
        class ConsoleAPICalledNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TypeSet = 1 << 1,
                    ArgsSet = 1 << 2,
                    ExecutionContextIdSet = 1 << 3,
                    TimestampSet = 1 << 4,
                    AllFieldsSet = (TypeSet | ArgsSet | ExecutionContextIdSet | TimestampSet | 0)
                };


                ConsoleAPICalledNotificationBuilder<STATE | TypeSet>& setType(const String& value) {
                    static_assert(!(STATE & TypeSet), "property type should not be set yet");
                    m_result->setType(value);
                    return castState<TypeSet>();
                }

                ConsoleAPICalledNotificationBuilder<STATE | ArgsSet>& setArgs(std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> value) {
                    static_assert(!(STATE & ArgsSet), "property args should not be set yet");
                    m_result->setArgs(std::move(value));
                    return castState<ArgsSet>();
                }

                ConsoleAPICalledNotificationBuilder<STATE | ExecutionContextIdSet>& setExecutionContextId(int value) {
                    static_assert(!(STATE & ExecutionContextIdSet), "property executionContextId should not be set yet");
                    m_result->setExecutionContextId(value);
                    return castState<ExecutionContextIdSet>();
                }

                ConsoleAPICalledNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value) {
                    static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
                    m_result->setTimestamp(value);
                    return castState<TimestampSet>();
                }

                ConsoleAPICalledNotificationBuilder<STATE>& setStackTrace(std::unique_ptr<protocol::Runtime::StackTrace> value) {
                    m_result->setStackTrace(std::move(value));
                    return *this;
                }

                ConsoleAPICalledNotificationBuilder<STATE>& setContext(const String& value) {
                    m_result->setContext(value);
                    return *this;
                }

                std::unique_ptr<ConsoleAPICalledNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ConsoleAPICalledNotification;
                ConsoleAPICalledNotificationBuilder() : m_result(new ConsoleAPICalledNotification()) { }

                template<int STEP> ConsoleAPICalledNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ConsoleAPICalledNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ConsoleAPICalledNotification> m_result;
        };

        static ConsoleAPICalledNotificationBuilder<0> create() {
            return ConsoleAPICalledNotificationBuilder<0>();
        }

    private:
        ConsoleAPICalledNotification() {
            m_executionContextId = 0;
            m_timestamp = 0;
        }

        String m_type;
        std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> m_args;
        int m_executionContextId;
        double m_timestamp;
        Maybe<protocol::Runtime::StackTrace> m_stackTrace;
        Maybe<String> m_context;
};


class  ExceptionRevokedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(ExceptionRevokedNotification);
    public:
        static std::unique_ptr<ExceptionRevokedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ExceptionRevokedNotification() override { }

        String getReason() {
            return m_reason;
        }
        void setReason(const String& value) {
            m_reason = value;
        }

        int getExceptionId() {
            return m_exceptionId;
        }
        void setExceptionId(int value) {
            m_exceptionId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ExceptionRevokedNotification> clone() const;

        template<int STATE>
        class ExceptionRevokedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ReasonSet = 1 << 1,
                    ExceptionIdSet = 1 << 2,
                    AllFieldsSet = (ReasonSet | ExceptionIdSet | 0)
                };


                ExceptionRevokedNotificationBuilder<STATE | ReasonSet>& setReason(const String& value) {
                    static_assert(!(STATE & ReasonSet), "property reason should not be set yet");
                    m_result->setReason(value);
                    return castState<ReasonSet>();
                }

                ExceptionRevokedNotificationBuilder<STATE | ExceptionIdSet>& setExceptionId(int value) {
                    static_assert(!(STATE & ExceptionIdSet), "property exceptionId should not be set yet");
                    m_result->setExceptionId(value);
                    return castState<ExceptionIdSet>();
                }

                std::unique_ptr<ExceptionRevokedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ExceptionRevokedNotification;
                ExceptionRevokedNotificationBuilder() : m_result(new ExceptionRevokedNotification()) { }

                template<int STEP> ExceptionRevokedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ExceptionRevokedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ExceptionRevokedNotification> m_result;
        };

        static ExceptionRevokedNotificationBuilder<0> create() {
            return ExceptionRevokedNotificationBuilder<0>();
        }

    private:
        ExceptionRevokedNotification() {
            m_exceptionId = 0;
        }

        String m_reason;
        int m_exceptionId;
};


class  ExceptionThrownNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(ExceptionThrownNotification);
    public:
        static std::unique_ptr<ExceptionThrownNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ExceptionThrownNotification() override { }

        double getTimestamp() {
            return m_timestamp;
        }
        void setTimestamp(double value) {
            m_timestamp = value;
        }

        protocol::Runtime::ExceptionDetails* getExceptionDetails() {
            return m_exceptionDetails.get();
        }
        void setExceptionDetails(std::unique_ptr<protocol::Runtime::ExceptionDetails> value) {
            m_exceptionDetails = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ExceptionThrownNotification> clone() const;

        template<int STATE>
        class ExceptionThrownNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    TimestampSet = 1 << 1,
                    ExceptionDetailsSet = 1 << 2,
                    AllFieldsSet = (TimestampSet | ExceptionDetailsSet | 0)
                };


                ExceptionThrownNotificationBuilder<STATE | TimestampSet>& setTimestamp(double value) {
                    static_assert(!(STATE & TimestampSet), "property timestamp should not be set yet");
                    m_result->setTimestamp(value);
                    return castState<TimestampSet>();
                }

                ExceptionThrownNotificationBuilder<STATE | ExceptionDetailsSet>& setExceptionDetails(std::unique_ptr<protocol::Runtime::ExceptionDetails> value) {
                    static_assert(!(STATE & ExceptionDetailsSet), "property exceptionDetails should not be set yet");
                    m_result->setExceptionDetails(std::move(value));
                    return castState<ExceptionDetailsSet>();
                }

                std::unique_ptr<ExceptionThrownNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ExceptionThrownNotification;
                ExceptionThrownNotificationBuilder() : m_result(new ExceptionThrownNotification()) { }

                template<int STEP> ExceptionThrownNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ExceptionThrownNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ExceptionThrownNotification> m_result;
        };

        static ExceptionThrownNotificationBuilder<0> create() {
            return ExceptionThrownNotificationBuilder<0>();
        }

    private:
        ExceptionThrownNotification() {
            m_timestamp = 0;
        }

        double m_timestamp;
        std::unique_ptr<protocol::Runtime::ExceptionDetails> m_exceptionDetails;
};


class  ExecutionContextCreatedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(ExecutionContextCreatedNotification);
    public:
        static std::unique_ptr<ExecutionContextCreatedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ExecutionContextCreatedNotification() override { }

        protocol::Runtime::ExecutionContextDescription* getContext() {
            return m_context.get();
        }
        void setContext(std::unique_ptr<protocol::Runtime::ExecutionContextDescription> value) {
            m_context = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ExecutionContextCreatedNotification> clone() const;

        template<int STATE>
        class ExecutionContextCreatedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ContextSet = 1 << 1,
                    AllFieldsSet = (ContextSet | 0)
                };


                ExecutionContextCreatedNotificationBuilder<STATE | ContextSet>& setContext(std::unique_ptr<protocol::Runtime::ExecutionContextDescription> value) {
                    static_assert(!(STATE & ContextSet), "property context should not be set yet");
                    m_result->setContext(std::move(value));
                    return castState<ContextSet>();
                }

                std::unique_ptr<ExecutionContextCreatedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ExecutionContextCreatedNotification;
                ExecutionContextCreatedNotificationBuilder() : m_result(new ExecutionContextCreatedNotification()) { }

                template<int STEP> ExecutionContextCreatedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ExecutionContextCreatedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ExecutionContextCreatedNotification> m_result;
        };

        static ExecutionContextCreatedNotificationBuilder<0> create() {
            return ExecutionContextCreatedNotificationBuilder<0>();
        }

    private:
        ExecutionContextCreatedNotification() {
        }

        std::unique_ptr<protocol::Runtime::ExecutionContextDescription> m_context;
};


class  ExecutionContextDestroyedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(ExecutionContextDestroyedNotification);
    public:
        static std::unique_ptr<ExecutionContextDestroyedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~ExecutionContextDestroyedNotification() override { }

        int getExecutionContextId() {
            return m_executionContextId;
        }
        void setExecutionContextId(int value) {
            m_executionContextId = value;
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<ExecutionContextDestroyedNotification> clone() const;

        template<int STATE>
        class ExecutionContextDestroyedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ExecutionContextIdSet = 1 << 1,
                    AllFieldsSet = (ExecutionContextIdSet | 0)
                };


                ExecutionContextDestroyedNotificationBuilder<STATE | ExecutionContextIdSet>& setExecutionContextId(int value) {
                    static_assert(!(STATE & ExecutionContextIdSet), "property executionContextId should not be set yet");
                    m_result->setExecutionContextId(value);
                    return castState<ExecutionContextIdSet>();
                }

                std::unique_ptr<ExecutionContextDestroyedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class ExecutionContextDestroyedNotification;
                ExecutionContextDestroyedNotificationBuilder() : m_result(new ExecutionContextDestroyedNotification()) { }

                template<int STEP> ExecutionContextDestroyedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<ExecutionContextDestroyedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::ExecutionContextDestroyedNotification> m_result;
        };

        static ExecutionContextDestroyedNotificationBuilder<0> create() {
            return ExecutionContextDestroyedNotificationBuilder<0>();
        }

    private:
        ExecutionContextDestroyedNotification() {
            m_executionContextId = 0;
        }

        int m_executionContextId;
};


class  InspectRequestedNotification : public Serializable {
        PROTOCOL_DISALLOW_COPY(InspectRequestedNotification);
    public:
        static std::unique_ptr<InspectRequestedNotification> fromValue(protocol::Value* value, ErrorSupport* errors);

        ~InspectRequestedNotification() override { }

        protocol::Runtime::RemoteObject* getObject() {
            return m_object.get();
        }
        void setObject(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
            m_object = std::move(value);
        }

        protocol::DictionaryValue* getHints() {
            return m_hints.get();
        }
        void setHints(std::unique_ptr<protocol::DictionaryValue> value) {
            m_hints = std::move(value);
        }

        std::unique_ptr<protocol::DictionaryValue> toValue() const;
        String serialize() override {
            return toValue()->serialize();
        }
        std::unique_ptr<InspectRequestedNotification> clone() const;

        template<int STATE>
        class InspectRequestedNotificationBuilder {
            public:
                enum {
                    NoFieldsSet = 0,
                    ObjectSet = 1 << 1,
                    HintsSet = 1 << 2,
                    AllFieldsSet = (ObjectSet | HintsSet | 0)
                };


                InspectRequestedNotificationBuilder<STATE | ObjectSet>& setObject(std::unique_ptr<protocol::Runtime::RemoteObject> value) {
                    static_assert(!(STATE & ObjectSet), "property object should not be set yet");
                    m_result->setObject(std::move(value));
                    return castState<ObjectSet>();
                }

                InspectRequestedNotificationBuilder<STATE | HintsSet>& setHints(std::unique_ptr<protocol::DictionaryValue> value) {
                    static_assert(!(STATE & HintsSet), "property hints should not be set yet");
                    m_result->setHints(std::move(value));
                    return castState<HintsSet>();
                }

                std::unique_ptr<InspectRequestedNotification> build() {
                    static_assert(STATE == AllFieldsSet, "state should be AllFieldsSet");
                    return std::move(m_result);
                }

            private:
                friend class InspectRequestedNotification;
                InspectRequestedNotificationBuilder() : m_result(new InspectRequestedNotification()) { }

                template<int STEP> InspectRequestedNotificationBuilder<STATE | STEP>& castState() {
                    return *reinterpret_cast<InspectRequestedNotificationBuilder<STATE | STEP>*>(this);
                }

                std::unique_ptr<protocol::Runtime::InspectRequestedNotification> m_result;
        };

        static InspectRequestedNotificationBuilder<0> create() {
            return InspectRequestedNotificationBuilder<0>();
        }

    private:
        InspectRequestedNotification() {
        }

        std::unique_ptr<protocol::Runtime::RemoteObject> m_object;
        std::unique_ptr<protocol::DictionaryValue> m_hints;
};


// ------------- Backend interface.

class  Backend {
    public:
        virtual ~Backend() { }

        class  AwaitPromiseCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) = 0;
                virtual void sendFailure(const DispatchResponse&) = 0;
                virtual void fallThrough() = 0;
                virtual ~AwaitPromiseCallback() { }
        };
        virtual void awaitPromise(const String& in_promiseObjectId, Maybe<bool> in_returnByValue, Maybe<bool> in_generatePreview, std::unique_ptr<AwaitPromiseCallback> callback) = 0;
        class  CallFunctionOnCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) = 0;
                virtual void sendFailure(const DispatchResponse&) = 0;
                virtual void fallThrough() = 0;
                virtual ~CallFunctionOnCallback() { }
        };
        virtual void callFunctionOn(const String& in_functionDeclaration, Maybe<String> in_objectId, Maybe<protocol::Array<protocol::Runtime::CallArgument>> in_arguments, Maybe<bool> in_silent, Maybe<bool> in_returnByValue, Maybe<bool> in_generatePreview, Maybe<bool> in_userGesture, Maybe<bool> in_awaitPromise, Maybe<int> in_executionContextId, Maybe<String> in_objectGroup, std::unique_ptr<CallFunctionOnCallback> callback) = 0;
        virtual DispatchResponse compileScript(const String& in_expression, const String& in_sourceURL, bool in_persistScript, Maybe<int> in_executionContextId, Maybe<String>* out_scriptId, Maybe<protocol::Runtime::ExceptionDetails>* out_exceptionDetails) = 0;
        virtual DispatchResponse disable() = 0;
        virtual DispatchResponse discardConsoleEntries() = 0;
        virtual DispatchResponse enable() = 0;
        class  EvaluateCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) = 0;
                virtual void sendFailure(const DispatchResponse&) = 0;
                virtual void fallThrough() = 0;
                virtual ~EvaluateCallback() { }
        };
        virtual void evaluate(const String& in_expression, Maybe<String> in_objectGroup, Maybe<bool> in_includeCommandLineAPI, Maybe<bool> in_silent, Maybe<int> in_contextId, Maybe<bool> in_returnByValue, Maybe<bool> in_generatePreview, Maybe<bool> in_userGesture, Maybe<bool> in_awaitPromise, Maybe<bool> in_throwOnSideEffect, Maybe<double> in_timeout, std::unique_ptr<EvaluateCallback> callback) = 0;
        virtual DispatchResponse getIsolateId(String* out_id) = 0;
        virtual DispatchResponse getHeapUsage(double* out_usedSize, double* out_totalSize) = 0;
        virtual DispatchResponse getProperties(const String& in_objectId, Maybe<bool> in_ownProperties, Maybe<bool> in_accessorPropertiesOnly, Maybe<bool> in_generatePreview, std::unique_ptr<protocol::Array<protocol::Runtime::PropertyDescriptor>>* out_result, Maybe<protocol::Array<protocol::Runtime::InternalPropertyDescriptor>>* out_internalProperties, Maybe<protocol::Runtime::ExceptionDetails>* out_exceptionDetails) = 0;
        virtual DispatchResponse globalLexicalScopeNames(Maybe<int> in_executionContextId, std::unique_ptr<protocol::Array<String>>* out_names) = 0;
        virtual DispatchResponse queryObjects(const String& in_prototypeObjectId, Maybe<String> in_objectGroup, std::unique_ptr<protocol::Runtime::RemoteObject>* out_objects) = 0;
        virtual DispatchResponse releaseObject(const String& in_objectId) = 0;
        virtual DispatchResponse releaseObjectGroup(const String& in_objectGroup) = 0;
        virtual DispatchResponse runIfWaitingForDebugger() = 0;
        class  RunScriptCallback {
            public:
                virtual void sendSuccess(std::unique_ptr<protocol::Runtime::RemoteObject> result, Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails) = 0;
                virtual void sendFailure(const DispatchResponse&) = 0;
                virtual void fallThrough() = 0;
                virtual ~RunScriptCallback() { }
        };
        virtual void runScript(const String& in_scriptId, Maybe<int> in_executionContextId, Maybe<String> in_objectGroup, Maybe<bool> in_silent, Maybe<bool> in_includeCommandLineAPI, Maybe<bool> in_returnByValue, Maybe<bool> in_generatePreview, Maybe<bool> in_awaitPromise, std::unique_ptr<RunScriptCallback> callback) = 0;
        virtual DispatchResponse setCustomObjectFormatterEnabled(bool in_enabled) = 0;
        virtual DispatchResponse setMaxCallStackSizeToCapture(int in_size) = 0;
        class  TerminateExecutionCallback {
            public:
                virtual void sendSuccess() = 0;
                virtual void sendFailure(const DispatchResponse&) = 0;
                virtual void fallThrough() = 0;
                virtual ~TerminateExecutionCallback() { }
        };
        virtual void terminateExecution(std::unique_ptr<TerminateExecutionCallback> callback) = 0;
        virtual DispatchResponse addBinding(const String& in_name, Maybe<int> in_executionContextId) = 0;
        virtual DispatchResponse removeBinding(const String& in_name) = 0;

};

// ------------- Frontend interface.

class  Frontend {
    public:
        explicit Frontend(FrontendChannel* frontendChannel) : m_frontendChannel(frontendChannel) { }
        void bindingCalled(const String& name, const String& payload, int executionContextId);
        void consoleAPICalled(const String& type, std::unique_ptr<protocol::Array<protocol::Runtime::RemoteObject>> args, int executionContextId, double timestamp, Maybe<protocol::Runtime::StackTrace> stackTrace = Maybe<protocol::Runtime::StackTrace>(), Maybe<String> context = Maybe<String>());
        void exceptionRevoked(const String& reason, int exceptionId);
        void exceptionThrown(double timestamp, std::unique_ptr<protocol::Runtime::ExceptionDetails> exceptionDetails);
        void executionContextCreated(std::unique_ptr<protocol::Runtime::ExecutionContextDescription> context);
        void executionContextDestroyed(int executionContextId);
        void executionContextsCleared();
        void inspectRequested(std::unique_ptr<protocol::Runtime::RemoteObject> object, std::unique_ptr<protocol::DictionaryValue> hints);

        void flush();
        void sendRawNotification(const String&);
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
