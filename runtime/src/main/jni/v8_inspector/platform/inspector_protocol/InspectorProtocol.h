// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Allocator_h
#define Allocator_h

enum NotNullTagEnum { NotNullLiteral };

#define PROTOCOL_DISALLOW_NEW()                                 \
    private:                                                    \
        void* operator new(size_t) = delete;                    \
        void* operator new(size_t, NotNullTagEnum, void*) = delete; \
        void* operator new(size_t, void*) = delete;             \
    public:

#define PROTOCOL_DISALLOW_COPY(ClassName) \
    private: \
        ClassName(const ClassName&) = delete; \
        ClassName& operator=(const ClassName&) = delete

#endif /* Allocator_h */


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef protocol_Platform_h
#define protocol_Platform_h

#include "platform/v8_inspector/PlatformSTL.h"

#endif // !defined(protocol_Platform_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Collections_h
#define Collections_h

#include <cstddef>

#if defined(__APPLE__) && !defined(_LIBCPP_VERSION)
#include <map>
#include <set>

namespace blink {
namespace protocol {

template <class Key, class T> using HashMap = std::map<Key, T>;
template <class Key> using HashSet = std::set<Key>;

} // namespace protocol
} // namespace blink

#else
#include <unordered_map>
#include <unordered_set>

namespace blink {
namespace protocol {

template <class Key, class T> using HashMap = std::unordered_map<Key, T>;
template <class Key> using HashSet = std::unordered_set<Key>;

} // namespace protocol
} // namespace blink

#endif // defined(__APPLE__) && !defined(_LIBCPP_VERSION)

// Macro that returns a compile time constant with the length of an array, but gives an error if passed a non-array.
template<typename T, std::size_t Size> char (&ArrayLengthHelperFunction(T (&)[Size]))[Size];
// GCC needs some help to deduce a 0 length array.
#if defined(__GNUC__)
template<typename T> char (&ArrayLengthHelperFunction(T (&)[0]))[0];
#endif
#define PROTOCOL_ARRAY_LENGTH(array) sizeof(::ArrayLengthHelperFunction(array))

#endif // !defined(Collections_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef String16_h
#define String16_h

//#include "Collections.h"
//#include "Platform.h"
#include "platform/PlatformExport.h"

#include <vector>

namespace blink {
namespace protocol {

namespace internal {
PLATFORM_EXPORT void intToStr(int, char*, size_t);
PLATFORM_EXPORT void doubleToStr(double, char*, size_t);
PLATFORM_EXPORT void doubleToStr3(double, char*, size_t);
PLATFORM_EXPORT void doubleToStr6(double, char*, size_t);
PLATFORM_EXPORT double strToDouble(const char*, bool*);
PLATFORM_EXPORT int strToInt(const char*, bool*);
} // namespace internal

template <typename T, typename C>
class PLATFORM_EXPORT String16Base {
public:
    static bool isASCII(C c)
    {
        return !(c & ~0x7F);
    }

    static bool isSpaceOrNewLine(C c)
    {
        return isASCII(c) && c <= ' ' && (c == ' ' || (c <= 0xD && c >= 0x9));
    }

    static T fromInteger(int number)
    {
        char buffer[50];
        internal::intToStr(number, buffer, PROTOCOL_ARRAY_LENGTH(buffer));
        return T(buffer);
    }

    static T fromDouble(double number)
    {
        char buffer[100];
        internal::doubleToStr(number, buffer, PROTOCOL_ARRAY_LENGTH(buffer));
        return T(buffer);
    }

    static T fromDoublePrecision3(double number)
    {
        char buffer[100];
        internal::doubleToStr3(number, buffer, PROTOCOL_ARRAY_LENGTH(buffer));
        return T(buffer);
    }

    static T fromDoublePrecision6(double number)
    {
        char buffer[100];
        internal::doubleToStr6(number, buffer, PROTOCOL_ARRAY_LENGTH(buffer));
        return T(buffer);
    }

    static double charactersToDouble(const C* characters, size_t length, bool* ok = nullptr)
    {
        std::vector<char> buffer;
        buffer.reserve(length + 1);
        for (size_t i = 0; i < length; ++i) {
            if (!isASCII(characters[i])) {
                if (ok)
                    *ok = false;
                return 0;
            }
            buffer.push_back(static_cast<char>(characters[i]));
        }
        buffer.push_back('\0');
        return internal::strToDouble(buffer.data(), ok);
    }

    static int charactersToInteger(const C* characters, size_t length, bool* ok = nullptr)
    {
        std::vector<char> buffer;
        buffer.reserve(length + 1);
        for (size_t i = 0; i < length; ++i) {
            if (!isASCII(characters[i])) {
                if (ok)
                    *ok = false;
                return 0;
            }
            buffer.push_back(static_cast<char>(characters[i]));
        }
        buffer.push_back('\0');
        return internal::strToInt(buffer.data(), ok);
    }

