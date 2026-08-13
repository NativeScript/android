describe("ns:runtime", function () {
    var runtime = require("ns:runtime");

    it("exposes frozen exports", function () {
        expect(Object.isFrozen(runtime)).toBe(true);
        expect(typeof runtime.setConfig).toBe("function");
        expect(typeof runtime.getConfig).toBe("function");
    });

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

    it("defaults logScriptLoading and httpFetchUrlLog from app config", function () {
        expect(runtime.getConfig("logScriptLoading")).toBe(false);
        expect(runtime.getConfig("httpFetchUrlLog")).toBe(false);
    });

    it("round-trips logScriptLoading and httpFetchUrlLog", function () {
        runtime.setConfig("logScriptLoading", true);
        expect(runtime.getConfig("logScriptLoading")).toBe(true);
        runtime.setConfig("logScriptLoading", false);
        expect(runtime.getConfig("logScriptLoading")).toBe(false);

        runtime.setConfig("httpFetchUrlLog", true);
        expect(runtime.getConfig("httpFetchUrlLog")).toBe(true);
        runtime.setConfig("httpFetchUrlLog", false);
        expect(runtime.getConfig("httpFetchUrlLog")).toBe(false);
    });

    it("rejects non-boolean log flag values and keeps the current one", function () {
        expect(function () {
            runtime.setConfig("logScriptLoading", "yes");
        }).toThrowError(TypeError, /must be a boolean/);
        expect(runtime.getConfig("logScriptLoading")).toBe(false);
        expect(function () {
            runtime.setConfig("httpFetchUrlLog", 1);
        }).toThrowError(TypeError, /must be a boolean/);
        expect(runtime.getConfig("httpFetchUrlLog")).toBe(false);
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

    it("does not expose releasedObjectPolicy (iOS-only)", function () {
        expect(function () {
            runtime.getConfig("releasedObjectPolicy");
        }).toThrowError(TypeError, /Unknown runtime config key/);
    });
});
