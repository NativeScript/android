#include "TextEncoding.h"

#include <string>
#include <vector>

#include "v8-fast-api-calls.h"

#include "ArgConverter.h"
#include "Util.h"

using namespace v8;

namespace tns {

namespace {

// Encoding ids shared with text-encoding.js, which maps them back to the
// canonical names through its kEncodingNames array — keep the two in step.
enum Encoding : uint32_t {
    kUtf8 = 0,
    kUtf16le = 1,
    kUtf16be = 2,
    kWindows1252 = 3,
};

// Decode option bits, mirrored by kFlag* in text-encoding.js.
constexpr uint32_t kFlagFatal = 1;
constexpr uint32_t kFlagIgnoreBOM = 2;
constexpr uint32_t kFlagStream = 4;

constexpr uint16_t kReplacementCharacter = 0xFFFD;

struct EncodingLabel {
    const char* label;
    Encoding encoding;
};

// The complete label set of the four encodings the runtime supports
// (https://encoding.spec.whatwg.org/#names-and-labels), sorted by label so
// lookup is a binary search over a table with no runtime setup cost.
constexpr EncodingLabel kLabels[] = {
        {"ansi_x3.4-1968", kWindows1252},
        {"ascii", kWindows1252},
        {"cp1252", kWindows1252},
        {"cp819", kWindows1252},
        {"csisolatin1", kWindows1252},
        {"csunicode", kUtf16le},
        {"ibm819", kWindows1252},
        {"iso-10646-ucs-2", kUtf16le},
        {"iso-8859-1", kWindows1252},
        {"iso-ir-100", kWindows1252},
        {"iso8859-1", kWindows1252},
        {"iso88591", kWindows1252},
        {"iso_8859-1", kWindows1252},
        {"iso_8859-1:1987", kWindows1252},
        {"l1", kWindows1252},
        {"latin1", kWindows1252},
        {"ucs-2", kUtf16le},
        {"unicode", kUtf16le},
        {"unicode-1-1-utf-8", kUtf8},
        {"unicode11utf8", kUtf8},
        {"unicode20utf8", kUtf8},
        {"unicodefeff", kUtf16le},
        {"unicodefffe", kUtf16be},
        {"us-ascii", kWindows1252},
        {"utf-16", kUtf16le},
        {"utf-16be", kUtf16be},
        {"utf-16le", kUtf16le},
        {"utf-8", kUtf8},
        {"utf8", kUtf8},
        {"windows-1252", kWindows1252},
        {"x-cp1252", kWindows1252},
        {"x-unicode20utf8", kUtf8},
};

// windows-1252 index, pointers 0x80-0x9F. Everything outside that block is
// Latin-1 (identity), including the C1 controls this table maps to
// themselves.
constexpr uint16_t kWindows1252Index[32] = {
        0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
        0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,
        0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
        0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178};

bool IsAsciiWhitespace(char c) {
    return c == '\t' || c == '\n' || c == '\f' || c == '\r' || c == ' ';
}

int32_t LookupEncoding(const std::string& rawLabel) {
    size_t begin = 0;
    size_t end = rawLabel.size();
    while (begin < end && IsAsciiWhitespace(rawLabel[begin])) {
        begin++;
    }
    while (end > begin && IsAsciiWhitespace(rawLabel[end - 1])) {
        end--;
    }

    std::string label = rawLabel.substr(begin, end - begin);
    for (char& c : label) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c - 'A' + 'a');
        }
    }

    size_t lo = 0;
    size_t hi = sizeof(kLabels) / sizeof(kLabels[0]);
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        int cmp = label.compare(kLabels[mid].label);
        if (cmp == 0) {
            return static_cast<int32_t>(kLabels[mid].encoding);
        }
        if (cmp < 0) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return -1;
}

// Everything a decoder must remember between streaming calls. It lives in a
// Uint8Array the builtin allocates per TextDecoder, so the native side stays
// stateless and no instance needs a finalizer.
constexpr int kDecoderStateBytes = 10;  // bytes Store writes
static_assert(kDecoderStateBytes <= TextEncoding::kDecoderStateSize,
              "the builtin allocates too little decoder state");

struct DecoderState {
    bool bomSeen = false;
    uint8_t utf8PendingLength = 0;
    uint8_t utf8Pending[3] = {0, 0, 0};
    bool hasLeadByte = false;
    uint8_t leadByte = 0;
    bool hasLeadSurrogate = false;
    uint16_t leadSurrogate = 0;