    double toDouble(bool* ok = nullptr) const
    {
        const C* characters = static_cast<const T&>(*this).characters16();
        size_t length = static_cast<const T&>(*this).length();
        return charactersToDouble(characters, length, ok);
    }

    int toInteger(bool* ok = nullptr) const
    {
        const C* characters = static_cast<const T&>(*this).characters16();
        size_t length = static_cast<const T&>(*this).length();
        return charactersToInteger(characters, length, ok);
    }

    T stripWhiteSpace() const
    {
        size_t length = static_cast<const T&>(*this).length();
        if (!length)
            return T();

        unsigned start = 0;
        unsigned end = length - 1;
        const C* characters = static_cast<const T&>(*this).characters16();

        // skip white space from start
        while (start <= end && isSpaceOrNewLine(characters[start]))
            ++start;

        // only white space
        if (start > end)
            return T();

        // skip white space from end
        while (end && isSpaceOrNewLine(characters[end]))
            --end;

        if (!start && end == length - 1)
            return T(static_cast<const T&>(*this));
        return T(characters + start, end + 1 - start);
    }

    bool startsWith(const char* prefix) const
    {
        const C* characters = static_cast<const T&>(*this).characters16();
        size_t length = static_cast<const T&>(*this).length();
        for (size_t i = 0, j = 0; prefix[j] && i < length; ++i, ++j) {
            if (characters[i] != prefix[j])
                return false;
        }
        return true;
    }
};

} // namespace protocol
} // namespace blink

#include "platform/v8_inspector/String16STL.h"

namespace blink {
namespace protocol {

class PLATFORM_EXPORT String16Builder {
public:
    String16Builder();
    void append(const String16&);
    void append(UChar);
    void append(char);
    void append(const UChar*, size_t);
    void append(const char*, size_t);
    String16 toString();
    void reserveCapacity(size_t);

private:
    std::vector<UChar> m_buffer;
};

} // namespace protocol
} // namespace blink

using String16 = blink::protocol::String16;
using String16Builder = blink::protocol::String16Builder;

#endif // !defined(String16_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ErrorSupport_h
#define ErrorSupport_h

//#include "Platform.h"
//#include "String16.h"
#include "platform/PlatformExport.h"

#include <vector>

namespace blink {
namespace protocol {

using ErrorString = String16;

class PLATFORM_EXPORT ErrorSupport {
public:
    ErrorSupport();
    ErrorSupport(String16* errorString);
    ~ErrorSupport();

    void push();
    void setName(const String16&);
    void pop();
    void addError(const String16&);
    bool hasErrors();
    String16 errors();

private:
    std::vector<String16> m_path;
    std::vector<String16> m_errors;
    String16* m_errorString;
};

} // namespace platform
} // namespace blink

using blink::protocol::ErrorString;

#endif // !defined(ErrorSupport_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Values_h
#define Values_h

//#include "Allocator.h"
//#include "Collections.h"
//#include "Platform.h"
//#include "String16.h"
#include "platform/PlatformExport.h"

#include <vector>

namespace blink {
namespace protocol {

class ListValue;
class DictionaryValue;
class Value;

class PLATFORM_EXPORT Value {
    PROTOCOL_DISALLOW_COPY(Value);
public:
    static const int maxDepth = 1000;

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
    virtual bool asString(String16* output) const;
    virtual bool asSerialized(String16* output) const;

    String16 toJSONString() const;
    virtual void writeJSON(String16Builder* output) const;
    virtual std::unique_ptr<Value> clone() const;

protected:
    Value() : m_type(TypeNull) { }
    explicit Value(ValueType type) : m_type(type) { }

private:
    friend class DictionaryValue;
    friend class ListValue;

    ValueType m_type;
};

class PLATFORM_EXPORT FundamentalValue : public Value {
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
    void writeJSON(String16Builder* output) const override;
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

class PLATFORM_EXPORT StringValue : public Value {
public:
    static std::unique_ptr<StringValue> create(const String16& value)
    {
        return wrapUnique(new StringValue(value));
    }

    static std::unique_ptr<StringValue> create(const char* value)
    {
        return wrapUnique(new StringValue(value));
    }

    bool asString(String16* output) const override;
    void writeJSON(String16Builder* output) const override;
    std::unique_ptr<Value> clone() const override;

private:
    explicit StringValue(const String16& value) : Value(TypeString), m_stringValue(value) { }
    explicit StringValue(const char* value) : Value(TypeString), m_stringValue(value) { }

    String16 m_stringValue;
};

class PLATFORM_EXPORT SerializedValue : public Value {
public:
    static std::unique_ptr<SerializedValue> create(const String16& value)
    {
        return wrapUnique(new SerializedValue(value));
    }

