describe("ns:module", function () {
    it("should expose the dev-loader primitives via the ns:module builtin", function () {
        var nsModule = require("ns:module");
        expect(Object.isFrozen(nsModule)).toBe(true);
        expect(typeof nsModule.configureLoader).toBe("function");
        expect(typeof nsModule.invalidateModules).toBe("function");
        expect(typeof nsModule.getLoadedModuleUrls).toBe("function");
        expect(typeof nsModule.setDevBootComplete).toBe("function");
        expect(nsModule.terminateAllWorkers).toBeUndefined();
        expect(global.__NS_DEV__).toBeUndefined();
    });

    it("exposes exactly the declared surface", function () {
        var nsModule = require("ns:module");
        var expected = ["configureLoader", "getLoadedModuleUrls", "invalidateModules", "setDevBootComplete"];
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

    it("setDevBootComplete flips the JS-visible boot-complete global", function () {
        var nsModule = require("ns:module");
        nsModule.setDevBootComplete(true);
        expect(global.__NS_HMR_BOOT_COMPLETE__).toBe(true);
        nsModule.setDevBootComplete(false);
        expect(global.__NS_HMR_BOOT_COMPLETE__).toBe(false);
        nsModule.setDevBootComplete();
        expect(global.__NS_HMR_BOOT_COMPLETE__).toBe(true);
        nsModule.setDevBootComplete(false);
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

    it("drops dev cache-busters (t/v/import) but keeps real query params", function () {
        checkKey("http://h/ns/core?p=x&t=123&v=9&import=1", "http://h/ns/core?p=x");
    });

    it("leaves public (non-dev, non-volatile) URLs untouched", function () {
        checkKey("https://cdn.example.com/lib.js?token=abc", "https://cdn.example.com/lib.js?token=abc");
    });

    it("treats module identity as literally the URL — no path-tag collapses", function () {
        checkKey("http://h/ns/m/foo.js", "http://h/ns/m/foo.js");
        checkKey("http://h/ns/rt", "http://h/ns/rt");
        checkKey("http://h/ns/core", "http://h/ns/core");
    });

    it("ignores URL fragments for dev endpoints", function () {
        checkKey("http://h/ns/m/foo.js#frag", "http://h/ns/m/foo.js");
    });

    it("honors a client-supplied canonicalization vocabulary via configureLoader", function () {
        var canon = getCanon();
        if (typeof canon !== "function") {
            pending("ns:module.canonicalizeHttpUrlKey not exposed (release build)");
            return;
        }
        require("ns:module").configureLoader({
            canonicalization: {
                stripParams: ["t", "v", "import"],
                forPathPrefixes: ["/ns/", "/node_modules/.vite/", "/@id/", "/@fs/"],
                preserveQueryFor: ["/@ng/component"],
            },
        });
        expect(canon("http://h/ns/core?p=x&t=123&v=9&import=1")).toBe("http://h/ns/core?p=x");
        expect(canon("http://h/ns/m/comp/@ng/component?c=a&t=42")).toBe("http://h/ns/m/comp/@ng/component?c=a&t=42");
        expect(canon("https://cdn.example.com/lib.js?token=abc")).toBe("https://cdn.example.com/lib.js?token=abc");
    });
});
