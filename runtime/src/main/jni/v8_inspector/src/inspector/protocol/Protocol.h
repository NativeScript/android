// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Collections_h
#define v8_inspector_protocol_Collections_h

#include "src/inspector/protocol/Forward.h"
#include <cstddef>

#if defined(__APPLE__) && !defined(_LIBCPP_VERSION)
#include <map>
#include <set>

namespace v8_inspector {
namespace protocol {

template <class Key, class T> using HashMap = std::map<Key, T>;
template <class Key> using HashSet = std::set<Key>;

} // namespace v8_inspector
} // namespace protocol

#else
#include <unordered_map>
#include <unordered_set>

namespace v8_inspector {
namespace protocol {

template <class Key, class T> using HashMap = std::unordered_map<Key, T>;
template <class Key> using HashSet = std::unordered_set<Key>;

} // namespace v8_inspector
} // namespace protocol

#endif // defined(__APPLE__) && !defined(_LIBCPP_VERSION)

#endif // !defined(v8_inspector_protocol_Collections_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_ErrorSupport_h
#define v8_inspector_protocol_ErrorSupport_h

//#include "Forward.h"

namespace v8_inspector {
namespace protocol {

class  ErrorSupport {
public:
    ErrorSupport();
    ErrorSupport(String* errorString);
    ~ErrorSupport();

    void push();
    void setName(const String&);
    void pop();
    void addError(const String&);
    bool hasErrors();
    String errors();

private:
    std::vector<String> m_path;
    std::vector<String> m_errors;
    String* m_errorString;
};

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_ErrorSupport_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Values_h
#define v8_inspector_protocol_Values_h

//#include "Allocator.h"
//#include "Collections.h"
//#include "Forward.h"

namespace v8_inspector {
namespace protocol {

class ListValue;
class DictionaryValue;
class Value;

class  Value {
    PROTOCOL_DISALLOW_COPY(Value);
public:
    virtual ~Value() { }

    static std::unique_ptr<Value> null()
    {
        return wrapUnique(new Value());
    }

    enum ValueType {
        TypeNull = 0,
        TypeBoolean,
        TypeInteger,
        TypeDouble,
        TypeString,
        TypeObject,
        TypeArray,
        TypeSerialized
    };

    ValueType type() const { return m_type; }

    bool isNull() const { return m_type == TypeNull; }

    virtual bool asBoolean(bool* output) const;
    virtual bool asDouble(double* output) const;
    virtual bool asInteger(int* output) const;
    virtual bool asString(String* output) const;
    virtual bool asSerialized(String* output) const;

    String toJSONString() const;
    virtual void writeJSON(StringBuilder* output) const;
    virtual std::unique_ptr<Value> clone() const;

protected:
    Value() : m_type(TypeNull) { }
    explicit Value(ValueType type) : m_type(type) { }

private:
    friend class DictionaryValue;
    friend class ListValue;

    ValueType m_type;
};

class  FundamentalValue : public Value {
public:
    static std::unique_ptr<FundamentalValue> create(bool value)
    {
        return wrapUnique(new FundamentalValue(value));
    }

    static std::unique_ptr<FundamentalValue> create(int value)
    {
        return wrapUnique(new FundamentalValue(value));
    }

    static std::unique_ptr<FundamentalValue> create(double value)
    {
        return wrapUnique(new FundamentalValue(value));
    }

    bool asBoolean(bool* output) const override;
    bool asDouble(double* output) const override;
    bool asInteger(int* output) const override;
    void writeJSON(StringBuilder* output) const override;
    std::unique_ptr<Value> clone() const override;

private:
    explicit FundamentalValue(bool value) : Value(TypeBoolean), m_boolValue(value) { }
    explicit FundamentalValue(int value) : Value(TypeInteger), m_integerValue(value) { }
    explicit FundamentalValue(double value) : Value(TypeDouble), m_doubleValue(value) { }

    union {
        bool m_boolValue;
        double m_doubleValue;
        int m_integerValue;
    };
};

class  StringValue : public Value {
public:
    static std::unique_ptr<StringValue> create(const String& value)
    {
        return wrapUnique(new StringValue(value));
    }

    static std::unique_ptr<StringValue> create(const char* value)
    {
        return wrapUnique(new StringValue(value));
    }

