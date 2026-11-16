// Runs in both modes; the suite is run once as built and once with
// -PloadCodeFromApk (see tools/appfiles-test-mode.init.gradle).
describe("App files", function () {
    var File = java.io.File;
    var loadsCodeFromApk = com.tns.AppFiles.loadsCodeFromApk();
    var filesRoot = __approot;
    var appRoot = filesRoot + "/app";

    function otherSpelling(path) {
        if (path.indexOf("/data/data/") === 0) {
            return "/data/user/0/" + path.substring("/data/data/".length);
        }
        if (path.indexOf("/data/user/0/") === 0) {
            return "/data/data/" + path.substring("/data/user/0/".length);
        }
        return path;
    }

    function loadInWorker(path, callback) {
        var worker = new Worker("./codeCacheLoadWorker.js");
        worker.onmessage = function (msg) {
            worker.terminate();
            callback(msg.data);
        };
        worker.postMessage({ path: path, isModule: false });
    }

    describe("under either spelling of the files directory", function () {
        var aliasRoot = otherSpelling(filesRoot);

        it("has two spellings to test on this device", function () {
            expect(aliasRoot).not.toBe(filesRoot);
        });

        it("finds runtime files", function () {
            expect(com.tns.AppFiles.isFile(aliasRoot + "/internal/livesync.js")).toBe(true);
            expect(com.tns.AppFiles.isFile(aliasRoot + "/app/tests/appFileLayers/target.js")).toBe(true);
        });

        it("runs a script named through getFilesDir()", function () {
            var context = com.tns.NativeScriptApplication.getInstance();
            var previous = global.__onLiveSync;
            var calls = 0;
            global.__onLiveSync = function () {
                calls++;
            };
            try {
                com.tns.Runtime.getCurrentRuntime().runScript(new File(context.getFilesDir(), "internal/livesync.js"));
                com.tns.Runtime.getCurrentRuntime().runScript(new File(aliasRoot, "internal/livesync.js"));
            } finally {
                global.__onLiveSync = previous;
            }
            expect(calls).toBe(2);
        });

        it("requires one CommonJS module", function () {
            var viaAlias = require(aliasRoot + "/app/tests/appFileLayers/target.js");
            var viaRoot = require(appRoot + "/tests/appFileLayers/target.js");
            expect(viaAlias).toBe(viaRoot);
            expect(viaAlias.filename).toBe(appRoot + "/tests/appFileLayers/target.js");
        });

        it("imports one ES module", function (done) {
            Promise.all([
                import(aliasRoot + "/app/tests/loadCodeFromApk/aliasTarget.mjs"),
                import(appRoot + "/tests/loadCodeFromApk/aliasTarget.mjs")
            ]).then(function (namespaces) {
                expect(namespaces[0].value).toBe("alias");
                expect(namespaces[0]).toBe(namespaces[1]);
                done();
            }, function (e) {
                expect(String(e)).toBe("");
                done();
            });
        });
    });

    it("extracts what android.alwaysExtract names", function () {
        expect(new File(appRoot + "/tests/alwaysExtracted/kept.js").isFile()).toBe(true);
        expect(require("./alwaysExtracted/kept.js").value).toBe("kept");
    });

    if (!loadsCodeFromApk) {
        it("extracts code when android.loadCodeFromApk is off", function () {
            expect(new File(appRoot + "/mainpage.js").isFile()).toBe(true);
            expect(new File(filesRoot + "/internal/ts_helpers.js").isFile()).toBe(true);
            expect(new File(filesRoot + "/metadata/treeNodeStream.dat").isFile()).toBe(true);
        });
        return;
    }

    describe("with android.loadCodeFromApk", function () {
        function countFiles(dir, extensions) {
            var count = 0;
            var children = dir.listFiles();
            for (var i = 0; children && i < children.length; i++) {
                var child = children[i];
                if (child.isDirectory()) {
                    count += countFiles(child, extensions);
                } else if (extensions.test(child.getName())) {
                    count++;
                }
            }
            return count;
        }

        it("serves code from the APK without extracting it", function () {
            expect(countFiles(new File(appRoot + "/tests"), /\.(js|mjs|cjs|map)$/)).toBe(
                // the one alwaysExtract match
                1);
            expect(new File(appRoot + "/mainpage.js").exists()).toBe(false);
            expect(com.tns.AppFiles.isFile(appRoot + "/mainpage.js")).toBe(true);
            expect(require("./appFileLayers/target.js").value).toBe("target");
        });

        it("still extracts JSON, images and other files", function () {
            expect(new File(appRoot + "/package.json").isFile()).toBe(true);
            expect(new File(appRoot + "/tests/image.jpg").isFile()).toBe(true);
        });

        it("serves internal files and metadata from the APK, removing extracted copies", function () {
            expect(new File(filesRoot + "/internal").exists()).toBe(false);
            expect(new File(filesRoot + "/metadata").exists()).toBe(false);
            expect(com.tns.AppFiles.isFile(filesRoot + "/internal/ts_helpers.js")).toBe(true);
            expect(com.tns.AppFiles.isFile(filesRoot + "/metadata/treeNodeStream.dat")).toBe(true);
        });

        it("loads APK-served code in a worker", function (done) {
            loadInWorker(appRoot + "/tests/loadCodeFromApk/liveSyncOverride.js", function (result) {
                expect(result.error).toBeUndefined();
                expect(result.value).toBe("apk");
                done();
            });
        });

        it("lets a file LiveSync writes into files/app win over the APK copy", function (done) {
            var target = new File(appRoot + "/tests/loadCodeFromApk/liveSyncOverride.js");
            var runtime = com.tns.Runtime.getCurrentRuntime();
            target.getParentFile().mkdirs();
            runtime.lock();
            try {
                var out = new java.io.FileOutputStream(target);
                out.write(new java.lang.String("module.exports.value = 'disk';\n").getBytes("UTF-8"));
                out.close();
            } finally {
                runtime.unlock();
            }
            loadInWorker(target.getAbsolutePath(), function (overridden) {
                target.delete();
                loadInWorker(target.getAbsolutePath(), function (restored) {
                    expect(overridden.value).toBe("disk");
                    expect(restored.value).toBe("apk");
                    done();
                });
            });
        });
    });
});
