// This file is generated.

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/protocol/Protocol.h"

#include <algorithm>
#include <cmath>
#include <stdlib.h>
#include <cstring>


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "ErrorSupport.h"

namespace v8_inspector {
namespace protocol {

ErrorSupport::ErrorSupport() : m_errorString(nullptr) { }
ErrorSupport::ErrorSupport(String* errorString) : m_errorString(errorString) { }
ErrorSupport::~ErrorSupport()
{
    if (m_errorString && hasErrors()) {
        StringBuilder builder;
        builder.append("Internal error(s): ");
        builder.append(errors());
        *m_errorString = builder.toString();
    }
}

void ErrorSupport::setName(const String& name)
{
    DCHECK(m_path.size());
    m_path[m_path.size() - 1] = name;
}

void ErrorSupport::push()
{
    m_path.push_back(String());
}

void ErrorSupport::pop()
{
    m_path.pop_back();
}

void ErrorSupport::addError(const String& error)
{
    StringBuilder builder;
    for (size_t i = 0; i < m_path.size(); ++i) {
        if (i)
            builder.append('.');
        builder.append(m_path[i]);
    }
    builder.append(": ");
    builder.append(error);
    m_errors.push_back(builder.toString());
}

bool ErrorSupport::hasErrors()
{
    return m_errors.size();
}

String ErrorSupport::errors()
{
    StringBuilder builder;
    for (size_t i = 0; i < m_errors.size(); ++i) {
        if (i)
            builder.append("; ");
        builder.append(m_errors[i]);
    }
    return builder.toString();
}

} // namespace v8_inspector
} // namespace protocol


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "Values.h"