    bool asString(String* output) const override;
    void writeJSON(StringBuilder* output) const override;
    std::unique_ptr<Value> clone() const override;

private:
    explicit StringValue(const String& value) : Value(TypeString), m_stringValue(value) { }
    explicit StringValue(const char* value) : Value(TypeString), m_stringValue(value) { }

    String m_stringValue;
};

class  SerializedValue : public Value {
public:
    static std::unique_ptr<SerializedValue> create(const String& value)
    {
        return wrapUnique(new SerializedValue(value));
    }

    bool asSerialized(String* output) const override;
    void writeJSON(StringBuilder* output) const override;
    std::unique_ptr<Value> clone() const override;

private:
    explicit SerializedValue(const String& value) : Value(TypeSerialized), m_serializedValue(value) { }

    String m_serializedValue;
};

class  DictionaryValue : public Value {
public:
    using Entry = std::pair<String, Value*>;
    static std::unique_ptr<DictionaryValue> create()
    {
        return wrapUnique(new DictionaryValue());
    }

    static DictionaryValue* cast(Value* value)
    {
        if (!value || value->type() != TypeObject)
            return nullptr;
        return static_cast<DictionaryValue*>(value);
    }

    static std::unique_ptr<DictionaryValue> cast(std::unique_ptr<Value> value)
    {
        return wrapUnique(DictionaryValue::cast(value.release()));
    }

    void writeJSON(StringBuilder* output) const override;
    std::unique_ptr<Value> clone() const override;

    size_t size() const { return m_data.size(); }

    void setBoolean(const String& name, bool);
    void setInteger(const String& name, int);
    void setDouble(const String& name, double);
    void setString(const String& name, const String&);
    void setValue(const String& name, std::unique_ptr<Value>);
    void setObject(const String& name, std::unique_ptr<DictionaryValue>);
    void setArray(const String& name, std::unique_ptr<ListValue>);

    bool getBoolean(const String& name, bool* output) const;
    bool getInteger(const String& name, int* output) const;
    bool getDouble(const String& name, double* output) const;
    bool getString(const String& name, String* output) const;

    DictionaryValue* getObject(const String& name) const;
    ListValue* getArray(const String& name) const;
    Value* get(const String& name) const;
    Entry at(size_t index) const;

    bool booleanProperty(const String& name, bool defaultValue) const;
    int integerProperty(const String& name, int defaultValue) const;
    double doubleProperty(const String& name, double defaultValue) const;
    void remove(const String& name);

    ~DictionaryValue() override;

private:
    DictionaryValue();
    template<typename T>
    void set(const String& key, std::unique_ptr<T>& value)
    {
        DCHECK(value);
        bool isNew = m_data.find(key) == m_data.end();
        m_data[key] = std::move(value);
        if (isNew)
            m_order.push_back(key);
    }

    using Dictionary = protocol::HashMap<String, std::unique_ptr<Value>>;
    Dictionary m_data;
    std::vector<String> m_order;
};

class  ListValue : public Value {
public:
    static std::unique_ptr<ListValue> create()
    {
        return wrapUnique(new ListValue());
    }

    static ListValue* cast(Value* value)
    {
        if (!value || value->type() != TypeArray)
            return nullptr;
        return static_cast<ListValue*>(value);
    }

    static std::unique_ptr<ListValue> cast(std::unique_ptr<Value> value)
    {
        return wrapUnique(ListValue::cast(value.release()));
    }

    ~ListValue() override;

    void writeJSON(StringBuilder* output) const override;
    std::unique_ptr<Value> clone() const override;

    void pushValue(std::unique_ptr<Value>);

    Value* at(size_t index);
    size_t size() const { return m_data.size(); }

private:
    ListValue();
    std::vector<std::unique_ptr<Value>> m_data;
};

} // namespace v8_inspector
} // namespace protocol

#endif // v8_inspector_protocol_Values_h


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Object_h
#define v8_inspector_protocol_Object_h

//#include "ErrorSupport.h"
//#include "Forward.h"
//#include "Values.h"

namespace v8_inspector {
namespace protocol {

class  Object {
public:
    static std::unique_ptr<Object> parse(protocol::Value*, ErrorSupport*);
    ~Object();

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<Object> clone() const;
private:
    explicit Object(std::unique_ptr<protocol::DictionaryValue>);
    std::unique_ptr<protocol::DictionaryValue> m_object;
};

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Object_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_ValueConversions_h
#define v8_inspector_protocol_ValueConversions_h

//#include "ErrorSupport.h"
//#include "Forward.h"
//#include "Values.h"

namespace v8_inspector {
namespace protocol {

template<typename T>
struct ValueConversions {
    static std::unique_ptr<T> parse(protocol::Value* value, ErrorSupport* errors)
    {
        return T::parse(value, errors);
    }

