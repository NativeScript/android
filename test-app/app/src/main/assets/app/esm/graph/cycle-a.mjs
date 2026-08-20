import { bValue, describeA } from "./cycle-b.mjs";

export function describeB() {
    return "a-saw-" + bValue;
}

export const aValue = "a";
export const roundTrip = describeA();
