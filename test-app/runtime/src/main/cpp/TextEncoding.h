#ifndef TEXTENCODING_H_
#define TEXTENCODING_H_

#include "v8.h"

namespace tns {

/*
 * Native ops behind the text-encoding builtin (internal/text-encoding.js):
 * the WHATWG label table, UTF-8 encoding and the decoders for the encodings
 * the runtime supports (utf-8, utf-16le, utf-16be, windows-1252). The builtin
 * owns the web-facing shapes; everything that touches bytes lives here.
 */
class TextEncoding {
public:
    /*
     * The builtin's exports, `{ TextEncoder, TextDecoder }`, from the one run
     * it gets per isolate. The lazy globals and `ns:util` both hand out these
     * objects, so require("ns:util").TextDecoder === globalThis.TextDecoder
     * whichever is reached first.
     */
    static v8::MaybeLocal<v8::Object> GetExports(v8::Local<v8::Context> context);

    // Bytes of decoder state the builtin must hand back on every decode call.
    static constexpr int kDecoderStateSize = 16;
};

}  // namespace tns

#endif /* TEXTENCODING_H_ */
