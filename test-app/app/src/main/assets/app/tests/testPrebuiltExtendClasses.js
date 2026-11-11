describe("Prebuilt classes of anonymous extends", function () {

    // DexFactory writes the classes it generates at runtime to one of these directories
    // (see RuntimeHelper). A class the static binding generator built must be loaded from
    // the APK and leave nothing here.
    var filesDir = __dirname.replace(/\/app\/tests$/, "");
    var dataDir = filesDir.replace(/\/files$/, "");
    var dexDirs = [dataDir + "/code_cache/secondary-dexes", filesDir + "/secondary-dexes"];

    function generatedDexFiles(instance) {
        var proxyName = instance.getClass().getName().replace(/^com\.tns\.gen\./, "");
        var result = [];
        dexDirs.forEach(function (dir) {
            var files = new java.io.File(dir).list();
            if (!files) {
                return;
            }
            for (var i = 0; i < files.length; i++) {
                var file = files[i];
                if (file.indexOf(proxyName + "-") === 0 && /\.dex$/.test(file)) {
                    result.push(file);
                }
            }
        });
        return result;
    }

    it("loads an anonymous extend's class from the APK", function () {
        var Anonymous = java.lang.Object.extend({
            toString: function () {
                return "anonymous";
            }
        });
        var instance = new Anonymous();

        expect(instance.toString()).toBe("anonymous");
        expect(generatedDexFiles(instance)).toEqual([]);
    });

    it("loads a TypeScript-compiled native class from the APK", function () {
        var CompiledClass = (function (_super) {
            __extends(CompiledClass, _super);
            function CompiledClass() {
                var _this = _super.call(this) || this;
                return global.__native(_this);
            }
            CompiledClass.prototype.toString = function () {
                return "compiled";
            };
            return CompiledClass;
        }(java.lang.Object));
        var instance = new CompiledClass();

        expect(instance.toString()).toBe("compiled");
        expect(generatedDexFiles(instance)).toEqual([]);
    });

    it("loads the class of an extend on the first line of an ES module from the APK", function () {
        var LineOneObject = require("./prebuiltExtendLineOne.mjs").LineOneObject;
        var instance = new LineOneObject();

        expect(instance.toString()).toBe("line one");
        expect(generatedDexFiles(instance)).toEqual([]);
    });

    it("caches a class generated at runtime under the name DexFactory looks up", function () {
        // A computed member hides the extend from the static binding generator.
        var ext = "ex" + "tend";
        var RuntimeGenerated = java.lang.Object[ext]({
            toString: function () {
                return "runtime generated";
            }
        });
        var instance = new RuntimeGenerated();

        expect(instance.toString()).toBe("runtime generated");
        var files = generatedDexFiles(instance);
        expect(files.length).toBe(1);
        // <name>-<update time>-<version code>-<content digest>.dex, as DexFactory.getDexFile probes it.
        expect(files[0]).toMatch(/-\d+-\d+-[0-9a-f]{10}\.dex$/);
    });
});