    static std::unique_ptr<protocol::Value> serialize(T* value)
    {
        return value->serialize();
    }

    static std::unique_ptr<protocol::Value> serialize(const std::unique_ptr<T>& value)
    {
        return value->serialize();
    }
};

template<>
struct ValueConversions<bool> {
    static bool parse(protocol::Value* value, ErrorSupport* errors)
    {
        bool result = false;
        bool success = value ? value->asBoolean(&result) : false;
        if (!success)
            errors->addError("boolean value expected");
        return result;
    }

    static std::unique_ptr<protocol::Value> serialize(bool value)
    {
        return FundamentalValue::create(value);
    }
};

template<>
struct ValueConversions<int> {
    static int parse(protocol::Value* value, ErrorSupport* errors)
    {
        int result = 0;
        bool success = value ? value->asInteger(&result) : false;
        if (!success)
            errors->addError("integer value expected");
        return result;
    }

    static std::unique_ptr<protocol::Value> serialize(int value)
    {
        return FundamentalValue::create(value);
    }
};

template<>
struct ValueConversions<double> {
    static double parse(protocol::Value* value, ErrorSupport* errors)
    {
        double result = 0;
        bool success = value ? value->asDouble(&result) : false;
        if (!success)
            errors->addError("double value expected");
        return result;
    }

    static std::unique_ptr<protocol::Value> serialize(double value)
    {
        return FundamentalValue::create(value);
    }
};

template<>
struct ValueConversions<String> {
    static String parse(protocol::Value* value, ErrorSupport* errors)
    {
        String result;
        bool success = value ? value->asString(&result) : false;
        if (!success)
            errors->addError("string value expected");
        return result;
    }

    static std::unique_ptr<protocol::Value> serialize(const String& value)
    {
        return StringValue::create(value);
    }
};

template<>
struct ValueConversions<Value> {
    static std::unique_ptr<Value> parse(protocol::Value* value, ErrorSupport* errors)
    {
        bool success = !!value;
        if (!success) {
            errors->addError("value expected");
            return nullptr;
        }
        return value->clone();
    }

    static std::unique_ptr<protocol::Value> serialize(Value* value)
    {
        return value->clone();
    }

    static std::unique_ptr<protocol::Value> serialize(const std::unique_ptr<Value>& value)
    {
        return value->clone();
    }
};

template<>
struct ValueConversions<DictionaryValue> {
    static std::unique_ptr<DictionaryValue> parse(protocol::Value* value, ErrorSupport* errors)
    {
        bool success = value && value->type() == protocol::Value::TypeObject;
        if (!success)
            errors->addError("object expected");
        return DictionaryValue::cast(value->clone());
    }

    static std::unique_ptr<protocol::Value> serialize(DictionaryValue* value)
    {
        return value->clone();
    }

    static std::unique_ptr<protocol::Value> serialize(const std::unique_ptr<DictionaryValue>& value)
    {
        return value->clone();
    }
};

template<>
struct ValueConversions<ListValue> {
    static std::unique_ptr<ListValue> parse(protocol::Value* value, ErrorSupport* errors)
    {
        bool success = value && value->type() == protocol::Value::TypeArray;
        if (!success)
            errors->addError("list expected");
        return ListValue::cast(value->clone());
    }

    static std::unique_ptr<protocol::Value> serialize(ListValue* value)
    {
        return value->clone();
    }

