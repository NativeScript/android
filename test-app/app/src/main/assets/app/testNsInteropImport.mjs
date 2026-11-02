// Static `import` of the interop builtins, so the ES module resolve callback
// handles a specifier with a path segment (`ns:interop/android`).
import interopDefault from "ns:interop";
import androidDefault, { long, newArray } from "ns:interop/android";
import { escapeException } from "ns:runtime";

export { interopDefault, androidDefault, long, newArray, escapeException };
