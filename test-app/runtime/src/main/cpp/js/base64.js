"use strict";
// atob / btoa (HTML Standard §8.3, base64 utility methods) over the WHATWG
// forgiving-base64 codec in Base64.cpp.
//
// This file exports the two functions instead of installing them; the C++
// lazy-global tier (LazyGlobals) places them and is what runs this file, on
// the first read of either name. Nothing here may depend on a builtin that
// runs after it, so `internals` is off limits — see the README.
//
// Deliberate deviation from the spec: no DOMException in this runtime, so the
// failure is an Error with `name` patched to "InvalidCharacterError", the same
// stand-in abort-signal.js and performance.js use. The native ops answer null
// on failure rather than throwing, so that shape stays here.
const { Error, TypeError } = primordials;

const { atob: decodeBase64, btoa: encodeBase64 } = binding;

function invalidCharacterError() {
  const e = new Error("Invalid character");
  e.name = "InvalidCharacterError";
  return e;
}

function btoa(data) {
  if (arguments.length < 1) {
    throw new TypeError("btoa requires 1 argument");
  }
  const result = encodeBase64(`${data}`);
  if (result === null) {
    throw invalidCharacterError();
  }
  return result;
}

function atob(data) {
  if (arguments.length < 1) {
    throw new TypeError("atob requires 1 argument");
  }
  const result = decodeBase64(`${data}`);
  if (result === null) {
    throw invalidCharacterError();
  }
  return result;
}

module.exports = { atob, btoa };
