"use strict";
// atob / btoa (HTML Standard §8.3, base64 utility methods) over the WHATWG
// forgiving-base64 codec in Base64.cpp.
//
// This file exports the two functions instead of installing them; the C++
// lazy-global tier (LazyGlobals) places them and is what runs this file, on
// the first read of either name. Anything needed from a sibling builtin
// comes through `require` or `binding`, never init order — see the README.
//
// The native ops answer null on failure rather than throwing, so the
// exception shape — an "InvalidCharacterError" DOMException, per the HTML
// spec — stays here, required on first failure so well-formed input never
// runs the dom-exception builtin.
const { TypeError } = primordials;

const { atob: decodeBase64, btoa: encodeBase64 } = binding;

let DOMException;
function invalidCharacterError() {
  if (DOMException === undefined) {
    ({ DOMException } = require("internal/dom-exception"));
  }
  return new DOMException("Invalid character", "InvalidCharacterError");
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