    void Load(const uint8_t* raw) {
        bomSeen = (raw[0] & 1) != 0;
        utf8PendingLength = raw[1] > 3 ? 0 : raw[1];
        utf8Pending[0] = raw[2];
        utf8Pending[1] = raw[3];
        utf8Pending[2] = raw[4];
        hasLeadByte = raw[5] != 0;
        leadByte = raw[6];
        hasLeadSurrogate = raw[7] != 0;
        leadSurrogate = static_cast<uint16_t>(raw[8] | (raw[9] << 8));
    }

    void Store(uint8_t* raw) const {
        raw[0] = bomSeen ? 1 : 0;
        raw[1] = utf8PendingLength;
        raw[2] = utf8Pending[0];
        raw[3] = utf8Pending[1];
        raw[4] = utf8Pending[2];
        raw[5] = hasLeadByte ? 1 : 0;
        raw[6] = leadByte;
        raw[7] = hasLeadSurrogate ? 1 : 0;
        raw[8] = static_cast<uint8_t>(leadSurrogate & 0xFF);
        raw[9] = static_cast<uint8_t>(leadSurrogate >> 8);
    }

    void Reset() { *this = DecoderState(); }
};

// Collects decoded UTF-16 code units and tracks whether they all fit in one
// byte, so the finished string can take V8's one-byte representation.
class Utf16Sink {
public:
    void Append(uint16_t unit) {
        units_.push_back(unit);
        orAll_ |= unit;
    }

    void AppendCodePoint(uint32_t codePoint) {
        if (codePoint <= 0xFFFF) {
            Append(static_cast<uint16_t>(codePoint));
            return;
        }
        codePoint -= 0x10000;
        Append(static_cast<uint16_t>(0xD800 + (codePoint >> 10)));
        Append(static_cast<uint16_t>(0xDC00 + (codePoint & 0x3FF)));
    }

    MaybeLocal<v8::String> Finish(Isolate* isolate) const {
        if (units_.empty()) {
            return v8::String::Empty(isolate);
        }
        if (orAll_ <= 0xFF) {
            std::vector<uint8_t> oneByte(units_.size());
            for (size_t i = 0; i < units_.size(); i++) {
                oneByte[i] = static_cast<uint8_t>(units_[i]);
            }
            return v8::String::NewFromOneByte(isolate, oneByte.data(),
                                              NewStringType::kNormal,
                                              static_cast<int>(oneByte.size()));
        }
        return v8::String::NewFromTwoByte(isolate, units_.data(),
                                          NewStringType::kNormal,
                                          static_cast<int>(units_.size()));
    }

private:
    std::vector<uint16_t> units_;
    uint32_t orAll_ = 0;
};

// A code point leaving a utf-8 / utf-16 decoder, with the leading-BOM removal
// TextDecoder performs once per stream.
class BomFilter {
public:
    BomFilter(Utf16Sink& sink, DecoderState& state, bool ignoreBOM)
        : sink_(sink), state_(state), ignoreBOM_(ignoreBOM) {}

    void Emit(uint32_t codePoint) {
        if (!state_.bomSeen) {
            state_.bomSeen = true;
            if (!ignoreBOM_ && codePoint == 0xFEFF) {
                return;
            }
        }
        sink_.AppendCodePoint(codePoint);
    }

private:
    Utf16Sink& sink_;
    DecoderState& state_;
    const bool ignoreBOM_;
};

