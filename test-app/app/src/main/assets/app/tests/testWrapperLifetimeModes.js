describe("Wrapper lifetime modes", function () {
    var runtime = require("ns:runtime");
    var mode = runtime.getConfig("wrapperLifetimeMode");
    var scope = runtime.getConfig("wrapperLifetimeScope");
    var canary = mode === "canary";
    var Holder = com.tns.tests.WrapperHolder;

    function stats() {
        return runtime.getWrapperLifetimeStats();
    }

    function collectJava() {
        for (var i = 0; i < 3; i++) {
            java.lang.Runtime.getRuntime().gc();
            java.lang.System.runFinalization();
        }
    }

    // A JS->Java call: settles the wrappers the last collection queued.
    function crossIntoJava() {
        java.lang.System.nanoTime();
    }

    // Lets JS drop every wrapper nothing references, and Java collect every object nothing
    // but a weak entry holds.
    function collectBoth() {
        gc();
        crossIntoJava();
        collectJava();
        gc();
        crossIntoJava();
    }

    // Collects until predicate() holds; the watcher reports Java's collections from its own
    // thread, so their effect lands on a later event loop turn.
    function eventually(predicate, done, message) {
        var deadline = Date.now() + 10000;
        (function poll() {
            collectBoth();
            collectJava();
            if (predicate()) {
                done();
                return;
            }
            if (Date.now() > deadline) {
                done.fail(message);
                return;
            }
            setTimeout(poll, 50);
        })();
    }

    afterEach(function () {
        Holder.held = null;
    });

    it("runs in the configured mode (" + mode + "/" + scope + ")", function () {
        expect(["resurrecting", "canary"]).toContain(mode);
    });

    it("keeps own-property semantics for JS properties on a Java object", function () {
        var runnable = new java.lang.Runnable({ run: function () {} });
        runnable.a = 1;
        Object.defineProperty(runnable, "hidden", { value: 2, enumerable: false, writable: true, configurable: true });
        expect(Object.keys(runnable)).toEqual(["a"]);
        expect("a" in runnable).toBe(true);
        expect(Object.prototype.hasOwnProperty.call(runnable, "hidden")).toBe(true);
        expect(Object.getOwnPropertyDescriptor(runnable, "a")).toEqual({ value: 1, writable: true, enumerable: true, configurable: true });
        expect(delete runnable.a).toBe(true);
        expect("a" in runnable).toBe(false);
        runnable.a = 3;
        expect(runnable.a).toBe(3);
    });

    it("keeps the JS state of a Java-held extended object and the Java objects it references", function () {
        var rebuilds = stats().canaryRebuilds;
        (function () {
            var runnable = new java.lang.Runnable({
                run: function () {
                    this.ran = (this.ran || 0) + 1;
                }
            });
            runnable.tag = "kept";
            runnable.child = new java.lang.StringBuilder("child");
            Holder.held = runnable;
        })();
        collectBoth();
        collectJava();

        var runnable = Holder.held;
        expect(runnable.tag).toBe("kept");
        if (canary) {
            expect(runnable.child.toString()).toBe("child");
        } else {
            // Weakened with the holder's wrapper, and collected by Java under it.
            expect(function () {
                runnable.child.toString();
            }).toThrowError(/no longer has available Java instance counterpart/);
        }
        runnable.run();
        expect(runnable.ran).toBe(1);
        if (canary) {
            expect(stats().canaryRebuilds).toBeGreaterThan(rebuilds);
        }
    });

    it("lets Java call into a JS-implemented object whose wrapper was collected", function () {
        var task = (function () {
            var callable = new java.util.concurrent.Callable({
                call: function () {
                    return "answer:" + this.answer;
                }
            });
            callable.answer = 42;
            return new java.util.concurrent.FutureTask(callable);
        })();
        collectBoth();
        collectJava();

        task.run();
        expect(task.get()).toBe("answer:42");
    });

    it("hands an extend() instance's state to Java calls after its wrapper was collected", function () {
        var Described = java.lang.Object.extend({
            toString: function () {
                return "described:" + this.label;
            }
        });
        (function () {
            var described = new Described();
            described.label = "first";
            Holder.held = described;
        })();
        collectBoth();
        collectJava();

        expect(java.lang.String.valueOf(Holder.held)).toBe("described:first");
        Holder.held.label = "second";
        collectBoth();
        expect(java.lang.String.valueOf(Holder.held)).toBe("described:second");
    });

    it("keeps the state of an extended object Java constructed", function () {
        var Made = java.lang.Object.extend({
            toString: function () {
                return "made:" + this.value;
            }
        });
        (function () {
            var made = new Made().getClass().newInstance();
            made.value = 7;
            Holder.held = made;
        })();
        collectBoth();
        collectJava();

        expect(java.lang.String.valueOf(Holder.held)).toBe("made:7");
    });

    it("keeps the state a TypeScript-style subclass sets in its constructor", function () {
        var Greeter = (function (_super) {
            __extends(Greeter, _super);
            function Greeter(name) {
                var _this = _super.call(this) || this;
                _this.name = name;
                return __native(_this);
            }
            Greeter.prototype.toString = function () {
                return "hello " + this.name;
            };
            return Greeter;
        })(java.lang.Object);
        (function () {
            Holder.held = new Greeter("ts");
        })();
        collectBoth();
        collectJava();

        expect(java.lang.String.valueOf(Holder.held)).toBe("hello ts");
    });

    // Under "resurrecting" the child's Java object is weakened with the holder's wrapper
    // and Java collects it (testWrapperLifetime.js); a canary's state keeps it.
    it("keeps the Java objects a Java-held canary's JS properties reference", function () {
        if (!canary) {
            return;
        }
        (function () {
            var runnable = new java.lang.Runnable({ run: function () {} });
            runnable.child = new java.lang.StringBuilder("child");
            Holder.held = runnable;
        })();
        gc();
        crossIntoJava();
        collectJava();

        expect(Holder.held.child.toString()).toBe("child");
    });

    // Under scope "all" a plain wrapper is a canary too: its state is rebuilt onto a
    // wrapper made the usual way, and it keeps what it references (#2126's over-release).
    it("keeps the state of a Java-held plain object and the Java objects it references, under scope all", function () {
        if (!canary || scope !== "all") {
            return;
        }
        var states = stats().canaryStates;
        (function () {
            var holder = new java.lang.StringBuilder("holder");
            holder.tag = "plain";
            holder.child = new java.lang.StringBuilder("child");
            Holder.held = holder;
        })();
        expect(stats().canaryStates).toBe(states + 1);
        collectBoth();
        collectJava();

        var holder = Holder.held;
        expect(holder.toString()).toBe("holder");
        expect(holder.tag).toBe("plain");
        expect(holder.child.toString()).toBe("child");
    });

    it("hands back a property that shadows a prototype member on a new wrapper", function () {
        (function () {
            var runnable = new java.lang.Runnable({
                run: function () {
                    return "prototype";
                }
            });
            runnable.run = function () {
                return "shadow";
            };
            Holder.held = runnable;
        })();
        collectBoth();

        expect(Holder.held.run()).toBe("shadow");
        delete Holder.held.run;
        expect(Holder.held.run()).toBe("prototype");
    });

    it("hands back non-enumerable, read-only, accessor and symbol-keyed properties with their attributes", function () {
        var symbol = Symbol("key");
        (function () {
            var runnable = new java.lang.Runnable({ run: function () {} });
            Object.defineProperty(runnable, "fixed", { value: 1, writable: false, enumerable: true, configurable: false });
            Object.defineProperty(runnable, "hidden", { value: 2, writable: true, enumerable: false, configurable: true });
            Object.defineProperty(runnable, "doubled", {
                get: function () {
                    return this.fixed * 2;
                },
                enumerable: true,
                configurable: true
            });
            runnable[symbol] = "symbol";
            Holder.held = runnable;
        })();
        collectBoth();

        var runnable = Holder.held;
        expect(Object.getOwnPropertyDescriptor(runnable, "fixed")).toEqual({ value: 1, writable: false, enumerable: true, configurable: false });
        expect(Object.getOwnPropertyDescriptor(runnable, "hidden")).toEqual({ value: 2, writable: true, enumerable: false, configurable: true });
        expect(runnable.doubled).toBe(2);
        expect(runnable[symbol]).toBe("symbol");
    });

    it("writes Java fields through to Java without making them JS state", function () {
        var states = stats().canaryStates;
        var point = new android.graphics.Point(1, 2);
        point.x = 5;
        expect(point.x).toBe(5);
        expect(Object.keys(point)).not.toContain("x");
        expect(stats().canaryStates).toBe(states);
    });

    it("refuses to freeze a canary", function () {
        if (!canary) {
            return;
        }
        var runnable = new java.lang.Runnable({ run: function () {} });
        expect(function () {
            Object.freeze(runnable);
        }).toThrow();
    });

    it("leaves plain wrappers outside the extended scope without forwarding", function () {
        if (!canary || scope !== "extended") {
            return;
        }
        var writes = stats().canaryInterceptedWrites;
        var plain = new java.lang.StringBuilder();
        plain.tag = 1;
        expect(stats().canaryInterceptedWrites).toBe(writes);
        var runnable = new java.lang.Runnable({ run: function () {} });
        runnable.tag = 1;
        expect(stats().canaryInterceptedWrites).toBe(writes + 1);
    });

    // Under "extended" a plain wrapper is a canary without state: nothing resurrects it,
    // and Java stops holding its object for JS once it dies.
    it("lets plain wrappers die without resurrecting them, under scope extended", function () {
        if (!canary || scope !== "extended") {
            return;
        }
        var list = new java.util.ArrayList();
        collectBoth();
        var before = stats();
        (function () {
            for (var i = 0; i < 20; i++) {
                list.add(new java.lang.StringBuilder("plain" + i));
            }
        })();
        gc();
        crossIntoJava();
        var after = stats();
        expect(after.resurrections).toBe(before.resurrections);
        expect(after.collected - before.collected).not.toBeLessThan(20);
        expect(after.canaryStates).toBe(before.canaryStates);
        expect(list.get(0).toString()).toBe("plain0");
    });

    it("builds a new wrapper without the old one's JS properties for a plain object Java hands back, under scope extended", function () {
        (function () {
            var holder = new java.lang.StringBuilder("holder");
            holder.tag = "plain";
            Holder.held = holder;
        })();
        collectBoth();

        var holder = Holder.held;
        expect(holder.toString()).toBe("holder");
        if (canary && scope === "extended") {
            expect(holder.tag).toBeUndefined();
            expect(Object.keys(holder)).toEqual([]);
        } else {
            expect(holder.tag).toBe("plain");
        }
    });

    it("counts a plain wrapper as a canary without state, under scope extended", function () {
        if (!canary || scope !== "extended") {
            return;
        }
        var states = stats().canaryStates;
        var plain = new java.lang.StringBuilder("counted");
        plain.tag = 1;
        var entry = runtime.getWrapperClassCounts()["java.lang.StringBuilder"];
        expect(entry.canaries).toBeGreaterThan(0);
        expect(entry.states).toBe(0);
        expect(stats().canaryStates).toBe(states);
        expect(Object.isFrozen(Object.freeze(plain))).toBe(true);
    });

    // An interface implementation Java hands back after its link was released gets a plain
    // wrapper built from the interface's template, which carries the write handlers.
    it("keeps no state for a plain wrapper whose template carries the write handlers, under scope extended", function () {
        var runnable = new java.lang.Runnable({ run: function () {} });
        Holder.held = runnable;
        __releaseNativeCounterpart(runnable);
        var again = Holder.held;
        expect(again).not.toBe(runnable);
        var states = stats().canaryStates;
        again.tag = "plain";
        expect(again.tag).toBe("plain");
        if (canary && scope === "all") {
            expect(stats().canaryStates).toBe(states + 1);
        } else {
            expect(stats().canaryStates).toBe(states);
        }
    });

    it("releases a plain wrapper's link and lets Java hand its object back in a new wrapper", function () {
        var plain = new java.lang.StringBuilder("released");
        plain.tag = "released";
        Holder.held = plain;
        var before = stats();
        __releaseNativeCounterpart(plain);
        expect(stats().releasedExplicitly).toBe(before.releasedExplicitly + 1);
        expect(function () {
            plain.toString();
        }).toThrowError(/no longer has available Java instance counterpart/);
        expect(plain.tag).toBe("released");

        var again = Holder.held;
        expect(again).not.toBe(plain);
        expect(again.toString()).toBe("released");
        expect(again.tag).toBeUndefined();
    });

    it("lets Java collect an extended object only JS referenced", function (done) {
        var ref = (function () {
            var runnable = new java.lang.Runnable({ run: function () {} });
            runnable.tag = "state";
            return new java.lang.ref.WeakReference(runnable);
        })();
        var deaths = stats().canaryNativeDeaths;
        eventually(function () {
            return ref.get() === null && (!canary || stats().canaryNativeDeaths > deaths);
        }, done, "the Java object or its state stayed alive");
    }, 20000);

    it("drops a call finalize() makes into a JS-implemented method", function (done) {
        var Finalizable = com.tns.tests.FinalizeCallsOverride;
        var Override = Finalizable.extend({
            onFinalize: function () {
                return 7;
            }
        });
        Finalizable.failure = null;
        var finalized = Finalizable.finalized;
        (function () {
            var instance = new Override();
            instance.tag = "state";
        })();
        var deadline = Date.now() + 10000;
        (function poll() {
            // No runFinalization(): the finalizer needs the isolate this thread would hold.
            gc();
            crossIntoJava();
            java.lang.Runtime.getRuntime().gc();
            if (Finalizable.finalized > finalized) {
                var failure = Finalizable.failure;
                expect(failure === null ? null : String(failure)).toBe(null);
                if (canary) {
                    expect(Finalizable.lastResult).toBe(0);
                } else {
                    // The resurrected wrapper may still be linked when Java finalizes.
                    expect([0, 7]).toContain(Finalizable.lastResult);
                }
                done();
                return;
            }
            if (Date.now() > deadline) {
                done.fail("Java did not finalize the object");
                return;
            }
            setTimeout(poll, 50);
        })();
    }, 20000);

    // The contract: the state is rooted until Java collects the object, so state that
    // references its own object keeps it alive. Break such back references with a WeakRef.
    it("keeps an extended object whose JS state references it", function () {
        if (!canary) {
            return;
        }
        var ref = (function () {
            var runnable = new java.lang.Runnable({ run: function () {} });
            runnable.self = runnable;
            return new java.lang.ref.WeakReference(runnable);
        })();
        collectBoth();
        collectJava();
        collectBoth();

        expect(ref.get()).not.toBe(null);
        __releaseNativeCounterpart(ref.get());
    });

    it("lets an explicitly released extended object's state go", function () {
        if (!canary) {
            return;
        }
        var runnable = new java.lang.Runnable({ run: function () {} });
        runnable.tag = "released";
        Holder.held = runnable;
        __releaseNativeCounterpart(runnable);
        runnable = null;
        collectBoth();

        var again = Holder.held;
        expect(again.tag).toBeUndefined();
    });

    it("counts live wrappers, canaries and states per Java class", function () {
        var runnable = new java.lang.Runnable({ run: function () {} });
        runnable.tag = 1;
        var counts = runtime.getWrapperClassCounts();
        var names = Object.keys(counts).filter(function (name) {
            return counts[name].wrappers > 0;
        });
        expect(names.length).toBeGreaterThan(0);
        var entry = counts[runnable.getClass().getName()];
        expect(entry).toBeDefined();
        expect(entry.wrappers).toBeGreaterThan(0);
        if (canary) {
            expect(entry.canaries).toBeGreaterThan(0);
            expect(entry.states).toBeGreaterThan(0);
        } else {
            expect(entry.canaries).toBe(0);
            expect(entry.states).toBe(0);
        }
    });

    it("counts canary activity", function () {
        var before = stats();
        var runnable = new java.lang.Runnable({ run: function () {} });
        runnable.tag = 1;
        var after = stats();
        if (canary) {
            expect(after.canaryStates).toBe(before.canaryStates + 1);
            expect(after.canaryInterceptedWrites).toBe(before.canaryInterceptedWrites + 1);
        } else {
            expect(after.canaryStates).toBe(before.canaryStates);
            expect(after.canaryInterceptedWrites).toBe(before.canaryInterceptedWrites);
        }
    });
});
