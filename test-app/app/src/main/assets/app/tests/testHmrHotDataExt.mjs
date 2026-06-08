// HMR import.meta.hot.data sharing tests.
//
// These tests exercise the canonical-key extension-collapse path:
// importing the *same* logical module under `.mjs` and `.js` extensions
// MUST yield the same `import.meta.hot.data` object identity, so that
// state written from one variant is observable in the other.
//
// The two fixture files under `tests/esm/hmr/` MUST remain independent
// (no re-export of one from the other) — see the comment header in
// each fixture for why.
//
// HTTP-loader variants of these tests (live-tagged, boot-tagged, and
// /ns/core bridge URLs) live in HttpEsmLoaderTests on iOS. They depend
// on a dev-server harness that Android does not currently stand up,
// and are intentionally not ported here. The local twin-file path
// below still exercises the core canonical-key normalization.

describe("HMR hot.data", function () {
    it("exposes the import.meta.hot API surface", async function () {
        const mod = await import("~/tests/esm/hmr/hot-data-ext.mjs");
        expect(mod).toBeTruthy();
        expect(typeof mod.testHotApi).toBe("function");

        const result = mod.testHotApi();
        expect(result).toBeTruthy();
        if (!result.hasHot) {
            pending("import.meta.hot not available (release build?)");
            return;
        }

        expect(result.hasData).toBe(true);
        expect(result.hasAccept).toBe(true);
        expect(result.hasDispose).toBe(true);
        expect(result.hasDecline).toBe(true);
        expect(result.hasInvalidate).toBe(true);
        expect(result.hasPrune).toBe(true);
        expect(result.ok).toBe(true);
    });

    it("should share hot.data across .mjs and .js variants", async function () {
        const [mjs, js] = await Promise.all([
            import("~/tests/esm/hmr/hot-data-ext.mjs"),
            import("~/tests/esm/hmr/hot-data-ext.js"),
        ]);

        const hotMjs = mjs && typeof mjs.getHot === "function" ? mjs.getHot() : null;
        const hotJs = js && typeof js.getHot === "function" ? js.getHot() : null;
        if (!hotMjs || !hotJs) {
            pending("import.meta.hot not available (release build?)");
            return;
        }

        const dataMjs = mjs.getHotData();
        const dataJs = js.getHotData();
        expect(dataMjs).toBeDefined();
        expect(dataJs).toBeDefined();

        const token = "tok_" + Date.now() + "_" + Math.random();
        mjs.setHotValue(token);
        expect(js.getHotValue()).toBe(token);

        // Canonical hot key strips common script extensions, so these must share identity.
        expect(dataMjs).toBe(dataJs);
    });
});