// WHATWG utf-8 decoder. Incomplete trailing sequences are kept as raw bytes
// and replayed at the head of the next call, so the boundary constraints of a
// split sequence are re-derived from its own lead byte rather than carried in
// the saved state. Returns false when fatal mode hits invalid input.
bool DecodeUtf8(const uint8_t* input, size_t inputLength, DecoderState& state,
                bool stream, bool fatal, bool ignoreBOM, Utf16Sink& sink) {
    const size_t pendingLength = state.utf8PendingLength;
    const size_t total = pendingLength + inputLength;
    auto byteAt = [&](size_t index) -> uint8_t {
        return index < pendingLength ? state.utf8Pending[index]
                                     : input[index - pendingLength];
    };
    state.utf8PendingLength = 0;

    BomFilter out(sink, state, ignoreBOM);
    uint32_t codePoint = 0;
    uint32_t bytesNeeded = 0;
    uint32_t bytesSeen = 0;
    uint32_t lowerBoundary = 0x80;
    uint32_t upperBoundary = 0xBF;
    size_t sequenceStart = 0;
    size_t index = 0;

    while (index < total) {
        const uint8_t byte = byteAt(index);
        if (bytesNeeded == 0) {
            sequenceStart = index;
            index++;
            if (byte <= 0x7F) {
                out.Emit(byte);
            } else if (byte >= 0xC2 && byte <= 0xDF) {
                bytesNeeded = 1;
                codePoint = byte & 0x1F;
            } else if (byte >= 0xE0 && byte <= 0xEF) {
                if (byte == 0xE0) {
                    lowerBoundary = 0xA0;
                } else if (byte == 0xED) {
                    upperBoundary = 0x9F;
                }
                bytesNeeded = 2;
                codePoint = byte & 0x0F;
            } else if (byte >= 0xF0 && byte <= 0xF4) {
                if (byte == 0xF0) {
                    lowerBoundary = 0x90;
                } else if (byte == 0xF4) {
                    upperBoundary = 0x8F;
                }
                bytesNeeded = 3;
                codePoint = byte & 0x07;
            } else {
                if (fatal) {
                    state.Reset();
                    return false;
                }
                out.Emit(kReplacementCharacter);
            }
            continue;
        }

        if (byte < lowerBoundary || byte > upperBoundary) {
            // One replacement for the maximal subpart consumed so far; the
            // offending byte is reprocessed as the start of a new sequence
            // (index unchanged).
            codePoint = 0;
            bytesNeeded = 0;
            bytesSeen = 0;
            lowerBoundary = 0x80;
            upperBoundary = 0xBF;
            if (fatal) {
                state.Reset();
                return false;
            }
            out.Emit(kReplacementCharacter);
            continue;
        }

        lowerBoundary = 0x80;
        upperBoundary = 0xBF;
        codePoint = (codePoint << 6) | (byte & 0x3F);
        bytesSeen++;
        index++;
        if (bytesSeen != bytesNeeded) {
            continue;
        }
        const uint32_t finished = codePoint;
        codePoint = 0;
        bytesNeeded = 0;
        bytesSeen = 0;
        out.Emit(finished);
    }

    if (bytesNeeded == 0) {
        return true;
    }
    if (!stream) {
        if (fatal) {
            state.Reset();
            return false;
        }
        out.Emit(kReplacementCharacter);
        return true;
    }
    // At most three bytes, and each is read before the slot it overwrites.
    const size_t carried = total - sequenceStart;
    for (size_t i = 0; i < carried; i++) {
        state.utf8Pending[i] = byteAt(sequenceStart + i);
    }
    state.utf8PendingLength = static_cast<uint8_t>(carried);
    return true;
}

// WHATWG shared utf-16 decoder, both endiannesses.
bool DecodeUtf16(const uint8_t* input, size_t inputLength, bool bigEndian,
                 DecoderState& state, bool stream, bool fatal, bool ignoreBOM,
                 Utf16Sink& sink) {
    BomFilter out(sink, state, ignoreBOM);

    auto process = [&](uint16_t unit) -> bool {
        if (state.hasLeadSurrogate) {
            const uint16_t lead = state.leadSurrogate;
            state.hasLeadSurrogate = false;
            if (unit >= 0xDC00 && unit <= 0xDFFF) {
                out.Emit(0x10000u + (static_cast<uint32_t>(lead - 0xD800) << 10) +
                         (unit - 0xDC00));
                return true;
            }
            if (fatal) {
                return false;
            }
            // The unpaired lead is replaced and `unit` starts over below.
            out.Emit(kReplacementCharacter);
        }
        if (unit >= 0xD800 && unit <= 0xDBFF) {
            state.hasLeadSurrogate = true;
            state.leadSurrogate = unit;
            return true;
        }
        if (unit >= 0xDC00 && unit <= 0xDFFF) {
            if (fatal) {
                return false;
            }
            out.Emit(kReplacementCharacter);
            return true;
        }
        out.Emit(unit);
        return true;
    };

    for (size_t i = 0; i < inputLength; i++) {
        const uint8_t byte = input[i];
        if (!state.hasLeadByte) {
            state.hasLeadByte = true;
            state.leadByte = byte;
            continue;
        }
        const uint16_t unit =
                bigEndian ? static_cast<uint16_t>((state.leadByte << 8) | byte)
                          : static_cast<uint16_t>((byte << 8) | state.leadByte);
        state.hasLeadByte = false;
        if (!process(unit)) {
            state.Reset();
            return false;
        }
    }

    if (stream) {
        return true;
    }
    if (state.hasLeadByte || state.hasLeadSurrogate) {
        state.hasLeadByte = false;
        state.hasLeadSurrogate = false;
        if (fatal) {
            state.Reset();
            return false;
        }
        out.Emit(kReplacementCharacter);
    }
    return true;
}

