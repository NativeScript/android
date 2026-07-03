// Fixture for testNsDevBoundary: reports whether the runtime attached a
// native `import.meta.hot` (it must NOT — hot contexts are injected by the
// JS dev client via source rewrite, never by the runtime).
export const hasHot = typeof import.meta.hot !== "undefined";
export const hotValue = import.meta.hot;