namespace v8_inspector {
namespace protocol {

namespace {

const char* const nullValueString = "null";
const char* const trueValueString = "true";
const char* const falseValueString = "false";

inline bool escapeChar(uint16_t c, StringBuilder* dst)
{
    switch (c) {
    case '\b': dst->append("\\b"); break;
    case '\f': dst->append("\\f"); break;
    case '\n': dst->append("\\n"); break;
    case '\r': dst->append("\\r"); break;
    case '\t': dst->append("\\t"); break;
    case '\\': dst->append("\\\\"); break;
    case '"': dst->append("\\\""); break;
    default:
        return false;
    }
    return true;
}

const char hexDigits[17] = "0123456789ABCDEF";

void appendUnsignedAsHex(uint16_t number, StringBuilder* dst)
{
    dst->append("\\u");
    for (size_t i = 0; i < 4; ++i) {
        uint16_t c = hexDigits[(number & 0xF000) >> 12];
        dst->append(c);
        number <<= 4;
    }
}

void escapeStringForJSON(const String& str, StringBuilder* dst)
{
    for (unsigned i = 0; i < str.length(); ++i) {
        uint16_t c = str[i];
        if (!escapeChar(c, dst)) {
            if (c < 32 || c > 126 || c == '<' || c == '>') {
                // 1. Escaping <, > to prevent script execution.
                // 2. Technically, we could also pass through c > 126 as UTF8, but this
                //    is also optional. It would also be a pain to implement here.
                appendUnsignedAsHex(c, dst);
            } else {
                dst->append(c);
            }
        }
    }
}

void doubleQuoteStringForJSON(const String& str, StringBuilder* dst)
{
    dst->append('"');
    escapeStringForJSON(str, dst);
    dst->append('"');
}

} // anonymous namespace

bool Value::asBoolean(bool*) const
{
    return false;
}

bool Value::asDouble(double*) const
{
    return false;
}

bool Value::asInteger(int*) const
{
    return false;
}

bool Value::asString(String*) const
{
    return false;
}

bool Value::asSerialized(String*) const
{
    return false;
}

String Value::toJSONString() const
{
    StringBuilder result;
    StringUtil::builderReserve(result, 512);
    writeJSON(&result);
    return result.toString();
}

void Value::writeJSON(StringBuilder* output) const
{
    DCHECK(m_type == TypeNull);
    output->append(nullValueString, 4);
}

std::unique_ptr<Value> Value::clone() const
{
    return Value::null();
}

bool FundamentalValue::asBoolean(bool* output) const
{
    if (type() != TypeBoolean)
        return false;
    *output = m_boolValue;
    return true;
}

bool FundamentalValue::asDouble(double* output) const
{
    if (type() == TypeDouble) {
        *output = m_doubleValue;
        return true;
    }
    if (type() == TypeInteger) {
        *output = m_integerValue;
        return true;
    }
    return false;
}

bool FundamentalValue::asInteger(int* output) const
{
    if (type() != TypeInteger)
        return false;
    *output = m_integerValue;
    return true;
}

void FundamentalValue::writeJSON(StringBuilder* output) const
{
    DCHECK(type() == TypeBoolean || type() == TypeInteger || type() == TypeDouble);
    if (type() == TypeBoolean) {
        if (m_boolValue)
            output->append(trueValueString, 4);
        else
            output->append(falseValueString, 5);
    } else if (type() == TypeDouble) {
        if (!std::isfinite(m_doubleValue)) {
            output->append(nullValueString, 4);
            return;
        }
        output->append(StringUtil::fromDouble(m_doubleValue));
    } else if (type() == TypeInteger) {
        output->append(StringUtil::fromInteger(m_integerValue));
    }
}

std::unique_ptr<Value> FundamentalValue::clone() const
{
    switch (type()) {
    case TypeDouble: return FundamentalValue::create(m_doubleValue);
    case TypeInteger: return FundamentalValue::create(m_integerValue);
    case TypeBoolean: return FundamentalValue::create(m_boolValue);
    default:
        DCHECK(false);
    }
    return nullptr;
}

bool StringValue::asString(String* output) const
{
    *output = m_stringValue;
    return true;
}

void StringValue::writeJSON(StringBuilder* output) const
{
    DCHECK(type() == TypeString);
    doubleQuoteStringForJSON(m_stringValue, output);
}

std::unique_ptr<Value> StringValue::clone() const
{
    return StringValue::create(m_stringValue);
}

bool SerializedValue::asSerialized(String* output) const
{
    *output = m_serializedValue;
    return true;
}

void SerializedValue::writeJSON(StringBuilder* output) const
{
    DCHECK(type() == TypeSerialized);
    output->append(m_serializedValue);
}

std::unique_ptr<Value> SerializedValue::clone() const
{
    return SerializedValue::create(m_serializedValue);
}

DictionaryValue::~DictionaryValue()
{
}

void DictionaryValue::setBoolean(const String& name, bool value)
{
    setValue(name, FundamentalValue::create(value));
}

void DictionaryValue::setInteger(const String& name, int value)
{
    setValue(name, FundamentalValue::create(value));
}

void DictionaryValue::setDouble(const String& name, double value)
{
    setValue(name, FundamentalValue::create(value));
}

void DictionaryValue::setString(const String& name, const String& value)
{
    setValue(name, StringValue::create(value));
}

void DictionaryValue::setValue(const String& name, std::unique_ptr<Value> value)
{
    set(name, value);
}

void DictionaryValue::setObject(const String& name, std::unique_ptr<DictionaryValue> value)
{
    set(name, value);
}

void DictionaryValue::setArray(const String& name, std::unique_ptr<ListValue> value)
{
    set(name, value);
}

bool DictionaryValue::getBoolean(const String& name, bool* output) const
{
    protocol::Value* value = get(name);
    if (!value)
        return false;
    return value->asBoolean(output);
}

bool DictionaryValue::getInteger(const String& name, int* output) const
{
    Value* value = get(name);
    if (!value)
        return false;
    return value->asInteger(output);
}

bool DictionaryValue::getDouble(const String& name, double* output) const
{
    Value* value = get(name);
    if (!value)
        return false;
    return value->asDouble(output);
}

bool DictionaryValue::getString(const String& name, String* output) const
{
    protocol::Value* value = get(name);
    if (!value)
        return false;
    return value->asString(output);
}

DictionaryValue* DictionaryValue::getObject(const String& name) const
{
    return DictionaryValue::cast(get(name));
}

protocol::ListValue* DictionaryValue::getArray(const String& name) const
{
    return ListValue::cast(get(name));
}

protocol::Value* DictionaryValue::get(const String& name) const
{
    Dictionary::const_iterator it = m_data.find(name);
    if (it == m_data.end())
        return nullptr;
    return it->second.get();
}

DictionaryValue::Entry DictionaryValue::at(size_t index) const
{
    const String key = m_order[index];
    return std::make_pair(key, m_data.find(key)->second.get());
}

bool DictionaryValue::booleanProperty(const String& name, bool defaultValue) const
{
    bool result = defaultValue;
    getBoolean(name, &result);
    return result;
}

int DictionaryValue::integerProperty(const String& name, int defaultValue) const
{
    int result = defaultValue;
    getInteger(name, &result);
    return result;
}

double DictionaryValue::doubleProperty(const String& name, double defaultValue) const
{
    double result = defaultValue;
    getDouble(name, &result);
    return result;
}

void DictionaryValue::remove(const String& name)
{
    m_data.erase(name);
    m_order.erase(std::remove(m_order.begin(), m_order.end(), name), m_order.end());
}

void DictionaryValue::writeJSON(StringBuilder* output) const
{
    output->append('{');
    for (size_t i = 0; i < m_order.size(); ++i) {
        Dictionary::const_iterator it = m_data.find(m_order[i]);
        CHECK(it != m_data.end());
        if (i)
            output->append(',');
        doubleQuoteStringForJSON(it->first, output);
        output->append(':');
        it->second->writeJSON(output);
    }
    output->append('}');
}

std::unique_ptr<Value> DictionaryValue::clone() const
{
    std::unique_ptr<DictionaryValue> result = DictionaryValue::create();
    for (size_t i = 0; i < m_order.size(); ++i) {
        String key = m_order[i];
        Dictionary::const_iterator value = m_data.find(key);
        DCHECK(value != m_data.cend() && value->second);
        result->setValue(key, value->second->clone());
    }
    return std::move(result);
}

DictionaryValue::DictionaryValue()
    : Value(TypeObject)
{
}

ListValue::~ListValue()
{
}

void ListValue::writeJSON(StringBuilder* output) const
{
    output->append('[');
    bool first = true;
    for (const std::unique_ptr<protocol::Value>& value : m_data) {
        if (!first)
            output->append(',');
        value->writeJSON(output);
        first = false;
    }
    output->append(']');
}

std::unique_ptr<Value> ListValue::clone() const
{
    std::unique_ptr<ListValue> result = ListValue::create();
    for (const std::unique_ptr<protocol::Value>& value : m_data)
        result->pushValue(value->clone());
    return std::move(result);
}

ListValue::ListValue()
    : Value(TypeArray)
{
}

void ListValue::pushValue(std::unique_ptr<protocol::Value> value)
{
    DCHECK(value);
    m_data.push_back(std::move(value));
}

protocol::Value* ListValue::at(size_t index)
{
    DCHECK_LT(index, m_data.size());
    return m_data[index].get();
}

} // namespace v8_inspector
} // namespace protocol


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "Object.h"

