"use strict";

// The `ns:util` builtin module. See docs/ns-builtin-modules.md for the
// cross-runtime contract. `inspect` is created once per realm by
// Console::getInspect and handed in here, so the module and console share one
// formatter instance.

const { inspect } = binding;
const {
  JSONStringify,
  Number,
  NumberParseFloat,
  NumberParseInt,
  ObjectFreeze,
  ObjectIs,
  String,
  StringPrototypeCharCodeAt,
  StringPrototypeSlice,
} = primordials;

const CHAR_PERCENT = 37;
const CHAR_UPPERCASE_O = 79;
const CHAR_LOWERCASE_D = 100;
const CHAR_LOWERCASE_F = 102;
const CHAR_LOWERCASE_I = 105;
const CHAR_LOWERCASE_J = 106;
const CHAR_LOWERCASE_O = 111;
const CHAR_LOWERCASE_S = 115;

function formatNumber(value) {
  return ObjectIs(value, -0) ? "-0" : String(value);
}

// %d and %i keep a bigint's "n" suffix (%f, which parses a float, does not).
// Symbols refuse numeric conversion, so they render as NaN rather than throw.
function formatInteger(value, toNumber) {
  if (typeof value === "symbol") {
    return "NaN";
  }
  if (typeof value === "bigint") {
    return String(value) + "n";
  }
  return formatNumber(toNumber(value));
}

function stringify(value) {
  try {
    return JSONStringify(value);
  } catch (ignored) {
    // Circular structures are the common case; anything else JSON refuses
    // (a throwing toJSON) is not worth taking the log call down for.
    return "[Circular]";
  }
}

// A value spliced in for %s: strings go in raw, everything else through
// inspect (which already honors custom toString overrides).
function toDisplayString(value) {
  return typeof value === "string" ? value : inspect(value);
}

function format(...args) {
  const first = args[0];
  let str = "";
  let a = 0;
  let join = "";

  if (typeof first === "string") {
    if (args.length === 1) {
      return first;
    }
    let lastPos = 0;
    // A trailing "%" cannot start a substitution, hence length - 1.
    for (let i = 0; i < first.length - 1; i++) {
      if (StringPrototypeCharCodeAt(first, i) !== CHAR_PERCENT) {
        continue;
      }
      const nextChar = StringPrototypeCharCodeAt(first, ++i);
      if (a + 1 === args.length) {
        // Out of arguments: only %% still means something.
        if (nextChar === CHAR_PERCENT) {
          str += StringPrototypeSlice(first, lastPos, i);
          lastPos = i + 1;
        }
        continue;
      }
      let replacement;
      switch (nextChar) {
        case CHAR_LOWERCASE_S:
          replacement = toDisplayString(args[++a]);
          break;
        case CHAR_LOWERCASE_D: {
          const value = args[++a];
          replacement = formatInteger(value, Number);
          break;
        }
        case CHAR_LOWERCASE_I: {
          const value = args[++a];
          replacement = formatInteger(value, NumberParseInt);
          break;
        }
        case CHAR_LOWERCASE_F: {
          const value = args[++a];
          replacement = typeof value === "symbol" ? "NaN" : formatNumber(NumberParseFloat(value));
          break;
        }
        case CHAR_LOWERCASE_J:
          replacement = stringify(args[++a]);
          break;
        case CHAR_LOWERCASE_O:
        case CHAR_UPPERCASE_O:
          replacement = inspect(args[++a]);
          break;
        case CHAR_PERCENT:
          str += StringPrototypeSlice(first, lastPos, i);
          lastPos = i + 1;
          continue;
        default:
          // Not a placeholder: "100%" and friends survive verbatim.
          continue;
      }
      if (lastPos !== i - 1) {
        str += StringPrototypeSlice(first, lastPos, i - 1);
      }
      str += replacement;
      lastPos = i + 1;
    }
    if (lastPos !== 0) {
      // The format string consumed argument 0 and set the separator for the
      // extras that follow it.
      a++;
      join = " ";
      if (lastPos < first.length) {
        str += StringPrototypeSlice(first, lastPos);
      }
    }
  }

  // Leftovers (or every argument, when there was no format string) are
  // appended space-separated.
  while (a < args.length) {
    const value = args[a];
    str += join + toDisplayString(value);
    join = " ";
    a++;
  }

  return str;
}

module.exports = ObjectFreeze({ inspect, format });
