// Inside the scope: "ns-scoped-leaf" is overridden there, while
// "ns-scoped-fallthrough" is not and must fall through to top-level imports.
import { name as scoped } from "ns-scoped-leaf";
import { name as fellThrough } from "ns-scoped-fallthrough";

export const leaf = scoped;
export const fallthrough = fellThrough;
