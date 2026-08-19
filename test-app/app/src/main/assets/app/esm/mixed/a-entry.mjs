import { leaf as leafName } from "./a-mid.mjs";

(globalThis.__nsMixedOrdera = globalThis.__nsMixedOrdera || []).push("entry");

export const leaf = leafName;
export const order = globalThis.__nsMixedOrdera.slice();
