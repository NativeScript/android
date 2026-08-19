// The ESM route into the node:module shim: a static import, resolved by the
// engine's module resolver rather than by require().
import { createRequire } from "node:module";

export const createRequireType = typeof createRequire;

export function requireFrom(filename, specifier) {
    return createRequire(filename)(specifier);
}
