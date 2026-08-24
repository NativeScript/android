#include "Base64.h"

#include <vector>

#include "Util.h"

using namespace v8;

namespace tns {

namespace {

constexpr char kAlphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 6-bit value per ASCII byte; 0xFF marks everything outside the alphabet.
constexpr uint8_t kInvalid = 0xFF;

uint8_t SixBits(uint8_t c) {
    if (c >= 'A' && c <= 'Z') {
        return static_cast<uint8_t>(c - 'A');
    }
    if (c >= 'a' && c <= 'z') {
        return static_cast<uint8_t>(c - 'a' + 26);
    }
    if (c >= '0' && c <= '9') {
        return static_cast<uint8_t>(c - '0' + 52);
    }
    if (c == '+') {
        return 62;
    }
    if (c == '/') {
        return 63;
    }
    return kInvalid;
}

bool IsAsciiWhitespace(uint8_t c) {
    return c == '\t' || c == '\n' || c == '\f' || c == '\r' || c == ' ';
}

// The string's code units as bytes. Fails when any unit is above U+00FF,
// which neither op can represent.
bool GetLatin1Bytes(Isolate* isolate, Local<Value> value,
                    std::vector<uint8_t>* out) {
    if (!value->IsString()) {
        return false;
    }
    Local<v8::String> str = value.As<v8::String>();
    if (!str->ContainsOnlyOneByte()) {
        return false;
    }
    const int length = str->Length();
    out->resize(static_cast<size_t>(length));
    if (length > 0) {
        str->WriteOneByteV2(isolate, 0, static_cast<uint32_t>(length), out->data());
    }
    return true;
}

// btoa: base64-encode the input's code units.
void BtoaCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    std::vector<uint8_t> input;
    if (!GetLatin1Bytes(isolate, info[0], &input)) {
        info.GetReturnValue().SetNull();
        return;
    }

    std::vector<uint8_t> out;
    out.reserve((input.size() + 2) / 3 * 4);
    size_t i = 0;
    for (; i + 3 <= input.size(); i += 3) {
        const uint32_t group = (static_cast<uint32_t>(input[i]) << 16) |
                               (static_cast<uint32_t>(input[i + 1]) << 8) |
                               input[i + 2];
        out.push_back(kAlphabet[(group >> 18) & 0x3F]);
        out.push_back(kAlphabet[(group >> 12) & 0x3F]);
        out.push_back(kAlphabet[(group >> 6) & 0x3F]);
        out.push_back(kAlphabet[group & 0x3F]);
    }
    const size_t remaining = input.size() - i;
    if (remaining == 1) {
        const uint32_t group = static_cast<uint32_t>(input[i]) << 16;
        out.push_back(kAlphabet[(group >> 18) & 0x3F]);
        out.push_back(kAlphabet[(group >> 12) & 0x3F]);
        out.push_back('=');
        out.push_back('=');
    } else if (remaining == 2) {
        const uint32_t group = (static_cast<uint32_t>(input[i]) << 16) |
                               (static_cast<uint32_t>(input[i + 1]) << 8);
        out.push_back(kAlphabet[(group >> 18) & 0x3F]);
        out.push_back(kAlphabet[(group >> 12) & 0x3F]);
        out.push_back(kAlphabet[(group >> 6) & 0x3F]);
        out.push_back('=');
    }

    if (out.empty()) {
        info.GetReturnValue().Set(v8::String::Empty(isolate));
        return;
    }
    Local<v8::String> result;
    if (v8::String::NewFromOneByte(isolate, out.data(), NewStringType::kNormal,
                                   static_cast<int>(out.size()))
                .ToLocal(&result)) {
        info.GetReturnValue().Set(result);
    }
}

// atob: forgiving-base64 decode
// (https://infra.spec.whatwg.org/#forgiving-base64-decode).
void AtobCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    std::vector<uint8_t> raw;
    if (!GetLatin1Bytes(isolate, info[0], &raw)) {
        info.GetReturnValue().SetNull();
        return;
    }

    std::vector<uint8_t> data;
    data.reserve(raw.size());
    for (uint8_t c : raw) {
        if (!IsAsciiWhitespace(c)) {
            data.push_back(c);
        }
    }

    if (data.size() % 4 == 0) {
        size_t strip = 0;
        while (strip < 2 && !data.empty() && data.back() == '=') {
            data.pop_back();
            strip++;
        }
    }
    if (data.size() % 4 == 1) {
        info.GetReturnValue().SetNull();
        return;
    }

    std::vector<uint8_t> out;
    out.reserve(data.size() / 4 * 3 + 2);
    uint32_t accumulator = 0;
    uint32_t bits = 0;
    for (uint8_t c : data) {
        const uint8_t value = SixBits(c);
        if (value == kInvalid) {
            info.GetReturnValue().SetNull();
            return;
        }
        accumulator = (accumulator << 6) | value;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            out.push_back(static_cast<uint8_t>((accumulator >> bits) & 0xFF));
        }
    }

    if (out.empty()) {
        info.GetReturnValue().Set(v8::String::Empty(isolate));
        return;
    }
    Local<v8::String> result;
    if (v8::String::NewFromOneByte(isolate, out.data(), NewStringType::kNormal,
                                   static_cast<int>(out.size()))
                .ToLocal(&result)) {
        info.GetReturnValue().Set(result);
    }
}

}  // namespace

Local<Object> Base64::CreateBinding(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    Local<Object> binding = Object::New(isolate);
    tns::SetMethodNoSideEffect(context, binding, "btoa", BtoaCallback);
    tns::SetMethodNoSideEffect(context, binding, "atob", AtobCallback);
    return binding;
}

}  // namespace tns