void DecodeWindows1252(const uint8_t* input, size_t inputLength,
                       Utf16Sink& sink) {
    for (size_t i = 0; i < inputLength; i++) {
        const uint8_t byte = input[i];
        sink.Append(byte >= 0x80 && byte <= 0x9F ? kWindows1252Index[byte - 0x80]
                                                 : byte);
    }
}

bool AllBytesBelow(const uint8_t* input, size_t length, uint8_t limit) {
    for (size_t i = 0; i < length; i++) {
        if (input[i] >= limit) {
            return false;
        }
    }
    return true;
}

bool NoC1Bytes(const uint8_t* input, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (input[i] >= 0x80 && input[i] <= 0x9F) {
            return false;
        }
    }
    return true;
}

// Bytes of an ArrayBuffer, SharedArrayBuffer or any ArrayBufferView; a
// detached buffer reads as empty. Only the builtin calls in, so anything else
// is a programming error rather than a user-visible one.
bool GetByteSource(Local<Value> value, const uint8_t** data, size_t* length) {
    *data = nullptr;
    *length = 0;
    if (value.IsEmpty() || value->IsUndefined()) {
        return true;
    }
    if (value->IsArrayBufferView()) {
        Local<ArrayBufferView> view = value.As<ArrayBufferView>();
        Local<ArrayBuffer> buffer = view->Buffer();
        void* base = buffer->Data();
        if (base == nullptr) {
            return true;
        }
        *data = static_cast<const uint8_t*>(base) + view->ByteOffset();
        *length = view->ByteLength();
        return true;
    }
    if (value->IsArrayBuffer()) {
        Local<ArrayBuffer> buffer = value.As<ArrayBuffer>();
        if (buffer->Data() == nullptr) {
            return true;
        }
        *data = static_cast<const uint8_t*>(buffer->Data());
        *length = buffer->ByteLength();
        return true;
    }
    if (value->IsSharedArrayBuffer()) {
        Local<SharedArrayBuffer> buffer = value.As<SharedArrayBuffer>();
        if (buffer->Data() == nullptr) {
            return true;
        }
        *data = static_cast<const uint8_t*>(buffer->Data());
        *length = buffer->ByteLength();
        return true;
    }
    return false;
}

void LabelToEncodingCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(LookupEncoding(Util::ToString(isolate, info[0])));
}

void DecodeCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();

    const uint8_t* input = nullptr;
    size_t inputLength = 0;
    if (!GetByteSource(info[0], &input, &inputLength)) {
        isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(
                isolate,
                "The \"input\" argument must be an ArrayBuffer, SharedArrayBuffer or "
                "ArrayBufferView")));
        return;
    }

    const uint32_t encoding =
            static_cast<uint32_t>(info[1].As<v8::Number>()->Value());
    const uint32_t flags =
            static_cast<uint32_t>(info[2].As<v8::Number>()->Value());
    const bool fatal = (flags & kFlagFatal) != 0;
    const bool ignoreBOM = (flags & kFlagIgnoreBOM) != 0;
    const bool stream = (flags & kFlagStream) != 0;

    Local<Uint8Array> stateArray = info[3].As<Uint8Array>();
    uint8_t* rawState = static_cast<uint8_t*>(stateArray->Buffer()->Data()) +
                        stateArray->ByteOffset();
    DecoderState state;
    state.Load(rawState);

    // Byte-for-byte one-byte results skip the intermediate code-unit buffer.
    if (input != nullptr && !stream && state.utf8PendingLength == 0 &&
        !state.hasLeadByte && !state.hasLeadSurrogate) {
        const bool asciiUtf8 =
                encoding == kUtf8 && AllBytesBelow(input, inputLength, 0x80);
        const bool latin1Windows1252 =
                encoding == kWindows1252 && NoC1Bytes(input, inputLength);
        if (asciiUtf8 || latin1Windows1252) {
            state.Reset();
            state.Store(rawState);
            Local<v8::String> result;
            if (v8::String::NewFromOneByte(isolate, input, NewStringType::kNormal,
                                           static_cast<int>(inputLength))
                        .ToLocal(&result)) {
                info.GetReturnValue().Set(result);
            }
            return;
        }
    }

    Utf16Sink sink;
    bool ok = true;
    switch (encoding) {
        case kUtf8:
            ok = DecodeUtf8(input, inputLength, state, stream, fatal, ignoreBOM,
                            sink);
            break;
        case kUtf16le:
        case kUtf16be:
            ok = DecodeUtf16(input, inputLength, encoding == kUtf16be, state,
                             stream, fatal, ignoreBOM, sink);
            break;
        default:
            DecodeWindows1252(input, inputLength, sink);
            break;
    }

    // A non-streaming call is the end of a stream: the next one starts from a
    // clean decoder, BOM tracking included.
    if (!stream) {
        state.Reset();
    }
    state.Store(rawState);
    if (!ok) {
        isolate->ThrowException(Exception::TypeError(ArgConverter::ConvertToV8String(
                isolate, "The encoded data was not valid")));
        return;
    }

    Local<v8::String> result;
    if (sink.Finish(isolate).ToLocal(&result)) {
        info.GetReturnValue().Set(result);
    }
}

