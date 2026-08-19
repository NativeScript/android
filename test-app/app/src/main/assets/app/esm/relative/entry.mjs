import defaultValue, { relativeValue, getDependencyPayload } from "./dependency.mjs";

export const viaDefault = defaultValue;
export const viaNamed = relativeValue;

export function readDependencyPayload() {
    const payload = getDependencyPayload();
    return typeof payload === "object" && payload.from === "dependency";
}
