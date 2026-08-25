"use strict";
// TextEncoder / TextDecoder (WHATWG Encoding Standard,
// https://encoding.spec.whatwg.org).
//
// This file exports the two interfaces instead of installing them; the C++
// lazy-global tier (LazyGlobals) places them and is what runs this file, on
// the first read of either name. Anything needed from a sibling builtin
// comes through `require` or `binding`, never init order — see the README.
//
// The supported encodings (utf-8, utf-16le, utf-16be, windows-1252) with
// their complete label sets, the decoders and the UTF-8 encoder all live in
// TextEncoding.cpp. Per-decoder streaming state is the Uint8Array this file
// allocates and the native decoder reads and rewrites, so a TextDecoder needs
// neither a native handle nor a finalizer.
const {
  ObjectDefineProperty,
  ObjectGetOwnPropertyDescriptor,
  RangeError,
  SymbolToStringTag,
  TypeError,
  Uint8Array,
} = primordials;

const {
  labelToEncoding,
  decode,
  encodeUtf8,
  encodeInto,
  encodeIntoFallback,
  encodeIntoResults,
} = binding;

// Indexed by the encoding ids labelToEncoding returns.
const kEncodingNames = ["utf-8", "utf-16le", "utf-16be", "windows-1252"];

// Mirror the kFlag* constants in TextEncoding.cpp.
const kFlagFatal = 1;
const kFlagIgnoreBOM = 2;
const kFlagStream = 4;

// Mirrors TextEncoding::kDecoderStateSize.
const kDecoderStateSize = 16;

// Mirror the kEncodeInto* status codes in TextEncoding.cpp. The op reports
// {read, written} through binding.encodeIntoResults rather than allocating a
// result object per call; it is synchronous, so that one native Uint32Array
// serves every encoder in the isolate.
const kEncodeIntoBadDestination = 1;
const kEncodeIntoRetrySlow = 2;

// WebIDL dictionary conversion: undefined and null mean "all defaults",
// anything else must be an object.
function toDictionary(value, name) {
  if (value === undefined || value === null) {
    return undefined;
  }
  if (typeof value !== "object" && typeof value !== "function") {
    throw new TypeError(`The "${name}" argument must be an object`);
  }
  return value;
}

class TextEncoder {
  #brand;

  static #check(receiver) {
    if (!(#brand in receiver)) {
      throw new TypeError("Illegal invocation");
    }
  }

  get encoding() {
    TextEncoder.#check(this);
    return "utf-8";
  }

  encode(input = "") {
    TextEncoder.#check(this);
    return encodeUtf8(`${input}`);
  }

  encodeInto(source, destination) {
    TextEncoder.#check(this);
    const text = `${source}`;
    let code = encodeInto(text, destination, encodeIntoResults);
    if (code === kEncodeIntoRetrySlow) {
      code = encodeIntoFallback(text, destination, encodeIntoResults);
    }
    if (code === kEncodeIntoBadDestination) {
      throw new TypeError(
        'The "destination" argument must be an instance of Uint8Array'
      );
    }
    return { read: encodeIntoResults[0], written: encodeIntoResults[1] };
  }
}

class TextDecoder {
  #encoding;
  #fatal;
  #ignoreBOM;
  #flags;
  #state;

  static #check(receiver) {
    if (!(#encoding in receiver)) {
      throw new TypeError("Illegal invocation");
    }
  }

  constructor(label = "utf-8", options = undefined) {
    const name = `${label}`;
    const dictionary = toDictionary(options, "options");
    const encoding = labelToEncoding(name);
    if (encoding < 0) {
      throw new RangeError(`The encoding "${name}" is not supported`);
    }
    const fatal = dictionary !== undefined && !!dictionary.fatal;
    const ignoreBOM = dictionary !== undefined && !!dictionary.ignoreBOM;
    this.#encoding = encoding;
    this.#fatal = fatal;
    this.#ignoreBOM = ignoreBOM;
    this.#flags = (fatal ? kFlagFatal : 0) | (ignoreBOM ? kFlagIgnoreBOM : 0);
    this.#state = new Uint8Array(kDecoderStateSize);
  }

  get encoding() {
    TextDecoder.#check(this);
    return kEncodingNames[this.#encoding];
  }

  get fatal() {
    TextDecoder.#check(this);
    return this.#fatal;
  }

  get ignoreBOM() {
    TextDecoder.#check(this);
    return this.#ignoreBOM;
  }

  decode(input = undefined, options = undefined) {
    TextDecoder.#check(this);
    const dictionary = toDictionary(options, "options");
    const stream = dictionary !== undefined && !!dictionary.stream;
    return decode(
      input,
      this.#encoding,
      stream ? this.#flags | kFlagStream : this.#flags,
      this.#state
    );
  }
}

// WebIDL shape: interface members are enumerable prototype properties and the
// class string is a configurable, non-writable Symbol.toStringTag; class
// syntax alone yields non-enumerable members.
function finishInterface(ctor, tag, members) {
  const proto = ctor.prototype;
  ObjectDefineProperty(proto, SymbolToStringTag, {
    value: tag,
    writable: false,
    enumerable: false,
    configurable: true,
  });
  for (let i = 0; i < members.length; i++) {
    const desc = ObjectGetOwnPropertyDescriptor(proto, members[i]);
    desc.enumerable = true;
    ObjectDefineProperty(proto, members[i], desc);
  }
}
finishInterface(TextEncoder, "TextEncoder", [
  "encoding",
  "encode",
  "encodeInto",
]);
finishInterface(TextDecoder, "TextDecoder", [
  "encoding",
  "fatal",
  "ignoreBOM",
  "decode",
]);

module.exports = { TextEncoder, TextDecoder };