void EncodeUtf8Callback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<v8::String> source = info[0].As<v8::String>();

    const size_t length = source->Utf8LengthV2(isolate);
    std::unique_ptr<BackingStore> store =
            ArrayBuffer::NewBackingStore(isolate, length);
    if (length > 0) {
        source->WriteUtf8V2(isolate, static_cast<char*>(store->Data()), length,
                            v8::String::WriteFlags::kReplaceInvalidUtf8);
    }
    Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, std::move(store));
    info.GetReturnValue().Set(Uint8Array::New(buffer, 0, length));
}

// Writes as much of `source` as fits into `destination` without splitting an
// encoded code point, and reports {read, written} through `results` — a
// Uint32Array the builtin owns, so the op returns only the destination
// type check and stays expressible as a fast call. A destination that is a
// Uint8Array but detached or empty is a zero-length write, not a failure.
bool EncodeIntoImpl(Isolate* isolate, Local<Value> sourceValue,
                    Local<Value> destinationValue, Local<Value> resultsValue) {
    if (!destinationValue->IsUint8Array()) {
        return false;
    }
    if (!sourceValue->IsString() || !resultsValue->IsUint32Array()) {
        return true;
    }

    Local<Uint32Array> results = resultsValue.As<Uint32Array>();
    uint32_t* resultData = static_cast<uint32_t*>(results->Buffer()->Data());
    if (resultData == nullptr || results->Length() < 2) {
        return true;
    }
    resultData += results->ByteOffset() / sizeof(uint32_t);
    resultData[0] = 0;
    resultData[1] = 0;

    Local<Uint8Array> destination = destinationValue.As<Uint8Array>();
    void* base = destination->Buffer()->Data();
    const size_t capacity = destination->ByteLength();
    if (base == nullptr || capacity == 0) {
        return true;
    }

    size_t read = 0;
    const size_t written = sourceValue.As<v8::String>()->WriteUtf8V2(
            isolate, static_cast<char*>(base) + destination->ByteOffset(),
            capacity, v8::String::WriteFlags::kReplaceInvalidUtf8, &read);
    resultData[0] = static_cast<uint32_t>(read);
    resultData[1] = static_cast<uint32_t>(written);
    return true;
}

void EncodeIntoCallback(const FunctionCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(
            EncodeIntoImpl(info.GetIsolate(), info[0], info[1], info[2]));
}

#if NATIVESCRIPT_ENABLE_FAST_API
// Fast-call overload of encodeInto. It allocates nothing on the V8 heap and
// calls no JS.
bool FastEncodeInto(Local<Value> receiver, Local<Value> source,
                    Local<Value> destination, Local<Value> results,
                    // NOLINTNEXTLINE(runtime/references)
                    FastApiCallbackOptions& options) {
    HandleScope scope(options.isolate);
    return EncodeIntoImpl(options.isolate, source, destination, results);
}

const CFunction kFastEncodeInto = CFunction::Make(FastEncodeInto);
#endif

}  // namespace

Local<Object> TextEncoding::CreateBinding(Local<Context> context) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    Local<Object> binding = Object::New(isolate);

    tns::SetMethodNoSideEffect(context, binding, "labelToEncoding",
                               LabelToEncodingCallback);
    tns::SetMethod(context, binding, "decode", DecodeCallback);
    tns::SetMethodNoSideEffect(context, binding, "encodeUtf8",
                               EncodeUtf8Callback);
#if NATIVESCRIPT_ENABLE_FAST_API
    tns::SetFastMethod(context, binding, "encodeInto", EncodeIntoCallback,
                       &kFastEncodeInto);
#else
    tns::SetMethod(context, binding, "encodeInto", EncodeIntoCallback);
#endif

    return binding;
}

}  // namespace tns
