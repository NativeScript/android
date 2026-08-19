import { leaf as leafName } from "./b-mid.mjs";

(globalThis.__nsMixedOrderb = globalThis.__nsMixedOrderb || []).push("entry");

export const leaf = leafName;
export const order = globalThis.__nsMixedOrderb.slice();
