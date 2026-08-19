describe("ns:module", function () {
    it("should expose the dev-loader primitives via the ns:module builtin", function () {
        var nsModule = require("ns:module");
        expect(Object.isFrozen(nsModule)).toBe(true);
        expect(typeof nsModule.configureLoader).toBe("function");
        expect(typeof nsModule.invalidateModules).toBe("function");
        expect(typeof nsModule.getLoadedModuleUrls).toBe("function");
        expect(typeof nsModule.createRequire).toBe("function");
        expect(typeof nsModule.createPumpingRequire).toBe("function");
        expect(nsModule.terminateAllWorkers).toBeUndefined();
        expect(global.__NS_DEV__).toBeUndefined();
    });

    it("exposes exactly the declared surface", function () {
        var nsModule = require("ns:module");
        var expected = ["configureLoader", "createPumpingRequire", "createRequire",
                        "getLoadedModuleUrls", "invalidateModules"];
        if (typeof nsModule.canonicalizeHttpUrlKey === "function") {
            expected.push("canonicalizeHttpUrlKey");
        }
        expect(Object.keys(nsModule).sort()).toEqual(expected.sort());
    });

    it("resolves ns:module to the same members for require and import()", function (done) {
        var nsModule = require("ns:module");
        import("ns:module").then(function (ns) {
            expect(ns.default).toBe(nsModule);
            expect(ns.invalidateModules).toBe(nsModule.invalidateModules);
            expect(ns.configureLoader).toBe(nsModule.configureLoader);
            done();
        }).catch(function (error) {
            fail("import('ns:module') rejected: " + error.message);
            done();
        });
    });

    // Boot state is derived natively from the entry-evaluation window; there is
    // no client signal and no JS-visible mirror.
    it("exposes no boot-complete signal", function () {
        var nsModule = require("ns:module");
        expect(nsModule.setDevBootComplete).toBeUndefined();
        expect(global.__NS_HMR_BOOT_COMPLETE__).toBeUndefined();
    });
});

describe("HTTP canonical key (ns:module canonicalizeHttpUrlKey)", function () {
    function getCanon() {
        return require("ns:module").canonicalizeHttpUrlKey;
    }

    function checkKey(input, expected) {
        var canon = getCanon();
        if (typeof canon !== "function") {
            pending("ns:module.canonicalizeHttpUrlKey not exposed (release build)");
            return;
        }
        expect(canon(input)).toBe(expected);
    }

    it("is exposed as a function in debug builds", function () {
        var canon = getCanon();
        if (typeof canon !== "function") {
            pending("ns:module.canonicalizeHttpUrlKey not exposed (release build)");
            return;
        }
        expect(typeof canon).toBe("function");
    });

    // Unconfigured, canonicalization is purely mechanical: the fragment goes
    // and the query stays. Which params are cache-busters and which paths are
    // dev endpoints is client vocabulary the runtime no longer guesses.
    it("unconfigured: strips the fragment and nothing else", function () {
        checkKey("http://h/app/foo.js#frag", "http://h/app/foo.js");
        checkKey("http://h/app/foo.js?t=123&v=9#frag", "http://h/app/foo.js?t=123&v=9");
    });

    it("unconfigured: leaves every query param in the key", function () {
        checkKey("http://h/app/foo.js?t=123&v=9&import=1", "http://h/app/foo.js?t=123&v=9&import=1");
        checkKey("https://cdn.example.com/lib.js?token=abc", "https://cdn.example.com/lib.js?token=abc");
    });

    it("treats module identity as literally the URL — no path-tag collapses", function () {
        checkKey("http://h/app/m/foo.js", "http://h/app/m/foo.js");
        checkKey("http://h/app/rt", "http://h/app/rt");
    });

    it("honors a client-supplied canonicalization vocabulary via configureLoader", function () {
        var canon = getCanon();
        if (typeof canon !== "function") {
            pending("ns:module.canonicalizeHttpUrlKey not exposed (release build)");
            return;
        }
        // Neutral vocabulary: the mechanics under test are the runtime's, the
        // strings are the client's to choose.
        require("ns:module").configureLoader({
            canonicalization: {
                stripParams: ["cachebust", "rev"],
                forPathPrefixes: ["/dev/"],
                preserveQueryFor: ["/dev/metadata"],
            },
        });
        // Under a configured dev prefix, the named params drop and the rest sort.
        expect(canon("http://h/dev/core?p=x&cachebust=123&rev=9")).toBe("http://h/dev/core?p=x");
        // preserveQueryFor wins over the dev prefix: the query IS the identity.
        expect(canon("http://h/dev/metadata?c=a&cachebust=42")).toBe("http://h/dev/metadata?c=a&cachebust=42");
        // Outside every configured prefix, the query is untouched.
        expect(canon("https://cdn.example.com/lib.js?cachebust=abc")).toBe("https://cdn.example.com/lib.js?cachebust=abc");
    });
});
