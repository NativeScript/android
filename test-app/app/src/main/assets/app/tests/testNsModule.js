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
            // fail() throws in this Jasmine, which inside a promise chain
            // surfaces as an opaque timeout instead of the real reason.
            expect("rejected: " + String((error && error.message) || error)).toBe("resolved");
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

    // Unconfigured, the runtime knows no client vocabulary: it strips the
    // fragment and nothing else. Which params are cache-busters and which
    // paths are dev endpoints arrives through configureLoader.
    describe("unconfigured (mechanical only)", function () {
        it("keeps every query param, cache-buster-looking or not", function () {
            checkKey("http://h/app/core?p=x&t=123&v=9&import=1",
                     "http://h/app/core?p=x&t=123&v=9&import=1");
        });

        it("leaves public URLs untouched", function () {
            checkKey("https://cdn.example.com/lib.js?token=abc",
                     "https://cdn.example.com/lib.js?token=abc");
        });

        it("treats module identity as literally the URL — no path-tag collapses", function () {
            checkKey("http://h/app/m/foo.js", "http://h/app/m/foo.js");
            checkKey("http://h/app/rt", "http://h/app/rt");
        });

        it("still drops the fragment", function () {
            checkKey("http://h/app/m/foo.js#frag", "http://h/app/m/foo.js");
            checkKey("https://cdn.example.com/lib.js?token=abc#frag",
                     "https://cdn.example.com/lib.js?token=abc");
        });
    });

    // The canonicalization vocabulary is per-isolate loader state, so each spec
    // installs it and restores the unconfigured shape afterwards. (Jasmine
    // 2.0.1 has no beforeAll/afterAll.)
    describe("with a client-supplied vocabulary", function () {
        beforeEach(function () {
            if (typeof getCanon() !== "function") {
                return;
            }
            require("ns:module").configureLoader({
                canonicalization: {
                    stripParams: ["t", "v", "import"],
                    forPathPrefixes: ["/dev/"],
                    preserveQueryFor: ["/dev/metadata"],
                },
            });
        });

        afterEach(function () {
            if (typeof getCanon() !== "function") {
                return;
            }
            require("ns:module").configureLoader({
                canonicalization: { stripParams: [], forPathPrefixes: [], preserveQueryFor: [] },
            });
        });

        it("strips the configured cache-busters under a configured prefix", function () {
            checkKey("http://h/dev/core?p=x&t=123&v=9&import=1", "http://h/dev/core?p=x");
        });

        it("lets preserveQueryFor win under a configured prefix", function () {
            checkKey("http://h/dev/metadata?c=a&t=42", "http://h/dev/metadata?c=a&t=42");
        });

        it("leaves paths outside the configured prefixes alone", function () {
            checkKey("http://h/app/core?p=x&t=123", "http://h/app/core?p=x&t=123");
            checkKey("https://cdn.example.com/lib.js?token=abc",
                     "https://cdn.example.com/lib.js?token=abc");
        });
    });
});
