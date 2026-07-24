// HTTP canonical-key identity tests.
//
// Pins the behavior of the native CanonicalizeHttpUrlKey (the loader/registry
// key) via the debug-only __NS_DEV__.canonicalizeHttpUrlKey diagnostic. Pure
// string logic — no dev server required.
//
// Module identity IS the canonical URL: the dev server serves every module
// under one URL and freshness is handled by __NS_DEV__.invalidateModules
// (registry + prewarm-cache evict + fetch nonce), never by URL variation.
// There is deliberately no path-tag vocabulary (__ns_boot__/__ns_hmr__)
// to collapse, and no versioned-bridge-endpoint normalization.

describe("HTTP canonical key", function () {
    function canonFn() {
        const dev = globalThis.__NS_DEV__;
        return dev && typeof dev.canonicalizeHttpUrlKey === "function"
            ? dev.canonicalizeHttpUrlKey
            : null;
    }
    function canon(url) {
        return canonFn()(url);
    }

    it("is available in dev builds", function () {
        if (!canonFn()) {
            pending("__NS_DEV__.canonicalizeHttpUrlKey not available (release build?)");
            return;
        }
        expect(typeof canonFn()).toBe("function");
    });

    it("drops the fragment and unwraps file://http wrappers", function () {
        if (!canonFn()) { pending("release"); return; }
        expect(canon("http://h/ns/m/foo.js#frag")).toBe("http://h/ns/m/foo.js");
        expect(canon("file://http://h/x.js")).toBe("http://h/x.js");
    });

    it("does NOT collapse versioned endpoint paths or path tags", function () {
        if (!canonFn()) { pending("release"); return; }
        // Path is identity — no /ns/rt/<v> → /ns/rt collapse, no
        // __ns_hmr__/__ns_boot__ tag folding.
        expect(canon("http://h/ns/rt/42")).toBe("http://h/ns/rt/42");
        expect(canon("http://h/ns/rt/42/x.js")).toBe("http://h/ns/rt/42/x.js");
        expect(canon("http://h/ns/m/__ns_hmr__/v7/foo.js"))
            .toBe("http://h/ns/m/__ns_hmr__/v7/foo.js");
    });

    it("strips import/t/v markers and sorts remaining params on dev endpoints", function () {
        if (!canonFn()) { pending("release"); return; }
        expect(canon("http://h/ns/m/a?import=1&b=2&a=3")).toBe("http://h/ns/m/a?a=3&b=2");
        expect(canon("http://h/ns/m/a?b=2&a=1")).toBe("http://h/ns/m/a?a=1&b=2");
        expect(canon("http://h/ns/core?import=1")).toBe("http://h/ns/core");
        expect(canon("http://h/ns/m/a?t=123&v=abc&x=1")).toBe("http://h/ns/m/a?x=1");
    });

    it("preserves the query verbatim on non-dev endpoints", function () {
        if (!canonFn()) { pending("release"); return; }
        // Public-internet module URLs: the query can be part of identity
        // (auth, content versioning, routing) — only the fragment is dropped.
        expect(canon("http://h/a?import=1&b=2&a=3")).toBe("http://h/a?import=1&b=2&a=3");
        expect(canon("https://cdn.example.com/pkg.js?token=x#frag"))
            .toBe("https://cdn.example.com/pkg.js?token=x");
    });

    it("preserves the t param on @ng/component endpoints", function () {
        if (!canonFn()) { pending("release"); return; }
        // Angular HMR component-update endpoint: `t` identifies a specific
        // recompile and must remain a distinct registry entry.
        expect(canon("http://h/ns/m/app/@ng/component?c=x&t=111"))
            .toBe("http://h/ns/m/app/@ng/component?c=x&t=111");
    });

    it("leaves a non-http(s) specifier unchanged", function () {
        if (!canonFn()) { pending("release"); return; }
        expect(canon("~/local/foo.js")).toBe("~/local/foo.js");
    });
});
