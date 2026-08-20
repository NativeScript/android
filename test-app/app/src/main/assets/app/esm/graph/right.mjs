import { name as sharedName } from "./shared.mjs";

(globalThis.__nsGraphOrder = globalThis.__nsGraphOrder || []).push("right");

export const seenShared = sharedName;
export const name = "right";