    static std::unique_ptr<protocol::Value> serialize(const std::unique_ptr<ListValue>& value)
    {
        return value->clone();
    }
};

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_ValueConversions_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Maybe_h
#define v8_inspector_protocol_Maybe_h

//#include "Forward.h"

namespace v8_inspector {
namespace protocol {

template<typename T>
class Maybe {
public:
    Maybe() : m_value() { }
    Maybe(std::unique_ptr<T> value) : m_value(std::move(value)) { }
    void operator=(std::unique_ptr<T> value) { m_value = std::move(value); }
    T* fromJust() const { DCHECK(m_value); return m_value.get(); }
    T* fromMaybe(T* defaultValue) const { return m_value ? m_value.get() : defaultValue; }
    bool isJust() const { return !!m_value; }
    std::unique_ptr<T> takeJust() { DCHECK(m_value); return m_value.release(); }
private:
    std::unique_ptr<T> m_value;
};

template<typename T>
class MaybeBase {
public:
    MaybeBase() : m_isJust(false) { }
    MaybeBase(T value) : m_isJust(true), m_value(value) { }
    void operator=(T value) { m_value = value; m_isJust = true; }
    T fromJust() const { DCHECK(m_isJust); return m_value; }
    T fromMaybe(const T& defaultValue) const { return m_isJust ? m_value : defaultValue; }
    bool isJust() const { return m_isJust; }
    T takeJust() { DCHECK(m_isJust); return m_value; }

protected:
    bool m_isJust;
    T m_value;
};

template<>
class Maybe<bool> : public MaybeBase<bool> {
public:
    Maybe() { }
    Maybe(bool value) : MaybeBase(value) { }
    using MaybeBase::operator=;
};

template<>
class Maybe<int> : public MaybeBase<int> {
public:
    Maybe() { }
    Maybe(int value) : MaybeBase(value) { }
    using MaybeBase::operator=;
};

template<>
class Maybe<double> : public MaybeBase<double> {
public:
    Maybe() { }
    Maybe(double value) : MaybeBase(value) { }
    using MaybeBase::operator=;
};

template<>
class Maybe<String> : public MaybeBase<String> {
public:
    Maybe() { }
    Maybe(const String& value) : MaybeBase(value) { }
    using MaybeBase::operator=;
};

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Maybe_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Array_h
#define v8_inspector_protocol_Array_h

//#include "ErrorSupport.h"
//#include "Forward.h"
//#include "ValueConversions.h"
//#include "Values.h"

namespace v8_inspector {
namespace protocol {

template<typename T>
class Array {
public:
    static std::unique_ptr<Array<T>> create()
    {
        return wrapUnique(new Array<T>());
    }

    static std::unique_ptr<Array<T>> parse(protocol::Value* value, ErrorSupport* errors)
    {
        protocol::ListValue* array = ListValue::cast(value);
        if (!array) {
            errors->addError("array expected");
            return nullptr;
        }
        std::unique_ptr<Array<T>> result(new Array<T>());
        errors->push();
        for (size_t i = 0; i < array->size(); ++i) {
            errors->setName(StringUtil::fromInteger(i));
            std::unique_ptr<T> item = ValueConversions<T>::parse(array->at(i), errors);
            result->m_vector.push_back(std::move(item));
        }
        errors->pop();
        if (errors->hasErrors())
            return nullptr;
        return result;
    }

    void addItem(std::unique_ptr<T> value)
    {
        m_vector.push_back(std::move(value));
    }

    size_t length()
    {
        return m_vector.size();
    }

    T* get(size_t index)
    {
        return m_vector[index].get();
    }

    std::unique_ptr<protocol::ListValue> serialize()
    {
        std::unique_ptr<protocol::ListValue> result = ListValue::create();
        for (auto& item : m_vector)
            result->pushValue(ValueConversions<T>::serialize(item));
        return result;
    }

private:
    std::vector<std::unique_ptr<T>> m_vector;
};

template<typename T>
class ArrayBase {
public:
    static std::unique_ptr<Array<T>> create()
    {
        return wrapUnique(new Array<T>());
    }

    static std::unique_ptr<Array<T>> parse(protocol::Value* value, ErrorSupport* errors)
    {
        protocol::ListValue* array = ListValue::cast(value);
        if (!array) {
            errors->addError("array expected");
            return nullptr;
        }
        errors->push();
        std::unique_ptr<Array<T>> result(new Array<T>());
        for (size_t i = 0; i < array->size(); ++i) {
            errors->setName(StringUtil::fromInteger(i));
            T item = ValueConversions<T>::parse(array->at(i), errors);
            result->m_vector.push_back(item);
        }
        errors->pop();
        if (errors->hasErrors())
            return nullptr;
        return result;
    }

    void addItem(const T& value)
    {
        m_vector.push_back(value);
    }

    size_t length()
    {
        return m_vector.size();
    }