    bool asSerialized(String16* output) const override;
    void writeJSON(String16Builder* output) const override;
    std::unique_ptr<Value> clone() const override;

private:
    explicit SerializedValue(const String16& value) : Value(TypeSerialized), m_serializedValue(value) { }
    explicit SerializedValue(const char* value) : Value(TypeSerialized), m_serializedValue(value) { }

    String16 m_serializedValue;
};

class PLATFORM_EXPORT DictionaryValue : public Value {
public:
    using Entry = std::pair<String16, Value*>;
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

    void writeJSON(String16Builder* output) const override;
    std::unique_ptr<Value> clone() const override;

    size_t size() const { return m_data.size(); }

    void setBoolean(const String16& name, bool);
    void setInteger(const String16& name, int);
    void setDouble(const String16& name, double);
    void setString(const String16& name, const String16&);
    void setValue(const String16& name, std::unique_ptr<Value>);
    void setObject(const String16& name, std::unique_ptr<DictionaryValue>);
    void setArray(const String16& name, std::unique_ptr<ListValue>);

    bool getBoolean(const String16& name, bool* output) const;
    bool getInteger(const String16& name, int* output) const;
    bool getDouble(const String16& name, double* output) const;
    bool getString(const String16& name, String16* output) const;

    DictionaryValue* getObject(const String16& name) const;
    ListValue* getArray(const String16& name) const;
    Value* get(const String16& name) const;
    Entry at(size_t index) const;

    bool booleanProperty(const String16& name, bool defaultValue) const;
    int integerProperty(const String16& name, int defaultValue) const;
    double doubleProperty(const String16& name, double defaultValue) const;
    void remove(const String16& name);

    ~DictionaryValue() override;

private:
    DictionaryValue();
    template<typename T>
    void set(const String16& key, std::unique_ptr<T>& value)
    {
        DCHECK(value);
        bool isNew = m_data.find(key) == m_data.end();
        m_data[key] = std::move(value);
        if (isNew)
            m_order.push_back(key);
    }

    using Dictionary = protocol::HashMap<String16, std::unique_ptr<Value>>;
    Dictionary m_data;
    std::vector<String16> m_order;
};

class PLATFORM_EXPORT ListValue : public Value {
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

    void writeJSON(String16Builder* output) const override;
    std::unique_ptr<Value> clone() const override;

    void pushValue(std::unique_ptr<Value>);

    Value* at(size_t index);
    size_t size() const { return m_data.size(); }

private:
    ListValue();
    std::vector<std::unique_ptr<Value>> m_data;
};

} // namespace protocol
} // namespace blink

#endif // Values_h


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Object_h
#define Object_h

//#include "ErrorSupport.h"
//#include "Platform.h"
//#include "Values.h"
#include "platform/PlatformExport.h"

namespace blink {
namespace protocol {

class PLATFORM_EXPORT Object {
public:
    static std::unique_ptr<Object> parse(protocol::Value*, ErrorSupport*);
    ~Object();

    std::unique_ptr<protocol::DictionaryValue> serialize() const;
    std::unique_ptr<Object> clone() const;
private:
    explicit Object(std::unique_ptr<protocol::DictionaryValue>);
    std::unique_ptr<protocol::DictionaryValue> m_object;
};

} // namespace platform
} // namespace blink

#endif // !defined(Object_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ValueConversions_h
#define ValueConversions_h

//#include "ErrorSupport.h"
//#include "Platform.h"
//#include "String16.h"
//#include "Values.h"

namespace blink {
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
struct ValueConversions<InspectorProtocolConvenienceStringType> {
    static InspectorProtocolConvenienceStringType parse(protocol::Value* value, ErrorSupport* errors)
    {
        String16 result;
        bool success = value ? value->asString(&result) : false;
        if (!success)
            errors->addError("string value expected");
        return result;
    }

    static std::unique_ptr<protocol::Value> serialize(const InspectorProtocolConvenienceStringType& value)
    {
        return StringValue::create(value);
    }
};

template<>
struct ValueConversions<String16> {
    static String16 parse(protocol::Value* value, ErrorSupport* errors)
    {
        String16 result;
        bool success = value ? value->asString(&result) : false;
        if (!success)
            errors->addError("string value expected");
        return result;
    }

    static std::unique_ptr<protocol::Value> serialize(const String16& value)
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

} // namespace platform
} // namespace blink

#endif // !defined(ValueConversions_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Maybe_h
#define Maybe_h

//#include "Platform.h"
//#include "String16.h"

namespace blink {
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
class Maybe<InspectorProtocolConvenienceStringType> : public MaybeBase<InspectorProtocolConvenienceStringType> {
public:
    Maybe() { }
    Maybe(const InspectorProtocolConvenienceStringType& value) : MaybeBase(value) { }
    using MaybeBase::operator=;
};

template<>
class Maybe<String16> : public MaybeBase<String16> {
public:
    Maybe() { }
    Maybe(const String16& value) : MaybeBase(value) { }
    using MaybeBase::operator=;
};

} // namespace platform
} // namespace blink

