"use strict";

// The `ns:module` builtin: the dev-loader control surface the runtime
// exposes to development tooling (docs/ns-builtin-modules.md). Every member
// is a native function handed in through `binding`; this file only shapes
// and freezes the exports.
//
// Membership varies by build:
//   - `canonicalizeHttpUrlKey` exists only in debug builds (test diagnostic).
// Missing members are simply absent — never present-but-throwing — so
// feature checks work.

const {
  ArrayPrototypeIndexOf,
  decodeURIComponent,
  NumberIsFinite,
  ObjectFreeze,
  ObjectKeys,
  StringPrototypeEndsWith,
  StringPrototypeIndexOf,
  StringPrototypeLastIndexOf,
  StringPrototypeSlice,
  StringPrototypeStartsWith,
  TypeError,
} = primordials;

// Node's wording (lib/internal/modules/cjs/loader.js), so a message copied out
// of a NativeScript stack trace still matches what the ecosystem documents.
const CREATE_REQUIRE_ERROR =
  "The argument 'filename' must be a file URL object, file URL string, or absolute path string.";

// A `file:` URL string down to the path it names. Deliberately string-based
// rather than routed through the global URL: this runs before app code and
// must not depend on an intrinsic the app may have replaced.
function fileUrlToPath(href) {
  let rest = StringPrototypeSlice(href, "file://".length);

  // Only an empty or localhost authority names a local file.
  const authorityEnd = StringPrototypeIndexOf(rest, "/");
  if (authorityEnd < 0) {
    throw new TypeError(CREATE_REQUIRE_ERROR);
  }
  const authority = StringPrototypeSlice(rest, 0, authorityEnd);
  if (authority !== "" && authority !== "localhost") {
    throw new TypeError(CREATE_REQUIRE_ERROR);
  }
  rest = StringPrototypeSlice(rest, authorityEnd);

  // The query and fragment are URL syntax, never part of the path.
  const queryAt = StringPrototypeIndexOf(rest, "?");
  if (queryAt >= 0) {
    rest = StringPrototypeSlice(rest, 0, queryAt);
  }
  const hashAt = StringPrototypeIndexOf(rest, "#");
  if (hashAt >= 0) {
    rest = StringPrototypeSlice(rest, 0, hashAt);
  }

  try {
    return decodeURIComponent(rest);
  } catch {
    throw new TypeError(CREATE_REQUIRE_ERROR);
  }
}

// The directory a require created for `filenameOrURL` resolves against.
function requireBaseDir(filenameOrURL) {
  let filepath;

  if (typeof filenameOrURL === "object" && filenameOrURL !== null) {
    // A URL object, identified by its href rather than by instanceof so a
    // URL from another realm still works.
    const href = filenameOrURL.href;
    if (typeof href !== "string") {
      throw new TypeError(CREATE_REQUIRE_ERROR);
    }
    filepath = urlStringToPath(href);
  } else if (typeof filenameOrURL !== "string") {
    throw new TypeError(CREATE_REQUIRE_ERROR);
  } else if (StringPrototypeStartsWith(filenameOrURL, "/")) {
    filepath = filenameOrURL;
  } else {
    filepath = urlStringToPath(filenameOrURL);
  }

  // Node treats a trailing slash as "this directory is the base"; otherwise
  // the base is the directory holding the named file.
  if (StringPrototypeEndsWith(filepath, "/")) {
    const trimmed = StringPrototypeSlice(filepath, 0, filepath.length - 1);
    return trimmed === "" ? "/" : trimmed;
  }
  const lastSlash = StringPrototypeLastIndexOf(filepath, "/");
  return lastSlash <= 0 ? "/" : StringPrototypeSlice(filepath, 0, lastSlash);
}

function urlStringToPath(value) {
  if (StringPrototypeStartsWith(value, "file://")) {
    return fileUrlToPath(value);
  }
  if (StringPrototypeStartsWith(value, "http://") ||
      StringPrototypeStartsWith(value, "https://")) {
    // require() over HTTP is blocked runtime-wide; a dev-served module is
    // reachable through import(), and a require base must name a real file.
    throw new TypeError(
      "createRequire() cannot take an http(s) URL (" + value +
        "): require() of a dev-served module is not supported. Pass an app-root " +
        "file path and use import() for remote modules.");
  }
  throw new TypeError(CREATE_REQUIRE_ERROR);
}

// Every option a pumping require accepts, so an unknown key is a typo the
// caller hears about rather than a setting that silently does nothing.
const kPumpingOptionKeys = ["deadlineSeconds", "onTimeout", "pumpRunLoop"];

// Validated once, when the require is minted — a require() call itself does no
// option work at all. Returns the three values the native mint expects, with
// `undefined` standing for "leave the default alone".
function validatePumpingOptions(options) {
  if (options === undefined) {
    return { deadlineSeconds: undefined, throwOnTimeout: undefined, pumpRunLoop: undefined };
  }
  if (typeof options !== "object" || options === null) {
    throw new TypeError("createPumpingRequire: options must be an object");
  }

  const keys = ObjectKeys(options);
  for (let i = 0; i < keys.length; i++) {
    if (ArrayPrototypeIndexOf(kPumpingOptionKeys, keys[i]) < 0) {
      throw new TypeError("createPumpingRequire: unknown option '" + keys[i] + "'");
    }
  }

  const deadlineSeconds = options.deadlineSeconds;
  if (deadlineSeconds !== undefined &&
      (typeof deadlineSeconds !== "number" || !NumberIsFinite(deadlineSeconds) ||
       deadlineSeconds <= 0)) {
    throw new TypeError(
      "createPumpingRequire: 'deadlineSeconds' must be a positive finite number");
  }

  const onTimeout = options.onTimeout;
  if (onTimeout !== undefined && onTimeout !== "throw" && onTimeout !== "return-pending") {
    throw new TypeError("createPumpingRequire: 'onTimeout' must be 'throw' or 'return-pending'");
  }

  const pumpRunLoop = options.pumpRunLoop;
  if (pumpRunLoop !== undefined && typeof pumpRunLoop !== "boolean") {
    throw new TypeError("createPumpingRequire: 'pumpRunLoop' must be a boolean");
  }

  return {
    deadlineSeconds,
    throwOnTimeout: onTimeout === undefined ? undefined : onTimeout === "throw",
    pumpRunLoop,
  };
}

function createRequire(filenameOrURL, options) {
  if (options !== undefined) {
    throw new TypeError("options are not supported on createRequire");
  }
  return binding.createRequire(requireBaseDir(filenameOrURL), false);
}

function createPumpingRequire(filenameOrURL, options) {
  const resolved = validatePumpingOptions(options);
  return binding.createRequire(requireBaseDir(filenameOrURL), true, resolved.deadlineSeconds,
                               resolved.throwOnTimeout, resolved.pumpRunLoop);
}

const surface = {
  configureLoader: binding.configureLoader,
  invalidateModules: binding.invalidateModules,
  getLoadedModuleUrls: binding.getLoadedModuleUrls,
  setDevBootComplete: binding.setDevBootComplete,
  createRequire,
  createPumpingRequire,
};
if (binding.canonicalizeHttpUrlKey !== undefined) {
  surface.canonicalizeHttpUrlKey = binding.canonicalizeHttpUrlKey;
}

module.exports = ObjectFreeze(surface);
