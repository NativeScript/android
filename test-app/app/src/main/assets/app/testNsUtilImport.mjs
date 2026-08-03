// Static `import` of the builtins, so the ES module resolve callback is
// exercised too (dynamic import() has its own fast path).
import nsDefault, { inspect, format } from "ns:util";
import nodeDefault, { inspect as nodeInspect } from "node:util";

export { nsDefault, inspect, format, nodeDefault, nodeInspect };
