// Layers come from com.tns.tests.TestAppFileLayersCustomizer, registered in the
// test app's manifest.
describe("App file layers", function () {
    var dir = __dirname + "/appFileLayers/";

    it("serves a map entry from an APK asset under the requested path", function () {
        var remapped = require("./appFileLayers/remapped.js");
        expect(remapped.value).toBe("target");
        expect(remapped.filename).toBe(dir + "remapped.js");
        expect(com.tns.AppFiles.isFile(dir + "remapped.js")).toBe(true);
    });

    it("serves files from a customizer's directory layer", function () {
        expect(require("./appFileLayers/fromDirectory.js").value).toBe("directory");
    });

    it("serves ES modules from a customizer's directory layer", function (done) {
        import(dir + "fromDirectory.mjs").then(function (ns) {
            expect(ns.value).toBe("directory-esm");
            done();
        }, function (e) {
            expect(String(e)).toBe("");
            done();
        });
    });

    it("serves files from a customizer's APK asset layer", function () {
        expect(require("./appFileLayers/fromApkBase.js").value).toBe("apk-assets");
    });

    it("hides a whited-out path that a lower layer has", function () {
        expect(com.tns.AppFiles.isFile(dir + "whitedOut.js")).toBe(false);
        expect(function () {
            require("./appFileLayers/whitedOut.js");
        }).toThrow();
    });

    it("never layers the runtime's internal files", function () {
        expect(com.tns.AppFiles.isFile(__approot + "/internal/ts_helpers.js")).toBe(true);
        expect(com.tns.AppFiles.isFile(__approot + "/internal/tests/appFileLayers/remapped.js")).toBe(false);
    });
});
