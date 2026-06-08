// HMR hot.data test module (.js).
//
// INTENTIONAL twin of hot-data-ext.mjs. Two physical files with
// different extensions are required so the HMR canonical-key
// extension-collapse path is actually exercised by tests that import
// BOTH variants (see testHmrHotDataExt
// "should share hot.data across .mjs and .js variants"). Each file
// MUST own its own `import.meta.hot` reference — re-exporting from the
// sibling would defeat the test, because `dataMjs === dataJs` would
// then hold trivially via function identity instead of validating the
// runtime's canonical-key normalization.
//
// Keep the body in lock-step with `hot-data-ext.mjs`.

export function getHot() {
    return (typeof import.meta !== "undefined" && import.meta) ? import.meta.hot : undefined;
}

export function getHotData() {
    const hot = getHot();
    return hot ? hot.data : undefined;
}

export function setHotValue(value) {
    const hot = getHot();
    if (!hot || !hot.data) {
        throw new Error("import.meta.hot.data is not available");
    }
    hot.data.value = value;
    return hot.data.value;
}

export function getHotValue() {
    const hot = getHot();
    return hot && hot.data ? hot.data.value : undefined;
}

export function testHotApi() {
    const hot = getHot();
    const result = {
        ok: false,
        hasHot: !!hot,
        hasData: !!(hot && hot.data),
        hasAccept: !!(hot && typeof hot.accept === "function"),
        hasDispose: !!(hot && typeof hot.dispose === "function"),
        hasDecline: !!(hot && typeof hot.decline === "function"),
        hasInvalidate: !!(hot && typeof hot.invalidate === "function"),
        hasPrune: !!(hot && typeof hot.prune === "function"),
    };

    try {
        if (hot && typeof hot.accept === "function") {
            hot.accept(function () {});
        }
        if (hot && typeof hot.dispose === "function") {
            hot.dispose(function () {});
        }
        if (hot && typeof hot.decline === "function") {
            hot.decline();
        }
        if (hot && typeof hot.invalidate === "function") {
            hot.invalidate();
        }
        result.ok =
            result.hasHot &&
            result.hasData &&
            result.hasAccept &&
            result.hasDispose &&
            result.hasDecline &&
            result.hasInvalidate &&
            result.hasPrune;
    } catch (e) {
        result.error = (e && e.message) ? e.message : String(e);
    }

    return result;
}

console.log("HMR hot.data ext module loaded (.js)");
