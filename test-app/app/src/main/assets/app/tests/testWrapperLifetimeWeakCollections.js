// Holds in every wrapper lifetime mode: in "canary" the collections key a Java object by
// its lifetime key, elsewhere the wrapper itself outlives the JS references.
describe("Weak collections of Java objects", function () {
    var runtime = require("ns:runtime");
    var mode = runtime.getConfig("wrapperLifetimeMode");
    var canary = mode === "canary";
    var Holder = com.tns.tests.WrapperHolder;

    function collectJava() {
        for (var i = 0; i < 3; i++) {
            java.lang.Runtime.getRuntime().gc();
            java.lang.System.runFinalization();
        }
    }

    function crossIntoJava() {
        java.lang.System.nanoTime();
    }

    // Lets JS drop every wrapper nothing references.
    function collectBoth() {
        gc();
        crossIntoJava();
        collectJava();
        gc();
        crossIntoJava();
    }

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

    // A JS-extended object, a canary in every canary scope; Java keeps it in Holder.
    function heldRunnable() {
        var runnable = new java.lang.Runnable({ run: function () {} });
        Holder.held = runnable;
        return runnable;
    }

    afterEach(function () {
        Holder.held = null;
    });

    it("keeps a WeakMap entry for a Java object across a wrapper change (" + mode + ")", function () {
        var map = new WeakMap();
        (function () {
            map.set(heldRunnable(), "value");
        })();
        collectBoth();

        var again = Holder.held;
        expect(map.has(again)).toBe(true);
        expect(map.get(again)).toBe("value");
        expect(map.delete(again)).toBe(true);
        expect(map.has(again)).toBe(false);
    });

    // A canary rebuilt more than once is a copy of an earlier rebuilt one: the key lives
    // with the object's state, not on any canary, until the link is released.
    it("keys canaries rebuilt from a copy of an earlier one by their Java object, through a release", function () {
        var map = new WeakMap();
        var set = new WeakSet();
        (function () {
            heldRunnable();
        })();
        collectBoth();
        (function () {
            map.set(Holder.held, "first");
        })();
        collectBoth();
        (function () {
            var second = Holder.held;
            expect(map.get(second)).toBe("first");
            expect(map.delete(second)).toBe(true);
            set.add(second);
        })();
        collectBoth();

        var third = Holder.held;
        expect(map.has(third)).toBe(false);
        expect(set.has(third)).toBe(true);
        map.set(third, "third");
        __releaseNativeCounterpart(third);
        expect(map.get(third)).toBe("third");
        expect(set.has(third)).toBe(true);
        var after = Holder.held;
        expect(after === third).toBe(false);
        expect(map.has(after)).toBe(false);
        expect(set.has(after)).toBe(false);
    });

    it("keeps a WeakSet member for a Java object across a wrapper change", function () {
        var set = new WeakSet();
        (function () {
            set.add(heldRunnable());
        })();
        collectBoth();

        expect(set.has(Holder.held)).toBe(true);
    });

    it("keeps a getOrInsert entry for a Java object across a wrapper change", function () {
        var map = new WeakMap();
        if (typeof map.getOrInsert !== "function") {
            return;
        }
        (function () {
            map.getOrInsert(heldRunnable(), "first");
        })();
        collectBoth();

        expect(map.getOrInsert(Holder.held, "second")).toBe("first");
        var seen;
        expect(map.getOrInsertComputed(Holder.held, function (key) {
            seen = key;
            return "computed";
        })).toBe("first");
        expect(seen).toBeUndefined();
    });

    // A WeakRef keeps its target alive until the job that made it ends, so the wrapper can
    // only change on a later task.
    it("dereferences a WeakRef to a Java-held object while the object lives", function (done) {
        var rebuilds = runtime.getWrapperLifetimeStats().canaryRebuilds;
        var ref = (function () {
            var runnable = heldRunnable();
            runnable.tag = "state";
            return new WeakRef(runnable);
        })();
        setTimeout(function () {
            collectBoth();
            if (canary) {
                // Rebuilt by deref below, not by anything this spec kept.
                expect(runtime.getWrapperLifetimeStats().canaryRebuilds).toBe(rebuilds);
            }
            var target = ref.deref();
            expect(target).toBeDefined();
            expect(target.tag).toBe("state");
            expect(ref.get()).toBe(target);
            expect(target.equals(Holder.held)).toBe(true);
            if (canary) {
                expect(runtime.getWrapperLifetimeStats().canaryRebuilds).toBe(rebuilds + 1);
            }
            done();
        }, 0);
    });

    // Under scope "all" a plain wrapper is a canary: its key hangs off its state, and deref
    // makes a new wrapper the usual way, hydrated from the state.
    it("keeps a WeakMap entry and a WeakRef target for a Java-held plain object across a wrapper change", function (done) {
        var map = new WeakMap();
        var ref = (function () {
            var builder = new java.lang.StringBuilder("plain");
            builder.tag = "state";
            map.set(builder, "value");
            Holder.held = builder;
            return new WeakRef(builder);
        })();
        setTimeout(function () {
            collectBoth();
            var again = Holder.held;
            expect(map.get(again)).toBe("value");
            var target = ref.deref();
            expect(target).toBeDefined();
            expect(target.toString()).toBe("plain");
            expect(target.tag).toBe("state");
            done();
        }, 0);
    });

    it("clears a WeakRef once Java collected the object", function (done) {
        var ref = (function () {
            return new WeakRef(new java.lang.Runnable({ run: function () {} }));
        })();
        eventually(function () {
            return ref.deref() === undefined;
        }, done, "the WeakRef still dereferences");
    }, 20000);

    it("runs a FinalizationRegistry callback once Java collected the object", function (done) {
        var cleaned = false;
        var registry = new FinalizationRegistry(function (held) {
            cleaned = held === "held";
        });
        (function () {
            registry.register(new java.lang.Runnable({ run: function () {} }), "held");
        })();
        eventually(function () {
            return cleaned;
        }, done, "the cleanup callback did not run");
    }, 20000);

    it("unregisters a FinalizationRegistry target through a later wrapper of its token", function () {
        var registry = new FinalizationRegistry(function () {});
        var target = {};
        (function () {
            var runnable = heldRunnable();
            registry.register(target, "held", runnable);
        })();
        collectBoth();

        expect(registry.unregister(Holder.held)).toBe(true);
        expect(target).toBeDefined();
    });

    it("rejects a FinalizationRegistry registration whose target is its held value", function () {
        var registry = new FinalizationRegistry(function () {});
        var runnable = new java.lang.Runnable({ run: function () {} });
        expect(function () {
            registry.register(runnable, runnable);
        }).toThrowError(TypeError);
    });

    it("keys a Proxy around a Java object by the Proxy", function () {
        var runnable = new java.lang.Runnable({ run: function () {} });
        var proxy = new Proxy(runnable, {});
        var map = new WeakMap();
        map.set(proxy, "proxy");
        expect(map.get(proxy)).toBe("proxy");
        expect(map.has(runnable)).toBe(false);
    });

    it("does not give state to a Java object a lookup only probes", function () {
        var map = new WeakMap();
        var set = new WeakSet();
        var runnable = new java.lang.Runnable({ run: function () {} });
        var states = runtime.getWrapperLifetimeStats().canaryStates;
        expect(map.get(runnable)).toBeUndefined();
        expect(map.has(runnable)).toBe(false);
        expect(set.has(runnable)).toBe(false);
        expect(map.delete(runnable)).toBe(false);
        expect(runtime.getWrapperLifetimeStats().canaryStates).toBe(states);
    });

    it("keeps working as the standard collections for other keys", function () {
        var map = new WeakMap([[Object, 1]]);
        var key = {};
        map.set(key, 2);
        expect(map.get(Object)).toBe(1);
        expect(map.get(key)).toBe(2);
        var ref = new WeakRef(key);
        expect(ref.deref()).toBe(key);
        expect(new WeakSet([key]).has(key)).toBe(true);
        expect(map instanceof WeakMap).toBe(true);
    });

    it("keys the entries given to the WeakMap and WeakSet constructors by the Java object", function () {
        var map;
        var set;
        (function () {
            var runnable = heldRunnable();
            map = new WeakMap([[runnable, "constructed"]]);
            set = new WeakSet([runnable]);
        })();
        collectBoth();

        expect(map.get(Holder.held)).toBe("constructed");
        expect(set.has(Holder.held)).toBe(true);
    });

    it("keys a Java object in a subclass of WeakMap", function () {
        class TaggedMap extends WeakMap {
            set(key, value) {
                return super.set(key, "tagged:" + value);
            }
        }
        var map = new TaggedMap();
        (function () {
            map.set(heldRunnable(), "value");
        })();
        collectBoth();

        expect(map.get(Holder.held)).toBe("tagged:value");
        expect(map instanceof WeakMap).toBe(true);
    });

    it("does not run a FinalizationRegistry callback while Java holds the object", function (done) {
        var cleaned = false;
        var registry = new FinalizationRegistry(function () {
            cleaned = true;
        });
        (function () {
            registry.register(heldRunnable(), "held");
        })();
        var polls = 0;
        (function poll() {
            collectBoth();
            if (cleaned) {
                done.fail("the cleanup callback ran while Java held the object");
                return;
            }
            if (++polls < 5) {
                setTimeout(poll, 50);
                return;
            }
            expect(Holder.held).not.toBe(null);
            done();
        })();
    }, 20000);

    // Released, the wrapper is a plain JS object: what was keyed on it lives exactly as long
    // as it does. Only booleans reach expect(), which could otherwise keep the wrapper.
    it("keeps what was keyed on a Java object after its link is released, while the wrapper lives", function (done) {
        var map = new WeakMap();
        var set = new WeakSet();
        var cleaned = false;
        var registry = new FinalizationRegistry(function (held) {
            cleaned = held === "released";
        });
        var runnable = new java.lang.Runnable({ run: function () {} });
        map.set(runnable, "value");
        set.add(runnable);
        var ref = new WeakRef(runnable);
        registry.register(runnable, "released");
        __releaseNativeCounterpart(runnable);

        expect(map.get(runnable)).toBe("value");
        expect(set.has(runnable)).toBe(true);
        expect(ref.deref() === runnable).toBe(true);
        setTimeout(function () {
            collectBoth();
            expect(map.get(runnable)).toBe("value");
            expect(set.has(runnable)).toBe(true);
            expect(ref.deref() === runnable).toBe(true);
            expect(new WeakRef(runnable).deref() === runnable).toBe(true);
            map.set(runnable, "again");
            expect(map.get(runnable)).toBe("again");
            expect(cleaned).toBe(false);
            runnable = null;
            setTimeout(function () {
                eventually(function () {
                    // A collected registry runs no callbacks.
                    return cleaned && registry !== null;
                }, done, "the cleanup callback did not run once the wrapper was collected");
            }, 0);
        }, 0);
    }, 20000);

    it("makes no key for a call the standard collection rejects", function () {
        var runnable = new java.lang.Runnable({ run: function () {} });
        var registry = new FinalizationRegistry(function () {});
        var map = new WeakMap();
        var states = runtime.getWrapperLifetimeStats().canaryStates;
        expect(function () {
            WeakMap.prototype.set.call({}, runnable, 1);
        }).toThrowError(TypeError);
        expect(function () {
            WeakSet.prototype.add.call({}, runnable);
        }).toThrowError(TypeError);
        expect(function () {
            registry.register(runnable, "held", 1);
        }).toThrowError(TypeError);
        expect(function () {
            FinalizationRegistry.prototype.register.call({}, runnable, "held");
        }).toThrowError(TypeError);
        if (typeof map.getOrInsertComputed === "function") {
            expect(function () {
                WeakMap.prototype.getOrInsert.call({}, runnable, 1);
            }).toThrowError(TypeError);
            expect(function () {
                map.getOrInsertComputed(runnable, function () {
                    throw new Error("callback");
                });
            }).toThrowError("callback");
        }
        expect(runtime.getWrapperLifetimeStats().canaryStates).toBe(states);
    });

    it("hands getOrInsertComputed's callback the key it was given", function () {
        var map = new WeakMap();
        if (typeof map.getOrInsertComputed !== "function") {
            return;
        }
        var runnable = new java.lang.Runnable({ run: function () {} });
        var seen;
        expect(map.getOrInsertComputed(runnable, function (key) {
            seen = key;
            return "computed";
        })).toBe("computed");
        expect(seen === runnable).toBe(true);
        expect(map.get(runnable)).toBe("computed");
    });

    it("matches the standard FinalizationRegistry.prototype.register signature and errors", function () {
        var registry = new FinalizationRegistry(function () {});
        expect(FinalizationRegistry.prototype.register.length).toBe(2);
        expect(function () {
            registry.register(1, 1);
        }).toThrowError(TypeError, /invalid target/);
    });

    it("installs the canary collections only in canary mode", function () {
        var native = WeakMap.toString().indexOf("[native code]") !== -1;
        expect(native).toBe(!canary);
    });
});
