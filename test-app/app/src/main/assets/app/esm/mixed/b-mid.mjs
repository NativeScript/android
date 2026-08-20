// The HTTP leaf arrives through a bare specifier the import map points at a
// ModuleTestServer URL, so this file needs no baked-in origin.
import { name as leafName } from "ns-test-leaf-b";

(globalThis.__nsMixedOrderb = globalThis.__nsMixedOrderb || []).push("mid");

export const leaf = leafName;
