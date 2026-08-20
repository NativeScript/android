import { name as leftName } from "./left.mjs";
import { name as rightName } from "./right.mjs";

(globalThis.__nsGraphOrder = globalThis.__nsGraphOrder || []).push("entry");

export const order = globalThis.__nsGraphOrder.slice();
export const names = [leftName, rightName];
