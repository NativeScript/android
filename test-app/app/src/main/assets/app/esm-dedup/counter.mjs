// Singleton module: an ES module is evaluated once, so every importer must
// observe this same `state` object regardless of how the specifier spelled the
// path to this file.
export const state = { count: 0 };

export function increment() {
    state.count++;
}