namespace v8_inspector {
namespace protocol {

std::unique_ptr<Object> Object::parse(protocol::Value* value, ErrorSupport* errors)
{
    protocol::DictionaryValue* object = DictionaryValue::cast(value);
    if (!object) {
        errors->addError("object expected");
        return nullptr;
    }
    return wrapUnique(new Object(wrapUnique(static_cast<DictionaryValue*>(object->clone().release()))));
}

std::unique_ptr<protocol::DictionaryValue> Object::serialize() const
{
    return DictionaryValue::cast(m_object->clone());
}

std::unique_ptr<Object> Object::clone() const
{
    return wrapUnique(new Object(DictionaryValue::cast(m_object->clone())));
}

Object::Object(std::unique_ptr<protocol::DictionaryValue> object) : m_object(std::move(object)) { }

Object::~Object() { }

} // namespace v8_inspector
} // namespace protocol


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//#include "DispatcherBase.h"
//#include "Parser.h"

namespace v8_inspector {
namespace protocol {

// static
const char DispatcherBase::kInvalidRequest[] = "Invalid request";

DispatcherBase::WeakPtr::WeakPtr(DispatcherBase* dispatcher) : m_dispatcher(dispatcher) { }

DispatcherBase::WeakPtr::~WeakPtr()
{
    if (m_dispatcher)
        m_dispatcher->m_weakPtrs.erase(this);
}

DispatcherBase::Callback::Callback(std::unique_ptr<DispatcherBase::WeakPtr> backendImpl, int callId)
    : m_backendImpl(std::move(backendImpl))
    , m_callId(callId) { }

DispatcherBase::Callback::~Callback() = default;

void DispatcherBase::Callback::dispose()
{
    m_backendImpl = nullptr;
}

void DispatcherBase::Callback::sendIfActive(std::unique_ptr<protocol::DictionaryValue> partialMessage, const ErrorString& invocationError)
{
    if (!m_backendImpl || !m_backendImpl->get())
        return;
    m_backendImpl->get()->sendResponse(m_callId, invocationError, nullptr, std::move(partialMessage));
    m_backendImpl = nullptr;
}

DispatcherBase::DispatcherBase(FrontendChannel* frontendChannel)
    : m_frontendChannel(frontendChannel) { }

DispatcherBase::~DispatcherBase()
{
    clearFrontend();
}

// static
bool DispatcherBase::getCommandName(const String& message, String* result)
{
    std::unique_ptr<protocol::Value> value = parseJSON(message);
    if (!value)
        return false;

    protocol::DictionaryValue* object = DictionaryValue::cast(value.get());
    if (!object)
        return false;

    if (!object->getString("method", result))
        return false;

    return true;
}

void DispatcherBase::sendResponse(int callId, const ErrorString& invocationError, ErrorSupport* errors, std::unique_ptr<protocol::DictionaryValue> result)
{
    if (invocationError.length() || (errors && errors->hasErrors())) {
        reportProtocolError(callId, ServerError, invocationError, errors);
        return;
    }

    std::unique_ptr<protocol::DictionaryValue> responseMessage = DictionaryValue::create();
    responseMessage->setInteger("id", callId);
    responseMessage->setObject("result", std::move(result));
    if (m_frontendChannel)
        m_frontendChannel->sendProtocolResponse(callId, responseMessage->toJSONString());
}

void DispatcherBase::sendResponse(int callId, const ErrorString& invocationError, std::unique_ptr<protocol::DictionaryValue> result)
{
    sendResponse(callId, invocationError, nullptr, std::move(result));
}

void DispatcherBase::sendResponse(int callId, const ErrorString& invocationError)
{
    sendResponse(callId, invocationError, nullptr, DictionaryValue::create());
}

static void reportProtocolErrorTo(FrontendChannel* frontendChannel, int callId, DispatcherBase::CommonErrorCode code, const String& errorMessage, ErrorSupport* errors)
{
    if (!frontendChannel)
        return;
    std::unique_ptr<protocol::DictionaryValue> error = DictionaryValue::create();
    error->setInteger("code", code);
    error->setString("message", errorMessage);
    DCHECK(error);
    if (errors && errors->hasErrors())
        error->setString("data", errors->errors());
    std::unique_ptr<protocol::DictionaryValue> message = DictionaryValue::create();
    message->setObject("error", std::move(error));
    message->setInteger("id", callId);
    frontendChannel->sendProtocolResponse(callId, message->toJSONString());
}

void DispatcherBase::reportProtocolError(int callId, CommonErrorCode code, const String& errorMessage, ErrorSupport* errors)
{
    reportProtocolErrorTo(m_frontendChannel, callId, code, errorMessage, errors);
}

void DispatcherBase::clearFrontend()
{
    m_frontendChannel = nullptr;
    for (auto& weak : m_weakPtrs)
        weak->dispose();
    m_weakPtrs.clear();
}

std::unique_ptr<DispatcherBase::WeakPtr> DispatcherBase::weakPtr()
{
    std::unique_ptr<DispatcherBase::WeakPtr> weak(new DispatcherBase::WeakPtr(this));
    m_weakPtrs.insert(weak.get());
    return weak;
}

UberDispatcher::UberDispatcher(FrontendChannel* frontendChannel)
    : m_frontendChannel(frontendChannel) { }

void UberDispatcher::registerBackend(const String& name, std::unique_ptr<protocol::DispatcherBase> dispatcher)
{
    m_dispatchers[name] = std::move(dispatcher);
}

void UberDispatcher::dispatch(std::unique_ptr<Value> parsedMessage)
{
    if (!parsedMessage)
        return;
    std::unique_ptr<protocol::DictionaryValue> messageObject = DictionaryValue::cast(std::move(parsedMessage));
    if (!messageObject)
        return;

    int callId = 0;
    protocol::Value* callIdValue = messageObject->get("id");
    bool success = callIdValue && callIdValue->asInteger(&callId);
    if (!success)
        return;

    protocol::Value* methodValue = messageObject->get("method");
    String method;
    success = methodValue && methodValue->asString(&method);
    if (!success)
        return;

    size_t dotIndex = method.find(".");
    if (dotIndex == StringUtil::kNotFound) {
        reportProtocolErrorTo(m_frontendChannel, callId, DispatcherBase::MethodNotFound, "'" + method + "' wasn't found", nullptr);
        return;
    }
    String domain = StringUtil::substring(method, 0, dotIndex);
    auto it = m_dispatchers.find(domain);
    if (it == m_dispatchers.end()) {
        reportProtocolErrorTo(m_frontendChannel, callId, DispatcherBase::MethodNotFound, "'" + method + "' wasn't found", nullptr);
        return;
    }
    it->second->dispatch(callId, method, std::move(messageObject));
}

UberDispatcher::~UberDispatcher() = default;

} // namespace v8_inspector
} // namespace protocol


// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

namespace v8_inspector {
namespace protocol {

namespace {

const int stackLimit = 1000;

enum Token {
    ObjectBegin,
    ObjectEnd,
    ArrayBegin,
    ArrayEnd,
    StringLiteral,
    Number,
    BoolTrue,
    BoolFalse,
    NullToken,
    ListSeparator,
    ObjectPairSeparator,
    InvalidToken,
};

const char* const nullString = "null";
const char* const trueString = "true";
const char* const falseString = "false";

bool isASCII(uint16_t c)
{
    return !(c & ~0x7F);
}

bool isSpaceOrNewLine(uint16_t c)
{
    return isASCII(c) && c <= ' ' && (c == ' ' || (c <= 0xD && c >= 0x9));
}

double charactersToDouble(const uint16_t* characters, size_t length, bool* ok)
{
    std::vector<char> buffer;
    buffer.reserve(length + 1);
    for (size_t i = 0; i < length; ++i) {
        if (!isASCII(characters[i])) {
            *ok = false;
            return 0;
        }
        buffer.push_back(static_cast<char>(characters[i]));
    }
    buffer.push_back('\0');
    char* endptr;
    double result = strtod(buffer.data(), &endptr);
    *ok = !(*endptr);
    return result;
}

double charactersToDouble(const uint8_t* characters, size_t length, bool* ok)
{
    std::string buffer(reinterpret_cast<const char*>(characters), length);
    char* endptr;
    double result = strtod(buffer.data(), &endptr);
    *ok = !(*endptr);
    return result;
}

template<typename Char>
bool parseConstToken(const Char* start, const Char* end, const Char** tokenEnd, const char* token)
{
    while (start < end && *token != '\0' && *start++ == *token++) { }
    if (*token != '\0')
        return false;
    *tokenEnd = start;
    return true;
}

template<typename Char>
bool readInt(const Char* start, const Char* end, const Char** tokenEnd, bool canHaveLeadingZeros)
{
    if (start == end)
        return false;
    bool haveLeadingZero = '0' == *start;
    int length = 0;
    while (start < end && '0' <= *start && *start <= '9') {
        ++start;
        ++length;
    }
    if (!length)
        return false;
    if (!canHaveLeadingZeros && length > 1 && haveLeadingZero)
        return false;
    *tokenEnd = start;
    return true;
}

template<typename Char>
bool parseNumberToken(const Char* start, const Char* end, const Char** tokenEnd)
{
    // We just grab the number here. We validate the size in DecodeNumber.
    // According to RFC4627, a valid number is: [minus] int [frac] [exp]
    if (start == end)
        return false;
    Char c = *start;
    if ('-' == c)
        ++start;

    if (!readInt(start, end, &start, false))
        return false;
    if (start == end) {
        *tokenEnd = start;
        return true;
    }

    // Optional fraction part
    c = *start;
    if ('.' == c) {
        ++start;
        if (!readInt(start, end, &start, true))
            return false;
        if (start == end) {
            *tokenEnd = start;
            return true;
        }
        c = *start;
    }

    // Optional exponent part
    if ('e' == c || 'E' == c) {
        ++start;
        if (start == end)
            return false;
        c = *start;
        if ('-' == c || '+' == c) {
            ++start;
            if (start == end)
                return false;
        }
        if (!readInt(start, end, &start, true))
            return false;
    }

    *tokenEnd = start;
    return true;
}

template<typename Char>
bool readHexDigits(const Char* start, const Char* end, const Char** tokenEnd, int digits)
{
    if (end - start < digits)
        return false;
    for (int i = 0; i < digits; ++i) {
        Char c = *start++;
        if (!(('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')))
            return false;
    }
    *tokenEnd = start;
    return true;
}

template<typename Char>
bool parseStringToken(const Char* start, const Char* end, const Char** tokenEnd)
{
    while (start < end) {
        Char c = *start++;
        if ('\\' == c) {
	    if (start == end)
	        return false;
            c = *start++;
            // Make sure the escaped char is valid.
            switch (c) {
            case 'x':
                if (!readHexDigits(start, end, &start, 2))
                    return false;
                break;
            case 'u':
                if (!readHexDigits(start, end, &start, 4))
                    return false;
                break;
            case '\\':
            case '/':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 't':
            case 'v':
            case '"':
                break;
            default:
                return false;
            }
        } else if ('"' == c) {
            *tokenEnd = start;
            return true;
        }
    }
    return false;
}

template<typename Char>
bool skipComment(const Char* start, const Char* end, const Char** commentEnd)
{
    if (start == end)
        return false;

    if (*start != '/' || start + 1 >= end)
        return false;
    ++start;

    if (*start == '/') {
        // Single line comment, read to newline.
        for (++start; start < end; ++start) {
            if (*start == '\n' || *start == '\r') {
                *commentEnd = start + 1;
                return true;
            }
        }
        *commentEnd = end;
        // Comment reaches end-of-input, which is fine.
        return true;
    }

    if (*start == '*') {
        Char previous = '\0';
        // Block comment, read until end marker.
        for (++start; start < end; previous = *start++) {
            if (previous == '*' && *start == '/') {
                *commentEnd = start + 1;
                return true;
            }
        }
        // Block comment must close before end-of-input.
        return false;
    }

    return false;
}

template<typename Char>
void skipWhitespaceAndComments(const Char* start, const Char* end, const Char** whitespaceEnd)
{
    while (start < end) {
        if (isSpaceOrNewLine(*start)) {
            ++start;
        } else if (*start == '/') {
            const Char* commentEnd;
            if (!skipComment(start, end, &commentEnd))
                break;
            start = commentEnd;
        } else {
            break;
        }
    }
    *whitespaceEnd = start;
}

template<typename Char>
Token parseToken(const Char* start, const Char* end, const Char** tokenStart, const Char** tokenEnd)
{
    skipWhitespaceAndComments(start, end, tokenStart);
    start = *tokenStart;

    if (start == end)
        return InvalidToken;

    switch (*start) {
    case 'n':
        if (parseConstToken(start, end, tokenEnd, nullString))
            return NullToken;
        break;
    case 't':
        if (parseConstToken(start, end, tokenEnd, trueString))
            return BoolTrue;
        break;
    case 'f':
        if (parseConstToken(start, end, tokenEnd, falseString))
            return BoolFalse;
        break;
    case '[':
        *tokenEnd = start + 1;
        return ArrayBegin;
    case ']':
        *tokenEnd = start + 1;
        return ArrayEnd;
    case ',':
        *tokenEnd = start + 1;
        return ListSeparator;
    case '{':
        *tokenEnd = start + 1;
        return ObjectBegin;
    case '}':
        *tokenEnd = start + 1;
        return ObjectEnd;
    case ':':
        *tokenEnd = start + 1;
        return ObjectPairSeparator;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '-':
        if (parseNumberToken(start, end, tokenEnd))
            return Number;
        break;
    case '"':
        if (parseStringToken(start + 1, end, tokenEnd))
            return StringLiteral;
        break;
    }
    return InvalidToken;
}

template<typename Char>
int hexToInt(Char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    DCHECK(false);
    return 0;
}

template<typename Char>
bool decodeString(const Char* start, const Char* end, StringBuilder* output)
{
    while (start < end) {
        uint16_t c = *start++;
        if ('\\' != c) {
            output->append(c);
            continue;
        }
	if (start == end)
	    return false;
        c = *start++;

        if (c == 'x') {
            // \x is not supported.
            return false;
        }

        switch (c) {
        case '"':
        case '/':
        case '\\':
            break;
        case 'b':
            c = '\b';
            break;
        case 'f':
            c = '\f';
            break;
        case 'n':
            c = '\n';
            break;
        case 'r':
            c = '\r';
            break;
        case 't':
            c = '\t';
            break;
        case 'v':
            c = '\v';
            break;
        case 'u':
            c = (hexToInt(*start) << 12) +
                (hexToInt(*(start + 1)) << 8) +
                (hexToInt(*(start + 2)) << 4) +
                hexToInt(*(start + 3));
            start += 4;
            break;
        default:
            return false;
        }
        output->append(c);
    }
    return true;
}

template<typename Char>
bool decodeString(const Char* start, const Char* end, String* output)
{
    if (start == end) {
        *output = "";
        return true;
    }
    if (start > end)
        return false;
    StringBuilder buffer;
    StringUtil::builderReserve(buffer, end - start);
    if (!decodeString(start, end, &buffer))
        return false;
    *output = buffer.toString();
    return true;
}

template<typename Char>
std::unique_ptr<Value> buildValue(const Char* start, const Char* end, const Char** valueTokenEnd, int depth)
{
    if (depth > stackLimit)
        return nullptr;

    std::unique_ptr<Value> result;
    const Char* tokenStart;
    const Char* tokenEnd;
    Token token = parseToken(start, end, &tokenStart, &tokenEnd);
    switch (token) {
    case InvalidToken:
        return nullptr;
    case NullToken:
        result = Value::null();
        break;
    case BoolTrue:
        result = FundamentalValue::create(true);
        break;
    case BoolFalse:
        result = FundamentalValue::create(false);
        break;
    case Number: {
        bool ok;
        double value = charactersToDouble(tokenStart, tokenEnd - tokenStart, &ok);
        if (!ok)
            return nullptr;
        int number = static_cast<int>(value);
        if (number == value)
            result = FundamentalValue::create(number);
        else
            result = FundamentalValue::create(value);
        break;
    }
    case StringLiteral: {
        String value;
        bool ok = decodeString(tokenStart + 1, tokenEnd - 1, &value);
        if (!ok)
            return nullptr;
        result = StringValue::create(value);
        break;
    }
    case ArrayBegin: {
        std::unique_ptr<ListValue> array = ListValue::create();
        start = tokenEnd;
        token = parseToken(start, end, &tokenStart, &tokenEnd);
        while (token != ArrayEnd) {
            std::unique_ptr<Value> arrayNode = buildValue(start, end, &tokenEnd, depth + 1);
            if (!arrayNode)
                return nullptr;
            array->pushValue(std::move(arrayNode));

            // After a list value, we expect a comma or the end of the list.
            start = tokenEnd;
            token = parseToken(start, end, &tokenStart, &tokenEnd);
            if (token == ListSeparator) {
                start = tokenEnd;
                token = parseToken(start, end, &tokenStart, &tokenEnd);
                if (token == ArrayEnd)
                    return nullptr;
            } else if (token != ArrayEnd) {
                // Unexpected value after list value. Bail out.
                return nullptr;
            }
        }
        if (token != ArrayEnd)
            return nullptr;
        result = std::move(array);
        break;
    }
    case ObjectBegin: {
        std::unique_ptr<DictionaryValue> object = DictionaryValue::create();
        start = tokenEnd;
        token = parseToken(start, end, &tokenStart, &tokenEnd);
        while (token != ObjectEnd) {
            if (token != StringLiteral)
                return nullptr;
            String key;
            if (!decodeString(tokenStart + 1, tokenEnd - 1, &key))
                return nullptr;
            start = tokenEnd;

            token = parseToken(start, end, &tokenStart, &tokenEnd);
            if (token != ObjectPairSeparator)
                return nullptr;
            start = tokenEnd;

            std::unique_ptr<Value> value = buildValue(start, end, &tokenEnd, depth + 1);
            if (!value)
                return nullptr;
            object->setValue(key, std::move(value));
            start = tokenEnd;

            // After a key/value pair, we expect a comma or the end of the
            // object.
            token = parseToken(start, end, &tokenStart, &tokenEnd);
            if (token == ListSeparator) {
                start = tokenEnd;
                token = parseToken(start, end, &tokenStart, &tokenEnd);
                if (token == ObjectEnd)
                    return nullptr;
            } else if (token != ObjectEnd) {
                // Unexpected value after last object value. Bail out.
                return nullptr;
            }
        }
        if (token != ObjectEnd)
            return nullptr;
        result = std::move(object);
        break;
    }

    default:
        // We got a token that's not a value.
        return nullptr;
    }

    skipWhitespaceAndComments(tokenEnd, end, valueTokenEnd);
    return result;
}

template<typename Char>
std::unique_ptr<Value> parseJSONInternal(const Char* start, unsigned length)
{
    const Char* end = start + length;
    const Char *tokenEnd;
    std::unique_ptr<Value> value = buildValue(start, end, &tokenEnd, 0);
    if (!value || tokenEnd != end)
        return nullptr;
    return value;
}

} // anonymous namespace

std::unique_ptr<Value> parseJSON(const uint16_t* characters, unsigned length)
{
    return parseJSONInternal<uint16_t>(characters, length);
}

std::unique_ptr<Value> parseJSON(const uint8_t* characters, unsigned length)
{
    return parseJSONInternal<uint8_t>(characters, length);
}

} // namespace v8_inspector
} // namespace protocol