    T get(size_t index)
    {
        return m_vector[index];
    }

    std::unique_ptr<protocol::ListValue> serialize()
    {
        std::unique_ptr<protocol::ListValue> result = ListValue::create();
        for (auto& item : m_vector)
            result->pushValue(ValueConversions<T>::serialize(item));
        return result;
    }

private:
    std::vector<T> m_vector;
};

template<> class Array<String> : public ArrayBase<String> {};
template<> class Array<int> : public ArrayBase<int> {};
template<> class Array<double> : public ArrayBase<double> {};
template<> class Array<bool> : public ArrayBase<bool> {};

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Array_h)


// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_BackendCallback_h
#define v8_inspector_protocol_BackendCallback_h

//#include "Forward.h"

namespace v8_inspector {
namespace protocol {

class  BackendCallback {
public:
    virtual ~BackendCallback() { }
    virtual void sendFailure(const ErrorString&) = 0;
};

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_BackendCallback_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_DispatcherBase_h
#define v8_inspector_protocol_DispatcherBase_h

//#include "BackendCallback.h"
//#include "Collections.h"
//#include "ErrorSupport.h"
//#include "Forward.h"
//#include "Values.h"

namespace v8_inspector {
namespace protocol {

class WeakPtr;

class  DispatcherBase {
    PROTOCOL_DISALLOW_COPY(DispatcherBase);
public:
    static const char kInvalidRequest[];
    class  WeakPtr {
    public:
        explicit WeakPtr(DispatcherBase*);
        ~WeakPtr();
        DispatcherBase* get() { return m_dispatcher; }
        void dispose() { m_dispatcher = nullptr; }

    private:
        DispatcherBase* m_dispatcher;
    };

    class  Callback : public protocol::BackendCallback {
    public:
        Callback(std::unique_ptr<WeakPtr> backendImpl, int callId);
        virtual ~Callback();
        void dispose();

    protected:
        void sendIfActive(std::unique_ptr<protocol::DictionaryValue> partialMessage, const ErrorString& invocationError);

    private:
        std::unique_ptr<WeakPtr> m_backendImpl;
        int m_callId;
    };

    explicit DispatcherBase(FrontendChannel*);
    virtual ~DispatcherBase();

    enum CommonErrorCode {
        ParseError = -32700,
        InvalidRequest = -32600,
        MethodNotFound = -32601,
        InvalidParams = -32602,
        InternalError = -32603,
        ServerError = -32000,
    };

    static bool getCommandName(const String& message, String* result);

    virtual void dispatch(int callId, const String& method, std::unique_ptr<protocol::DictionaryValue> messageObject) = 0;

    void sendResponse(int callId, const ErrorString&, ErrorSupport*, std::unique_ptr<protocol::DictionaryValue> result);
    void sendResponse(int callId, const ErrorString&, std::unique_ptr<protocol::DictionaryValue> result);
    void sendResponse(int callId, const ErrorString&);

    void reportProtocolError(int callId, CommonErrorCode, const String& errorMessage, ErrorSupport* errors);
    void clearFrontend();

    std::unique_ptr<WeakPtr> weakPtr();

private:
    FrontendChannel* m_frontendChannel;
    protocol::HashSet<WeakPtr*> m_weakPtrs;
};

class  UberDispatcher {
    PROTOCOL_DISALLOW_COPY(UberDispatcher);
public:
    explicit UberDispatcher(FrontendChannel*);
    void registerBackend(const String& name, std::unique_ptr<protocol::DispatcherBase>);
    void dispatch(std::unique_ptr<Value> message);
    FrontendChannel* channel() { return m_frontendChannel; }
    virtual ~UberDispatcher();

private:
    FrontendChannel* m_frontendChannel;
    protocol::HashMap<String, std::unique_ptr<protocol::DispatcherBase>> m_dispatchers;
};

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_DispatcherBase_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef v8_inspector_protocol_Parser_h
#define v8_inspector_protocol_Parser_h

//#include "Forward.h"
//#include "Values.h"

namespace v8_inspector {
namespace protocol {

 std::unique_ptr<Value> parseJSON(const uint8_t*, unsigned);
 std::unique_ptr<Value> parseJSON(const uint16_t*, unsigned);

} // namespace v8_inspector
} // namespace protocol

#endif // !defined(v8_inspector_protocol_Parser_h)