#endif // !defined(Maybe_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Array_h
#define Array_h

//#include "ErrorSupport.h"
//#include "Platform.h"
//#include "String16.h"
//#include "ValueConversions.h"
//#include "Values.h"

#include <vector>

namespace blink {
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
            errors->setName(String16::fromInteger(i));
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
            errors->setName(String16::fromInteger(i));
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

template<> class Array<InspectorProtocolConvenienceStringType> : public ArrayBase<InspectorProtocolConvenienceStringType> {};
template<> class Array<String16> : public ArrayBase<String16> {};
template<> class Array<int> : public ArrayBase<int> {};
template<> class Array<double> : public ArrayBase<double> {};
template<> class Array<bool> : public ArrayBase<bool> {};

} // namespace platform
} // namespace blink

#endif // !defined(Array_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FrontendChannel_h
#define FrontendChannel_h

//#include "String16.h"
#include "platform/PlatformExport.h"

namespace blink {
namespace protocol {

class PLATFORM_EXPORT FrontendChannel {
public:
    virtual ~FrontendChannel() { }
    virtual void sendProtocolResponse(int callId, const String16& message) = 0;
    virtual void sendProtocolNotification(const String16& message) = 0;
    virtual void flushProtocolNotifications() = 0;
};

} // namespace protocol
} // namespace blink

#endif // !defined(FrontendChannel_h)


// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BackendCallback_h
#define BackendCallback_h

//#include "ErrorSupport.h"
//#include "Platform.h"
#include "platform/PlatformExport.h"

namespace blink {
namespace protocol {

class PLATFORM_EXPORT BackendCallback {
public:
    virtual ~BackendCallback() { }
    virtual void sendFailure(const ErrorString&) = 0;
};

} // namespace platform
} // namespace blink

#endif // !defined(BackendCallback_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DispatcherBase_h
#define DispatcherBase_h

//#include "BackendCallback.h"
//#include "Collections.h"
//#include "ErrorSupport.h"
//#include "Platform.h"
//#include "String16.h"
//#include "Values.h"
#include "platform/PlatformExport.h"

namespace blink {
namespace protocol {

class FrontendChannel;
class WeakPtr;

class PLATFORM_EXPORT DispatcherBase {
    PROTOCOL_DISALLOW_COPY(DispatcherBase);
public:
    static const char kInvalidRequest[];
    class PLATFORM_EXPORT WeakPtr {
    public:
        explicit WeakPtr(DispatcherBase*);
        ~WeakPtr();
        DispatcherBase* get() { return m_dispatcher; }
        void dispose() { m_dispatcher = nullptr; }

    private:
        DispatcherBase* m_dispatcher;
    };

    class PLATFORM_EXPORT Callback : public protocol::BackendCallback {
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

    static bool getCommandName(const String16& message, String16* result);

    virtual void dispatch(int callId, const String16& method, std::unique_ptr<protocol::DictionaryValue> messageObject) = 0;

    void sendResponse(int callId, const ErrorString&, ErrorSupport*, std::unique_ptr<protocol::DictionaryValue> result);
    void sendResponse(int callId, const ErrorString&, std::unique_ptr<protocol::DictionaryValue> result);
    void sendResponse(int callId, const ErrorString&);

    void reportProtocolError(int callId, CommonErrorCode, const String16& errorMessage, ErrorSupport* errors);
    void clearFrontend();

    std::unique_ptr<WeakPtr> weakPtr();

private:
    FrontendChannel* m_frontendChannel;
    protocol::HashSet<WeakPtr*> m_weakPtrs;
};

class PLATFORM_EXPORT UberDispatcher {
    PROTOCOL_DISALLOW_COPY(UberDispatcher);
public:
    explicit UberDispatcher(FrontendChannel*);
    void registerBackend(const String16& name, std::unique_ptr<protocol::DispatcherBase>);
    void dispatch(const String16& message);
    FrontendChannel* channel() { return m_frontendChannel; }
    virtual ~UberDispatcher();

private:
    FrontendChannel* m_frontendChannel;
    protocol::HashMap<String16, std::unique_ptr<protocol::DispatcherBase>> m_dispatchers;
};

} // namespace platform
} // namespace blink

#endif // !defined(DispatcherBase_h)


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef Parser_h
#define Parser_h

//#include "Platform.h"
//#include "String16.h"
#include "platform/PlatformExport.h"

namespace blink {
namespace protocol {

class Value;

PLATFORM_EXPORT std::unique_ptr<Value> parseJSON(const String16& json);

} // namespace platform
} // namespace blink

#endif // !defined(Parser_h)
