// `ns:module`'s createRequire / createPumpingRequire and the `node:module`
// shim that re-exports the first of them. The two flavors differ only in how
// an ES module graph is evaluated: strict refuses top-level await (Node's
// require(esm) rule), pumping drives the loop until the graph settles.
describe("createRequire", function () {
    var nsModule = require("ns:module");
    var appRoot = __dirname.replace(/\/tests$/, "");
    var fixtureDir = appRoot + "/esm/createrequire";

    var ARGUMENT_ERROR = "The argument 'filename' must be a file URL object, " +
        "file URL string, or absolute path string.";

    // Jasmine drives its queue synchronously out of the previous spec's done(),
    // so a spec reached that way still runs inside that promise continuation.
    // A java post lands back at task level, where the pump is allowed to run.
    function onFreshTask(fn) {
        var handler = new android.os.Handler(android.os.Looper.myLooper());
        handler.post(new java.lang.Runnable({ run: fn }));
    }

    function messageOf(fn) {
        try {
            fn();
        } catch (e) {
            return String((e && e.message) || e);
        }
        return "<no error thrown>";
    }

    describe("surface", function () {
        it("ns:module exposes both require factories", function () {
            expect(typeof nsModule.createRequire).toBe("function");
            expect(typeof nsModule.createPumpingRequire).toBe("function");
        });

        it("node:module re-exports createRequire and nothing else", function () {
            var nodeModule = require("node:module");
            expect(Object.isFrozen(nodeModule)).toBe(true);
            expect(Object.keys(nodeModule)).toEqual(["createRequire"]);
            // The pumping flavor is a NativeScript extension with no Node
            // counterpart, so it stays off the node: surface.
            expect(nodeModule.createPumpingRequire).toBeUndefined();
        });

        it("node:module is a distinct module object from ns:module", function () {
            expect(require("node:module")).not.toBe(require("ns:module"));
        });

        it("exposes createRequire through a static import of node:module", function (done) {
            import("~/esm/createrequire/node-module-import.mjs").then(function (ns) {
                expect(ns.createRequireType).toBe("function");
                var target = ns.requireFrom(fixtureDir + "/anything.js", "./target.js");
                expect(target.tag).toBe("createrequire-target");
                done();
            }).catch(function (e) {
                expect("rejected: " + String((e && e.message) || e)).toBe("resolved");
                done();
            });
        });

        // Absent rather than present-but-throwing, so a feature check that
        // guards on them takes the fallback path.
        it("mints a require without resolve, cache or main", function () {
            var req = nsModule.createRequire(fixtureDir + "/anything.js");
            expect(req.resolve).toBeUndefined();
            expect(req.cache).toBeUndefined();
            expect(req.main).toBeUndefined();
        });
    });

    describe("base resolution", function () {
        it("resolves ./ against the directory of the given file", function () {
            var req = nsModule.createRequire(fixtureDir + "/anything.js");
            expect(req("./target.js").tag).toBe("createrequire-target");
        });

        it("treats a trailing slash as the directory itself", function () {
            var req = nsModule.createRequire(fixtureDir + "/");
            expect(req("./target.js").tag).toBe("createrequire-target");
        });

        it("accepts a file URL string", function () {
            var req = nsModule.createRequire("file://" + fixtureDir + "/anything.js");
            expect(req("./target.js").tag).toBe("createrequire-target");
        });

        it("accepts a URL object", function () {
            var req = nsModule.createRequire(new URL("file://" + fixtureDir + "/anything.js"));
            expect(req("./target.js").tag).toBe("createrequire-target");
        });

        it("still resolves ~ specifiers against the app root", function () {
            var req = nsModule.createRequire(fixtureDir + "/anything.js");
            expect(req("~/esm/createrequire/target.js").tag).toBe("createrequire-target");
        });
    });

    describe("argument validation", function () {
        it("rejects a non-string, non-URL argument", function () {
            expect(messageOf(function () { nsModule.createRequire(42); })).toBe(ARGUMENT_ERROR);
        });

        it("rejects a relative path string", function () {
            expect(messageOf(function () { nsModule.createRequire("./tests/index.js"); }))
                .toBe(ARGUMENT_ERROR);
        });

        it("rejects a non-file URL scheme", function () {
            expect(messageOf(function () { nsModule.createRequire("ftp://example.com/a.js"); }))
                .toBe(ARGUMENT_ERROR);
        });

        it("refuses an http base with a dev-server specific message", function () {
            expect(messageOf(function () {
                nsModule.createRequire("http://localhost:8080/main.js");
            })).toBe("createRequire() cannot take an http(s) URL (http://localhost:8080/main.js): " +
                     "require() of a dev-served module is not supported. Pass an app-root file " +
                     "path and use import() for remote modules.");
        });

        it("applies the same validation to createPumpingRequire", function () {
            expect(messageOf(function () { nsModule.createPumpingRequire(42); }))
                .toBe(ARGUMENT_ERROR);
        });
    });

    describe("evaluation policy", function () {
        it("refuses a top-level-await graph strictly", function () {
            var strictRequire = nsModule.createRequire(fixtureDir + "/anything.js");

            var refusal = messageOf(function () { strictRequire("./microtask-tla.mjs"); });
            expect(refusal).toContain("require() cannot load ES module '");
            expect(refusal).toContain("': the module graph contains top-level await. " +
                                      "Use import() or createPumpingRequire from ns:module instead.");
        });

        it("evaluates the same graph when pumping", function (done) {
            onFreshTask(function () {
                var pumpingRequire = nsModule.createPumpingRequire(fixtureDir + "/anything.js");
                var result = "";
                try {
                    result = String(pumpingRequire("./microtask-tla.mjs").value);
                } catch (e) {
                    result = "threw: " + ((e && e.message) || e);
                }
                expect(result).toBe("ok");
                done();
            });
        });

        it("refuses to pump a top-level-await graph from inside a microtask", function (done) {
            var pumpingRequire = nsModule.createPumpingRequire(fixtureDir + "/anything.js");
            Promise.resolve().then(function () {
                var refusal = messageOf(function () {
                    pumpingRequire("./microtask-tla-guarded.mjs");
                });
                expect(refusal).toContain("createPumpingRequire cannot settle module graph '");
                expect(refusal).toContain("' from inside a microtask (after an await or inside a " +
                                          "promise callback): the event loop cannot be pumped " +
                                          "re-entrantly. Call it from a task context, or use import().");
                done();
            });
        });

        it("still loads a synchronous graph from inside a microtask", function (done) {
            var pumpingRequire = nsModule.createPumpingRequire(fixtureDir + "/anything.js");
            Promise.resolve().then(function () {
                expect(pumpingRequire("./target.js").tag).toBe("createrequire-target");
                done();
            });
        });

        // The refusal is decided before evaluation, so the graph stays loadable.
        it("still imports a graph a strict require refused", function (done) {
            import("~/esm/createrequire/microtask-tla.mjs").then(function (ns) {
                expect(ns.value).toBe("ok");
                done();
            }).catch(function (e) {
                expect("rejected: " + String((e && e.message) || e)).toBe("resolved");
                done();
            });
        });

        describe("pumping options", function () {
            it("rejects a non-object options bag", function () {
                expect(function () {
                    nsModule.createPumpingRequire(fixtureDir + "/x.js", 42);
                }).toThrowError(TypeError, "createPumpingRequire: options must be an object");
            });

            it("rejects an unknown option key by name", function () {
                expect(function () {
                    nsModule.createPumpingRequire(fixtureDir + "/x.js", { deadline: 1 });
                }).toThrowError(TypeError, "createPumpingRequire: unknown option 'deadline'");
            });

            it("rejects bad option values", function () {
                var badDeadline =
                    "createPumpingRequire: 'deadlineSeconds' must be a positive finite number";
                expect(function () {
                    nsModule.createPumpingRequire(fixtureDir + "/x.js", { deadlineSeconds: 0 });
                }).toThrowError(TypeError, badDeadline);
                expect(function () {
                    nsModule.createPumpingRequire(fixtureDir + "/x.js", { deadlineSeconds: Infinity });
                }).toThrowError(TypeError, badDeadline);
                expect(function () {
                    nsModule.createPumpingRequire(fixtureDir + "/x.js", { onTimeout: "wait" });
                }).toThrowError(TypeError,
                                "createPumpingRequire: 'onTimeout' must be 'throw' or 'return-pending'");
                expect(function () {
                    nsModule.createPumpingRequire(fixtureDir + "/x.js", { pumpRunLoop: "yes" });
                }).toThrowError(TypeError,
                                "createPumpingRequire: 'pumpRunLoop' must be a boolean");
            });

            it("refuses options on the strict createRequire", function () {
                expect(function () {
                    nsModule.createRequire(fixtureDir + "/x.js", { deadlineSeconds: 1 });
                }).toThrowError(TypeError, "options are not supported on createRequire");
            });

            // These reach the deadline, so they must run from a task context —
            // from a microtask the guard would refuse before evaluating.
            it("returns without throwing at the deadline under onTimeout return-pending",
               function (done) {
                onFreshTask(function () {
                    // The graph parks on a promise nothing settles, so the
                    // deadline is always what ends the wait.
                    var req = nsModule.createPumpingRequire(fixtureDir + "/anything.js", {
                        deadlineSeconds: 0.25,
                        onTimeout: "return-pending",
                    });
                    var outcome = "<returned nothing>";
                    try {
                        var mod = req("./tla-return-pending.mjs");
                        outcome = typeof mod === "object" ? "returned a namespace"
                                                          : "returned " + typeof mod;
                    } catch (e) {
                        outcome = "threw: " + ((e && e.message) || e);
                    }
                    expect(outcome).toBe("returned a namespace");
                    done();
                });
            });

            it("honors a short deadlineSeconds with the default onTimeout throw", function (done) {
                onFreshTask(function () {
                    var req = nsModule.createPumpingRequire(fixtureDir + "/anything.js", {
                        deadlineSeconds: 0.25,
                    });
                    var started = Date.now();
                    expect(messageOf(function () { req("./tla-deadline.mjs"); }))
                        .toContain("Top-level await timed out for ES module ");
                    // The configured deadline governed, not the 60s default.
                    expect(Date.now() - started < 5000 ? "within the short deadline"
                                                       : "took too long")
                        .toBe("within the short deadline");
                    done();
                });
            });

            it("keeps the microtask guard unconditional even with pumpRunLoop", function (done) {
                var req = nsModule.createPumpingRequire(fixtureDir + "/anything.js", {
                    pumpRunLoop: true,
                });
                Promise.resolve().then(function () {
                    expect(messageOf(function () { req("./microtask-tla-guarded.mjs"); }))
                        .toContain("cannot be pumped re-entrantly");
                    done();
                });
            });
        });

        it("refuses a foreground-task top-level await through createRequire", function () {
            var req = nsModule.createRequire(fixtureDir + "/anything.js");
            expect(messageOf(function () { req("./tla-foreground-task.mjs"); }))
                .toContain("the module graph contains top-level await");
        });
    });
});

// `~` marks the app root; the separator after it is optional.
describe("app-root specifiers", function () {
    it("resolves ~/path", function () {
        expect(require("~/esm/createrequire/target.js").tag).toBe("createrequire-target");
    });

    it("resolves ~path without a separator", function () {
        expect(require("~esm/createrequire/target.js").tag).toBe("createrequire-target");
    });
});
