// HTTP canonical-key identity tests.
//
// Pins the behavior of the native CanonicalizeHttpUrlKey (the loader/registry
// key) via the debug-only __nsCanonicalizeHttpUrlKey diagnostic global. Pure
// string logic — no dev server required. Android does NOT collapse the
// __ns_boot__/__ns_hmr__ virtual prefixes here (that is canonicalHotKey's job),
// which these specs assert explicitly.

describe("HTTP canonical key", function () {
    function canon(url) {
        return globalThis.__nsCanonicalizeHttpUrlKey(url);
    }

    it("is available in dev builds", function () {
        if (typeof globalThis.__nsCanonicalizeHttpUrlKey !== "function") {
            pending("__nsCanonicalizeHttpUrlKey not available (release build?)");
            return;
        }
        expect(typeof globalThis.__nsCanonicalizeHttpUrlKey).toBe("function");
    });

    it("drops the fragment and unwraps file://http wrappers", function () {
        if (typeof globalThis.__nsCanonicalizeHttpUrlKey !== "function") { pending("release"); return; }
        expect(canon("http://h/ns/m/foo.js#frag")).toBe("http://h/ns/m/foo.js");
        expect(canon("file://http://h/x.js")).toBe("http://h/x.js");
    });

    it("normalizes versioned bridge endpoints but not deeper paths", function () {
        if (typeof globalThis.__nsCanonicalizeHttpUrlKey !== "function") { pending("release"); return; }
        expect(canon("http://h/ns/rt/42")).toBe("http://h/ns/rt");
        expect(canon("http://h/ns/core/13")).toBe("http://h/ns/core");
        expect(canon("http://h/ns/rt/42/x.js")).toBe("http://h/ns/rt/42/x.js");
    });

    it("does NOT collapse __ns_hmr__/__ns_boot__ prefixes (Android loader key)", function () {
        if (typeof globalThis.__nsCanonicalizeHttpUrlKey !== "function") { pending("release"); return; }
        expect(canon("http://h/ns/m/__ns_hmr__/v7/foo.js"))
            .toBe("http://h/ns/m/__ns_hmr__/v7/foo.js");
    });

    it("strips ?import and sorts remaining query params", function () {
        if (typeof globalThis.__nsCanonicalizeHttpUrlKey !== "function") { pending("release"); return; }
        expect(canon("http://h/a?import=1&b=2&a=3")).toBe("http://h/a?a=3&b=2");
        expect(canon("http://h/a?b=2&a=1")).toBe("http://h/a?a=1&b=2");
        expect(canon("http://h/ns/core?import=1")).toBe("http://h/ns/core");
    });

    it("leaves a non-http(s) specifier unchanged", function () {
        if (typeof globalThis.__nsCanonicalizeHttpUrlKey !== "function") { pending("release"); return; }
        expect(canon("~/local/foo.js")).toBe("~/local/foo.js");
    });
});
