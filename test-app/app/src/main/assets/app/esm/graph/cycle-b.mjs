import { aValue } from "./cycle-a.mjs";

export function describeA() {
    return "b-saw-" + aValue;
}

export const bValue = "b";
