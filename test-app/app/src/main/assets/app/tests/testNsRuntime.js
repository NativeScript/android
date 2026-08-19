describe("ns:runtime", function () {
    var runtime = require("ns:runtime");

    it("exposes frozen exports", function () {
        expect(Object.isFrozen(runtime)).toBe(true);
        expect(typeof runtime.setConfig).toBe("function");
        expect(typeof runtime.getConfig).toBe("function");
    });

    // The export set is public API, declared alongside docs/ns-builtin-modules.md
    // — all of them change together.
    it("exposes exactly the declared surface", function () {
        expect(Object.keys(runtime).sort()).toEqual(["getConfig", "setConfig"]);
    });

    it("rejects unknown keys", function () {
        expect(function () {
            runtime.setConfig("noSuchKey", 1);
        }).toThrowError(TypeError, /Unknown runtime config key/);
        expect(function () {
            runtime.getConfig("noSuchKey");
        }).toThrowError(TypeError, /Unknown runtime config key/);
    });

    it("is a singleton across require calls", function () {
        expect(require("ns:runtime")).toBe(runtime);
    });

    describe("debug categories", function () {
        afterEach(function () {
            runtime.setConfig("debug", "");
        });

        it("starts disabled", function () {
            expect(runtime.getConfig("debug")).toBe("");
        });

        it("round-trips a category list canonically", function () {
            runtime.setConfig("debug", "esm,fetch");
            expect(runtime.getConfig("debug")).toBe("esm,fetch");
        });

        it("canonicalizes order and whitespace", function () {
            runtime.setConfig("debug", " fetch , esm ");
            expect(runtime.getConfig("debug")).toBe("esm,fetch");
        });

        it("replaces the whole set rather than adding to it", function () {
            runtime.setConfig("debug", "esm,fetch");
            runtime.setConfig("debug", "registry");
            expect(runtime.getConfig("debug")).toBe("registry");
        });

        it("ignores unknown categories but keeps the known ones", function () {
            runtime.setConfig("debug", "esm,nosuchcategory");
            expect(runtime.getConfig("debug")).toBe("esm");
        });

        it("accepts every declared category", function () {
            runtime.setConfig("debug", "esm,fetch,registry");
            expect(runtime.getConfig("debug")).toBe("esm,fetch,registry");
        });

        it("disables everything on an empty string", function () {
            runtime.setConfig("debug", "esm,fetch,registry");
            runtime.setConfig("debug", "");
            expect(runtime.getConfig("debug")).toBe("");
        });

        it("rejects a non-string value and keeps the current set", function () {
            runtime.setConfig("debug", "esm");
            expect(function () {
                runtime.setConfig("debug", true);
            }).toThrowError(TypeError, /comma-separated category string/);
            expect(runtime.getConfig("debug")).toBe("esm");
        });
    });

    it("no longer registers the removed log flags", function () {
        ["logScriptLoading", "httpFetchUrlLog"].forEach(function (key) {
            expect(function () {
                runtime.getConfig(key);
            }).toThrowError(TypeError, /Unknown runtime config key/);
        });
    });

    it("does not expose remote-module security through getConfig or setConfig", function () {
        ["security", "allowRemoteModules", "remoteModuleAllowlist"].forEach(function (key) {
            expect(function () {
                runtime.getConfig(key);
            }).toThrowError(TypeError, /Unknown runtime config key/);
            expect(function () {
                runtime.setConfig(key, true);
            }).toThrowError(TypeError, /Unknown runtime config key/);
        });
    });

    // releasedObjectPolicy is an iOS-only key; the GC teardown policy it names
    // has no Android counterpart.
    it("does not expose releasedObjectPolicy", function () {
        expect(function () {
            runtime.getConfig("releasedObjectPolicy");
        }).toThrowError(TypeError, /Unknown runtime config key/);
    });
});
