import { name as sharedName } from "./shared.mjs";

(globalThis.__nsGraphOrder = globalThis.__nsGraphOrder || []).push("left");

export const seenShared = sharedName;
export const name = "left";
