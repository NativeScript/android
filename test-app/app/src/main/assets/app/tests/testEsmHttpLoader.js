// The loopback fixture server byte-mirrors the routes of the iOS TestRunner's
// ModuleTestServer, so these specs pin the same loader contract on both
// platforms.
var appRoot = __dirname.replace(/\/tests$/, "");
var origin = "http://127.0.0.1:" + com.tns.tests.ModuleTestServer.ensureStarted();

describe("HTTP ESM Loader", function () {

    function formatError(e) {
        try {
            if (!e) return "(no error)";
            if (e instanceof Error) return e.message;
            if (typeof e === "string") return e;
            if (e && typeof e.message === "string") return e.message;
            return JSON.stringify(e);
        } catch (_) {
            return String(e);
        }
    }

    // This Jasmine's fail() throws, which inside a promise reaction surfaces as
    // an opaque spec timeout instead of a diff. Every rejection handler below
    // reports through a non-throwing expect for that reason.
    function reportRejection(error, done) {
        expect("rejected: " + formatError(error)).toBe("resolved");
        done();
    }

    function withTimeout(promise, ms, label) {
        return new Promise(function (resolve, reject) {
            var timer = setTimeout(function () {
                reject(new Error("Timeout after " + ms + "ms" + (label ? ": " + label : "")));
            }, ms);

            promise.then(function (value) {
                clearTimeout(timer);
                resolve(value);
            }).catch(function (err) {
                clearTimeout(timer);
                reject(err);
            });
        });
    }

    // Loopback fetches can outrun jasmine 2.0.1's 5s default on a cold
    // emulator. 2.0.1 has no beforeAll, so the pair is installed per describe.
    function useHttpTimeout() {
        var originalTimeout;
        beforeEach(function () {
            originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
            jasmine.DEFAULT_TIMEOUT_INTERVAL = 15000;
        });
        afterEach(function () {
            jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
        });
    }

    function onBackgroundThread(body) {
        new java.lang.Thread(new java.lang.Runnable({
            run: body
        })).start();
    }

    describe("URL Resolution", function () {
        it("should handle relative imports", function (done) {
            import("~/esm/relative/entry.mjs").then(function (module) {
                expect(module.viaDefault).toBe("relative-import-success");
                expect(module.viaNamed).toBe("relative-import-success");
                expect(module.readDependencyPayload()).toBe(true);
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });

        it("should surface helpful errors for unresolved bare specifiers", function (done) {
            import("bare-spec-example").then(function (mod) {
                // A placeholder module default-exports a Proxy whose get trap
                // throws; touching a property is what surfaces the diagnostic.
                var threw = false;
                try {
                    void (mod && mod.default && mod.default.__touch__);
                } catch (useErr) {
                    threw = true;
                    expect(formatError(useErr)).toContain("bare-spec-example");
                }
                expect(threw).toBe(true);
                done();
            }).catch(function (error) {
                expect(formatError(error)).toContain("bare-spec-example");
                done();
            });
        });
    });

    describe("HTTP Fetch Integration", function () {

        it("settles a local dynamic import issued from a background thread", function (done) {
            onBackgroundThread(function () {
                import("~/esm/graph/bg-solo.mjs").then(function (module) {
                    expect(module.name).toBe("bg-solo");
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });
        });

        describe("from a background thread over HTTP", function () {
            useHttpTimeout();

            it("settles an HTTP dynamic import issued from a background thread", function (done) {
                // Completion delivery must not depend on the calling thread
                // owning a looper, so nothing here schedules a timer.
                onBackgroundThread(function () {
                    import(origin + "/esm/query.mjs?v=bg").then(function (module) {
                        expect(module).toBeDefined();
                        expect(module.query).toContain("v=bg");
                        done();
                    }).catch(function (error) {
                        reportRejection(error, done);
                    });
                });
            });
        });

        it("evaluates a disk diamond graph in spec order, each module once", function (done) {
            import("~/esm/graph/diamond-entry.mjs").then(function (module) {
                expect(module.order).toEqual(["shared", "left", "right", "entry"]);
                expect(module.names).toEqual(["left", "right"]);
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });

        // A local root whose graph reaches an HTTP leaf. Discovery is
        // scheme-agnostic, so the walk compiles the whole closure up front and
        // the resolver never takes a blocking synchronous fetch.
        describe("mixed local/http graphs", function () {
            useHttpTimeout();

            function configureLeaves() {
                require("ns:module").configureLoader({
                    importMap: {
                        imports: {
                            "ns-test-leaf-a": origin + "/esm/graph-leaf.mjs?k=a",
                            "ns-test-leaf-b": origin + "/esm/graph-leaf.mjs?k=b",
                        },
                    },
                });
            }

            afterEach(function () {
                require("ns:module").configureLoader({ importMap: { imports: {} } });
            });

            it("resolves a local->local->http graph through require()", function () {
                configureLeaves();

                var req = require("ns:module").createRequire(appRoot + "/anything.js");
                var mod = req("./esm/mixed/a-entry.mjs");
                expect(mod.leaf).toBe("a");
                // Spec evaluation order, deepest first — the walk changes only
                // when modules are compiled, never when they run.
                expect(mod.order).toEqual(["leaf", "mid", "entry"]);
            });

            it("resolves a local->local->http graph through import()", function (done) {
                configureLeaves();

                import("~/esm/mixed/b-entry.mjs").then(function (mod) {
                    expect(mod.leaf).toBe("b");
                    expect(mod.order).toEqual(["leaf", "mid", "entry"]);
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });
        });

        // Each present configureLoader section replaces its state wholesale, so
        // an empty array is explicit policy — "nothing is volatile any more" —
        // not a no-op.
        describe("volatile patterns", function () {
            useHttpTimeout();

            var nsModule = require("ns:module");

            afterEach(function () {
                nsModule.configureLoader({ volatilePatterns: [] });
            });

            it("stops treating a URL as volatile once the list is emptied", function (done) {
                // The fixture pushes one entry per evaluation, so the bucket
                // length counts how many times the module actually ran.
                var url = origin + "/esm/graph-leaf.mjs?k=vol";
                function evaluations() {
                    return (globalThis.__nsMixedOrdervol || []).length;
                }

                nsModule.configureLoader({ volatilePatterns: ["k=vol"] });

                import(url).then(function () {
                    return import(url);
                }).then(function () {
                    // Volatile: the cached module is dropped, so it re-evaluates.
                    expect(evaluations()).toBe(2);

                    nsModule.configureLoader({ volatilePatterns: [] });
                    return import(url);
                }).then(function () {
                    // Cleared: the registry entry is reused, nothing re-runs.
                    expect(evaluations()).toBe(2);
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });
        });

        // The import map is process-wide, so every spec here installs its own
        // and restores the empty map afterwards.
        describe("import map", function () {
            useHttpTimeout();

            var nsModule = require("ns:module");

            function setMap(map) {
                nsModule.configureLoader({ importMap: map });
            }

            afterEach(function () {
                setMap({ imports: {} });
            });

            it("rejects an unknown top-level section by name", function () {
                expect(function () {
                    setMap({ imports: {}, integrity: {} });
                }).toThrowError(TypeError, /unsupported import-map section 'integrity'/);
            });

            it("rejects a trailing-slash key whose target does not end in '/'", function () {
                expect(function () {
                    setMap({ imports: { "pkg/": "http://example.com/pkg" } });
                }).toThrowError(TypeError, /must end with '\/'/);
            });

            it("rejects a trailing-slash key inside a scope map too", function () {
                expect(function () {
                    setMap({ scopes: { "/a/": { "pkg/": "http://example.com/pkg" } } });
                }).toThrowError(TypeError, /must end with '\/'/);
            });

            it("rejects a null or non-string target", function () {
                expect(function () {
                    setMap({ imports: { "pkg": null } });
                }).toThrowError(TypeError, /must be a string/);
                expect(function () {
                    setMap({ imports: { "pkg": 42 } });
                }).toThrowError(TypeError, /must be a string/);
            });

            it("rejects a non-object scope map", function () {
                expect(function () {
                    setMap({ scopes: { "/a/": "not-an-object" } });
                }).toThrowError(TypeError, /must be an object/);
            });

            it("prefixes every validation failure with 'configureLoader: '", function () {
                expect(function () {
                    setMap({ imports: {}, integrity: {} });
                }).toThrowError(TypeError, /^configureLoader: /);
            });

            it("keeps the previous map when an update is rejected", function (done) {
                setMap({ imports: { "ns-survivor": origin + "/esm/graph-leaf.mjs?k=surv" } });

                expect(function () {
                    nsModule.configureLoader({ importMap: "{ this is not json" });
                }).toThrowError(TypeError, /valid JSON/);

                // The rejected update changed nothing, so the module installed
                // by the previous map still resolves.
                import("~/esm/scoped/survivor.mjs").then(function (mod) {
                    expect(mod.leaf).toBe("surv");
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });

            // The vocabulary is per-isolate; a worker gets a copy taken on the
            // parent's thread as it spawns.
            it("gives a worker spawned after configureLoader the parent's map", function (done) {
                setMap({ imports: { "ns-worker-leaf": origin + "/esm/graph-leaf.mjs?k=wa" } });

                var worker = new Worker("./importMapWorker.js");
                worker.onmessage = function (msg) {
                    expect(msg.data.ok ? "resolved" : "failed: " + msg.data.error).toBe("resolved");
                    expect(msg.data.name).toBe("wa");
                    worker.terminate();
                    done();
                };
                worker.postMessage("ns-worker-leaf");
            });

            it("leaves a running worker on the map it was spawned with", function (done) {
                setMap({ imports: { "ns-worker-leaf": origin + "/esm/graph-leaf.mjs?k=wb" } });

                var worker = new Worker("./importMapWorker.js");
                worker.onmessage = function (msg) {
                    expect(msg.data.ok ? "resolved" : "failed: " + msg.data.error).toBe("resolved");
                    expect(msg.data.name).toBe("wb");
                    worker.terminate();
                    done();
                };

                // Reconfigure the parent only after the worker exists, then ask
                // it to resolve. The parent's own isolate does see the update.
                setMap({ imports: { "ns-worker-leaf": origin + "/esm/graph-leaf.mjs?k=wc" } });
                worker.postMessage("ns-worker-leaf");
            });

            it("resolves through the scope cascade for every referrer", function (done) {
                // A scope key is prefix-matched against the referrer's canonical
                // registry key, which for a disk module is a bare absolute path.
                var insideScope = appRoot + "/esm/scoped/inside/";
                var deepScope = appRoot + "/esm/scoped/inside/deep/";
                var scopes = {};
                scopes[insideScope] = { "ns-scoped-leaf": origin + "/esm/graph-leaf.mjs?k=in" };
                scopes[deepScope] = { "ns-scoped-leaf": origin + "/esm/graph-leaf.mjs?k=deep" };
                setMap({
                    imports: {
                        "ns-scoped-leaf": origin + "/esm/graph-leaf.mjs?k=top",
                        "ns-scoped-fallthrough": origin + "/esm/graph-leaf.mjs?k=fall",
                    },
                    scopes: scopes,
                });

                Promise.all([
                    import("~/esm/scoped/inside/mid.mjs"),
                    import("~/esm/scoped/inside/deep/mid.mjs"),
                    import("~/esm/scoped/outside/mid.mjs"),
                ]).then(function (mods) {
                    // A scope wins over the top-level entry for a referrer inside it.
                    expect(mods[0].leaf).toBe("in");
                    // ...and a specifier the scope does not define falls through.
                    expect(mods[0].fallthrough).toBe("fall");
                    // Two scopes match; the more specific one wins.
                    expect(mods[1].leaf).toBe("deep");
                    // No scope matches this referrer.
                    expect(mods[2].leaf).toBe("top");
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });
        });

        // An imperative API rejects bad input loudly, the way WebIDL does on the
        // web and ERR_INVALID_ARG_TYPE does in Node. Silently skipping a
        // mistyped section or a typo'd key turns a caller's bug into a config
        // that quietly does nothing.
        describe("loader surface argument validation", function () {
            useHttpTimeout();

            var nsModule = require("ns:module");

            it("rejects a missing or non-object config", function () {
                expect(function () {
                    nsModule.configureLoader();
                }).toThrowError(TypeError, /configureLoader expects a config object/);
                expect(function () {
                    nsModule.configureLoader(42);
                }).toThrowError(TypeError, /configureLoader expects a config object/);
            });

            it("rejects an unknown top-level config key by name", function () {
                expect(function () {
                    nsModule.configureLoader({ typoKey: [] });
                }).toThrowError(TypeError, /unknown option 'typoKey'/);
            });

            it("rejects a non-array volatilePatterns", function () {
                expect(function () {
                    nsModule.configureLoader({ volatilePatterns: "x" });
                }).toThrowError(TypeError, /volatilePatterns must be an array of strings/);
            });

            it("rejects a non-string volatilePatterns element by index", function () {
                expect(function () {
                    nsModule.configureLoader({ volatilePatterns: [1] });
                }).toThrowError(TypeError, /volatilePatterns\[0\] must be a string/);
                expect(function () {
                    nsModule.configureLoader({ volatilePatterns: ["ok", null] });
                }).toThrowError(TypeError, /volatilePatterns\[1\] must be a string/);
            });

            it("rejects a non-object canonicalization", function () {
                expect(function () {
                    nsModule.configureLoader({ canonicalization: "x" });
                }).toThrowError(TypeError, /canonicalization must be an object/);
            });

            it("rejects a non-array canonicalization sub-key by name", function () {
                expect(function () {
                    nsModule.configureLoader({ canonicalization: { stripParams: "t" } });
                }).toThrowError(TypeError,
                                /canonicalization\.stripParams must be an array of strings/);
                expect(function () {
                    nsModule.configureLoader({ canonicalization: { forPathPrefixes: [7] } });
                }).toThrowError(TypeError, /canonicalization\.forPathPrefixes\[0\] must be a string/);
            });

            it("rejects a non-array invalidateModules argument", function () {
                expect(function () {
                    nsModule.invalidateModules("x");
                }).toThrowError(TypeError, /invalidateModules expects an array of URL strings/);
            });

            it("rejects a non-string invalidateModules element by index", function () {
                expect(function () {
                    nsModule.invalidateModules([1]);
                }).toThrowError(TypeError, /urls\[0\] must be a string/);
            });

            it("rejects a non-string canonicalizeHttpUrlKey argument", function () {
                // Debug-only diagnostic; release builds omit the member entirely.
                if (typeof nsModule.canonicalizeHttpUrlKey !== "function") {
                    pending("canonicalizeHttpUrlKey is debug-only; absent in this build");
                    return;
                }
                expect(function () {
                    nsModule.canonicalizeHttpUrlKey(42);
                }).toThrowError(TypeError, /canonicalizeHttpUrlKey expects a URL string/);
            });

            // Validate-before-apply: the whole config is checked before any of
            // it is installed, so a call that throws leaves every section on the
            // state it already had.
            it("applies no section when any part of the config is invalid", function () {
                if (typeof nsModule.canonicalizeHttpUrlKey !== "function") {
                    pending("canonicalizeHttpUrlKey is debug-only; absent in this build");
                    return;
                }
                var url = "http://h/dev/core?p=x&t=123";
                var before = nsModule.canonicalizeHttpUrlKey(url);

                // A well-formed canonicalization section paired with a typo'd key.
                expect(function () {
                    nsModule.configureLoader({
                        canonicalization: { stripParams: ["t"], forPathPrefixes: ["/dev/"] },
                        typoKey: 1,
                    });
                }).toThrowError(TypeError, /unknown option 'typoKey'/);

                // Had the canonicalization section been applied, `t` would now
                // be stripped and the key would differ.
                expect(nsModule.canonicalizeHttpUrlKey(url)).toBe(before);
            });

            it("leaves volatilePatterns untouched when the same call throws", function (done) {
                var url = origin + "/esm/graph-leaf.mjs?k=vpre";
                function evaluations() {
                    return (globalThis.__nsMixedOrdervpre || []).length;
                }

                // Nothing is volatile yet, so a second import reuses the entry.
                import(url).then(function () {
                    return import(url);
                }).then(function () {
                    expect(evaluations()).toBe(1);

                    // A valid volatilePatterns alongside an unknown key: the
                    // call throws and the patterns must NOT be installed.
                    expect(function () {
                        nsModule.configureLoader({
                            volatilePatterns: ["k=vpre"],
                            typoKey: 1,
                        });
                    }).toThrowError(TypeError, /unknown option 'typoKey'/);

                    return import(url);
                }).then(function () {
                    // Still not volatile: the rejected call installed nothing.
                    expect(evaluations()).toBe(1);
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });
        });

        // Module scripts are strict about MIME on the web, and so is the
        // loader: the response policy lives in one classifier shared by the
        // synchronous fallback and the graph walk.
        describe("module MIME gate", function () {
            useHttpTimeout();

            function rejectionOf(url, callback) {
                import(url).then(function () {
                    callback("<resolved>");
                }).catch(function (error) {
                    callback(String((error && error.message) || error));
                });
            }

            it("rejects an SPA fallback that answers with text/html", function (done) {
                var url = origin + "/esm/html-fallback.mjs";
                rejectionOf(url, function (message) {
                    // The DX win: the cause is the MIME type, not a syntax
                    // error from HTML reaching the JS parser.
                    expect(message.indexOf("text/html") >= 0 ? "names the MIME" : message)
                        .toBe("names the MIME");
                    expect(message.indexOf(url) >= 0 ? "names the URL" : message)
                        .toBe("names the URL");
                    expect(message.indexOf("Unexpected token") >= 0 ? message : "no parse error")
                        .toBe("no parse error");
                    done();
                });
            });

            it("rejects a response that carries no MIME type", function (done) {
                var url = origin + "/esm/no-mime.mjs";
                rejectionOf(url, function (message) {
                    expect(message.indexOf("no MIME type") >= 0 ? "names the missing MIME" : message)
                        .toBe("names the missing MIME");
                    expect(message.indexOf(url) >= 0 ? "names the URL" : message)
                        .toBe("names the URL");
                    done();
                });
            });

            it("names the status for a non-2xx response", function (done) {
                var url = origin + "/esm/nonexistent-module-404.mjs";
                rejectionOf(url, function (message) {
                    expect(message).toBe("HTTP import failed: " + url + " (status=404)");
                    done();
                });
            });

            it("still serves an empty 200 with a JS MIME as the empty module", function (done) {
                // Type-only modules transform to zero runtime code; dev servers
                // serve them as empty 200s and they must stay valid.
                import(origin + "/esm/empty.mjs").then(function (mod) {
                    expect(typeof mod).toBe("object");
                    expect(Object.keys(mod)).toEqual([]);
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });

            it("routes a served JSON module through the JSON path, with stable identity",
               function (done) {
                var url = origin + "/esm/data.json";
                import(url).then(function (first) {
                    expect(first.default.kind).toBe("json-module");
                    expect(first.default.n).toBe(41);
                    return import(url).then(function (second) {
                        expect(second).toBe(first);
                        expect(second.default).toBe(first.default);
                        done();
                    });
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });

            // Re-importing from inside the first import's own resolution is
            // the case that exposed stale waiter routing: the reaction runs
            // while the first settle is still unwinding, so the loader must
            // already have cleared the state that would park this import on a
            // waiter list nothing will flush.
            it("settles a re-entrant re-import issued from the first import's handler",
               function (done) {
                var url = origin + "/esm/data.json?reentrant=1";
                var settled = "never settled";
                import(url).then(function (first) {
                    import(url).then(function (second) {
                        settled = second === first ? "same namespace" : "different namespace";
                    }, function (error) {
                        settled = "re-import rejected: " + ((error && error.message) || error);
                    });
                }, function (error) {
                    settled = "first import rejected: " + ((error && error.message) || error);
                });
                __ns__setTimeout(function () {
                    expect(settled).toBe("same namespace");
                    done();
                }, 1500);
            });
        });

        it("links and evaluates cyclic disk imports", function (done) {
            import("~/esm/graph/cycle-a.mjs").then(function (module) {
                expect(module.aValue).toBe("a");
                expect(module.roundTrip).toBe("b-saw-a");
                expect(module.describeB()).toBe("a-saw-b");
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });

        it("gives nested disk modules a correct import.meta", function (done) {
            import("~/esm/relative/meta.mjs").then(function (module) {
                expect(typeof module.metaUrl).toBe("string");
                expect(module.metaUrl.indexOf("file://")).toBe(0);
                expect(module.metaUrl).toContain("esm/relative/meta.mjs");
                expect(typeof module.metaDirname).toBe("string");
                expect(module.metaDirname).toContain("esm/relative");
                expect(module.metaDirname).not.toContain("meta.mjs");
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });

        it("returns one module identity for repeated JSON imports", function (done) {
            var spec = "~/esm/identity.json";
            Promise.all([import(spec), import(spec)]).then(function (results) {
                expect(results[0]).toBe(results[1]);
                expect(results[0].default.name).toBe("esm-identity-fixture");
                expect(results[0].default.value).toBe(42);
                return import(spec).then(function (third) {
                    expect(third).toBe(results[0]);
                    expect(third.default).toBe(results[0].default);
                    done();
                });
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });

        it("should fall back to filesystem when HTTP fetch fails", function (done) {
            import("~/esm/fs-fallback.mjs").then(function (module) {
                expect(module).toBeDefined();
                expect(module.ok || (module.default && module.default.ok)).toBe(true);
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });
    });

    describe("Module Compilation", function () {

        it("should compile filesystem-backed ES modules successfully", function (done) {
            import("~/esm/hmr/test-esm-module.mjs").then(function (module) {
                expect(module).toBeDefined();
                expect(module.testValue).toBe("http-esm-loaded");
                expect(typeof module.default).toBe("function");
                expect(module.default()).toContain("HTTP ESM loader working");
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });

        it("should reuse compiled modules across multiple dynamic imports", function (done) {
            var spec = "~/esm/hmr/test-esm-module.mjs";
            Promise.all([import(spec), import(spec)]).then(function (results) {
                expect(results[0]).toBeDefined();
                expect(results[1]).toBeDefined();
                expect(results[0].timestamp).toBe(results[1].timestamp);
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });
    });

    describe("Error Handling", function () {
        useHttpTimeout();

        it("surfaces the real compile error for a served module with a syntax error", function (done) {
            var url = origin + "/esm/syntax-error.mjs";
            withTimeout(import(url), 10000, "import " + url)
                .then(function () {
                    expect("resolved").toBe("rejected");
                    done();
                })
                .catch(function (error) {
                    // The parse error itself, not a generic "compile failed" /
                    // instantiation failure that names no cause.
                    var message = String((error && error.message) || error);
                    expect(message.indexOf("Unexpected token") >= 0 ? "names the parse error" : message)
                        .toBe("names the parse error");
                    expect(message.indexOf("syntax-error.mjs") >= 0 ? "names the module" : message)
                        .toBe("names the module");
                    done();
                });
        });

        describe("unreachable and slow endpoints", function () {
            it("rejects an unreachable host as a network error", function (done) {
                // A closed loopback port refuses instantly, so this pins the
                // network-error wording without waiting out a real timeout.
                var url = "http://127.0.0.1:59999/unreachable.mjs";
                import(url).then(function () {
                    expect("resolved").toBe("rejected");
                    done();
                }).catch(function (error) {
                    expect(String((error && error.message) || error))
                        .toBe("HTTP import failed: " + url + " (network error)");
                    done();
                });
            });

            it("waits out a slow response instead of aborting it early", function (done) {
                var url = origin + "/esm/timeout.mjs?delayMs=1500";
                import(url).then(function (mod) {
                    expect(typeof mod.evaluatedAt).toBe("number");
                    done();
                }).catch(function (error) {
                    reportRejection(error, done);
                });
            });
        });

        it("should handle malformed URLs gracefully", function () {
            // The rejection is swallowed deliberately: the contract under test
            // is only that a malformed http specifier throws nothing inline.
            expect(function () {
                import("http://").catch(function () { });
            }).not.toThrow();
        });
    });

    describe("Integration with HMR", function () {

        it("should NOT attach a native import.meta.hot (hot contexts are injected by the dev server)", function (done) {
            // The runtime owns no HMR policy: `import.meta.hot` is only present
            // when the @nativescript/vite dev server injects a JS hot context
            // into the served module source.
            import("~/esm/hmr/test-esm-module.mjs").then(function (module) {
                expect(module.getHotContext()).toBeUndefined();
                expect(module.callInvalidateSafe()).toBe(false);
                done();
            }).catch(function (error) {
                reportRejection(error, done);
            });
        });
    });

    describe("URL Key Canonicalization", function () {
        useHttpTimeout();

        it("preserves query for non-dev/public URLs", function (done) {
            var u1 = origin + "/esm/query.mjs?v=1";
            var u2 = origin + "/esm/query.mjs?v=2";

            withTimeout(import(u1), 10000, "import " + u1)
                .then(function (m1) {
                    return withTimeout(import(u2), 10000, "import " + u2).then(function (m2) {
                        expect(m1.query).toContain("v=1");
                        expect(m2.query).toContain("v=2");
                        expect(m1.query).not.toBe(m2.query);
                        done();
                    });
                })
                .catch(function (error) {
                    reportRejection(error, done);
                });
        });

        // Collapsing cache-busters onto one registry key needs a vocabulary;
        // the runtime ships none, so these specs install one. Canonicalization
        // config is process-wide, hence the restore. (Jasmine 2.0.1 has no
        // beforeAll/afterAll.)
        describe("with a dev-endpoint vocabulary configured", function () {
            beforeEach(function () {
                require("ns:module").configureLoader({
                    canonicalization: {
                        stripParams: ["t", "v", "import"],
                        forPathPrefixes: ["/ns/"],
                        preserveQueryFor: [],
                    },
                });
            });

            afterEach(function () {
                require("ns:module").configureLoader({
                    canonicalization: { stripParams: [], forPathPrefixes: [], preserveQueryFor: [] },
                });
            });

            it("drops the configured cache-busters for dev endpoints", function (done) {
                var u1 = origin + "/ns/m/query.mjs?v=1";
                var u2 = origin + "/ns/m/query.mjs?v=2";

                withTimeout(import(u1), 10000, "import " + u1)
                    .then(function (m1) {
                        return withTimeout(import(u2), 10000, "import " + u2).then(function (m2) {
                            // Both URLs map to one cache key, so the second
                            // import reuses the first evaluated module.
                            expect(m2.evaluatedAt).toBe(m1.evaluatedAt);
                            expect(m2.query).toBe(m1.query);
                            done();
                        });
                    })
                    .catch(function (error) {
                        reportRejection(error, done);
                    });
            });

            it("sorts query params for dev endpoints", function (done) {
                var u1 = origin + "/ns/m/query.mjs?b=2&a=1";
                var u2 = origin + "/ns/m/query.mjs?a=1&b=2";

                withTimeout(import(u1), 10000, "import " + u1)
                    .then(function (m1) {
                        return withTimeout(import(u2), 10000, "import " + u2).then(function (m2) {
                            expect(m2.evaluatedAt).toBe(m1.evaluatedAt);
                            expect(m2.query).toBe(m1.query);
                            done();
                        });
                    })
                    .catch(function (error) {
                        reportRejection(error, done);
                    });
            });
        });

        it("ignores URL fragments for cache identity", function (done) {
            var u1 = origin + "/esm/query.mjs#one";
            var u2 = origin + "/esm/query.mjs#two";

            withTimeout(import(u1), 10000, "import " + u1)
                .then(function (m1) {
                    return withTimeout(import(u2), 10000, "import " + u2).then(function (m2) {
                        expect(m2.evaluatedAt).toBe(m1.evaluatedAt);
                        done();
                    });
                })
                .catch(function (error) {
                    reportRejection(error, done);
                });
        });
    });
});

// A bare `@` is not a specifier the runtime knows: it resolves through the
// normal path and fails, naming itself, instead of being swallowed into a
// fabricated empty module.
describe("invalid module specifiers", function () {
    it("rejects a dynamic import of '@' with an error naming the specifier", function (done) {
        import("@").then(function () {
            expect("resolved").toBe("rejected");
            done();
        }).catch(function (e) {
            var message = String((e && e.message) || e);
            expect(message.indexOf("Cannot find module '@'") >= 0 ? "names the specifier" : message)
                .toBe("names the specifier");
            expect(message.indexOf("tried " + appRoot + "/@") >= 0 ? "names the path tried" : message)
                .toBe("names the path tried");
            done();
        });
    });
});
