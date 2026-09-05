import defaultValue, { relativeValue } from "./dependency.mjs?v=static";

export const viaDefault = defaultValue;
export const viaNamed = relativeValue;

export function loadWithQuery() {
    return import("./dependency.mjs?v=dynamic");
}
