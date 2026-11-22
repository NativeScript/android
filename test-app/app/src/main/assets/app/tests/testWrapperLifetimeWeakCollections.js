// Holds in every wrapper lifetime mode: in "canary" the collections key a Java object by
// its lifetime key, elsewhere the wrapper itself outlives the JS references.
describe("Weak collections of Java objects", function () {
    var runtime = require("ns:runtime");
    var mode = runtime.getConfig("wrapperLifetimeMode");
    var scope = runtime.getConfig("wrapperLifetimeScope");
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

    // In canary mode a plain wrapper is a canary: its key hangs off its record, and deref
    // makes a new wrapper the usual way, with the old one's JS properties under scope "all"
    // only.
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
            if (canary && scope === "extended") {
                expect(target.tag).toBeUndefined();
            } else {
                expect(target.tag).toBe("state");
            }
            done();
        }, 0);
    });

    // A keyed plain wrapper under scope "extended" is a canary with a record and no state:
    // the next one is a new, empty object that stands for the same key.
    it("finds a keyed Java-held plain object through its next canary, which has no state, under scope extended", function (done) {
        if (!canary || scope !== "extended") {
            done();
            return;
        }
        var NativeWeakRef = Object.getPrototypeOf(WeakRef);
        var map = new WeakMap();
        var set = new WeakSet();
        var registry = new FinalizationRegistry(function () {});
        var target = {};
        var states = runtime.getWrapperLifetimeStats().canaryStates;
        var first;
        var refs = (function () {
            var builder = new java.lang.StringBuilder("plain");
            builder.tag = "lost";
            map.set(builder, "value");
            set.add(builder);
            registry.register(target, "held", builder);
            Holder.held = builder;
            first = new NativeWeakRef(builder);
            return [new WeakRef(builder), new WeakRef(builder)];
        })();
        setTimeout(function () {
            collectBoth();
            expect(first.deref()).toBeUndefined();
            // No canary is alive: deref builds one.
            var built = refs[0].deref();
            expect(built).toBeDefined();
            expect(built.toString()).toBe("plain");
            expect(built.tag).toBeUndefined();
            expect(Holder.held === built).toBe(true);
            built = null;
            collectBoth();
            var again = Holder.held;
            expect(again.tag).toBeUndefined();
            expect(map.get(again)).toBe("value");
            expect(map.has(again)).toBe(true);
            expect(set.has(again)).toBe(true);
            expect(refs[1].deref() === again).toBe(true);
            expect(registry.unregister(again)).toBe(true);
            expect(runtime.getWrapperLifetimeStats().canaryStates).toBe(states);
            done();
        }, 0);
    });

    it("runs a FinalizationRegistry callback for a plain object only once Java collected it", function (done) {
        if (!canary) {
            done();
            return;
        }
        var heldCleaned = false;
        var droppedCleaned = false;
        var registry = new FinalizationRegistry(function (held) {
            if (held === "held") {
                heldCleaned = true;
            } else if (held === "dropped") {
                droppedCleaned = true;
            }
        });
        (function () {
            var held = new java.lang.StringBuilder("held");
            Holder.held = held;
            registry.register(held, "held");
            registry.register(new java.lang.StringBuilder("dropped"), "dropped");
        })();
        function finish() {
            expect(heldCleaned).toBe(false);
            expect(Holder.held.toString()).toBe("held");
            done();
        }
        finish.fail = done.fail;
        eventually(function () {
            // A collected registry runs no callbacks.
            return droppedCleaned && registry !== null;
        }, finish, "the cleanup callback did not run");
    }, 20000);

    it("keeps what was keyed on a plain Java object after its link is released", function () {
        if (!canary) {
            return;
        }
        var map = new WeakMap();
        var set = new WeakSet();
        var builder = new java.lang.StringBuilder("released");
        map.set(builder, "value");
        set.add(builder);
        __releaseNativeCounterpart(builder);
        collectBoth();
        expect(map.get(builder)).toBe("value");
        expect(set.has(builder)).toBe(true);
        expect(new WeakRef(builder).deref() === builder).toBe(true);
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
            // A collected registry runs no callbacks.
            return cleaned && registry !== null;
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
            // A collected registry runs no callbacks, so this spec would pass without one.
            expect(registry instanceof FinalizationRegistry).toBe(true);
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

    it("gives no state to a Java object it only keys", function () {
        var map = new WeakMap();
        var set = new WeakSet();
        var builder = new java.lang.StringBuilder("keyed");
        var states = runtime.getWrapperLifetimeStats().canaryStates;
        map.set(builder, 1);
        set.add(builder);
        expect(map.get(builder)).toBe(1);
        expect(set.has(builder)).toBe(true);
        expect(runtime.getWrapperLifetimeStats().canaryStates).toBe(states);
    });

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

    // A canary built for an object that has a key, by any path (here a Java field read, not
    // a deref), stands for the key at once; a canary that was never a key makes no state.
    it("finds what was keyed on a Java object through a canary built by a Java read", function (done) {
        var map = new WeakMap();
        var set = new WeakSet();
        var registry = new FinalizationRegistry(function () {});
        var token = new java.lang.StringBuilder("token");
        var NativeWeakRef = Object.getPrototypeOf(WeakRef) === Function.prototype ? WeakRef : Object.getPrototypeOf(WeakRef);
        var rebuilds = runtime.getWrapperLifetimeStats().canaryRebuilds;
        var firstCanaries;
        var refs = (function () {
            var plain = new java.lang.StringBuilder("plain");
            map.set(plain, "plain value");
            set.add(plain);
            var runnable = new java.lang.Runnable({ run: function () {} });
            registry.register(runnable, "held", plain);
            map.set(runnable, "extended value");
            var list = new java.util.ArrayList();
            list.add(plain);
            list.add(runnable);
            Holder.held = list;
            firstCanaries = [new NativeWeakRef(plain), new NativeWeakRef(runnable)];
            return [new WeakRef(plain), new WeakRef(runnable)];
        })();
        setTimeout(function () {
            collectBoth();
            if (canary) {
                expect(firstCanaries[0].deref()).toBeUndefined();
                expect(firstCanaries[1].deref()).toBeUndefined();
            }
            var list = Holder.held;
            var held = [list.get(0), list.get(1)];
            if (canary) {
                expect(runtime.getWrapperLifetimeStats().canaryRebuilds).toBeGreaterThan(rebuilds);
            }
            expect(map.get(held[0])).toBe("plain value");
            expect(set.has(held[0])).toBe(true);
            expect(map.get(held[1])).toBe("extended value");
            expect(refs[0].deref() === held[0]).toBe(true);
            expect(refs[1].deref() === held[1]).toBe(true);
            expect(refs[1].deref() === refs[1].deref()).toBe(true);
            expect(registry.unregister(held[0])).toBe(true);
            expect(set.delete(held[0])).toBe(true);
            expect(set.has(held[0])).toBe(false);
            expect(map.delete(held[1])).toBe(true);
            expect(map.has(held[1])).toBe(false);
            var states = runtime.getWrapperLifetimeStats().canaryStates;
            expect(map.has(token)).toBe(false);
            expect(map.get(token)).toBeUndefined();
            expect(set.delete(token)).toBe(false);
            expect(registry.unregister(token)).toBe(false);
            expect(runtime.getWrapperLifetimeStats().canaryStates).toBe(states);
            var plainTarget = {};
            expect(new WeakRef(plainTarget).deref()).toBe(plainTarget);
            done();
        }, 0);
    });

    // Keying a canary must not keep it alive: only WeakRef construction and deref do, until
    // the job ends, as the standard says.
    it("lets a canary that was just keyed die in the same job", function () {
        var map = new WeakMap();
        var set = new WeakSet();
        var rebuilds = runtime.getWrapperLifetimeStats().canaryRebuilds;
        (function () {
            var runnable = heldRunnable();
            map.set(runnable, "value");
            set.add(runnable);
        })();
        collectBoth();
        expect(map.get(Holder.held)).toBe("value");
        expect(set.has(Holder.held)).toBe(true);
        if (canary) {
            expect(runtime.getWrapperLifetimeStats().canaryRebuilds).toBeGreaterThan(rebuilds);
        }
    });

    function nativeCalls() {
        return runtime.getWrapperLifetimeStats().weakCollectionNativeCalls;
    }

    it("never calls into the runtime for plain JS objects or lookups of values that were never keys", function () {
        var map = new WeakMap();
        var set = new WeakSet();
        var registry = new FinalizationRegistry(function () {});
        var plain = {};
        var other = {};
        map.set(plain, 1);
        set.add(plain);
        registry.register(plain, "held", plain);
        var ref = new WeakRef(plain);
        var javaObject = new java.lang.Object();
        var runnable = new java.lang.Runnable({ run: function () {} });
        // A miss takes whatever canaries earlier specs left to be told about.
        map.has({});
        var before = nativeCalls();
        expect(map.get(plain)).toBe(1);
        expect(map.has(other)).toBe(false);
        expect(map.get(other)).toBeUndefined();
        expect(map.delete(other)).toBe(false);
        expect(set.has(plain)).toBe(true);
        expect(set.has(other)).toBe(false);
        expect(set.delete(other)).toBe(false);
        expect(ref.deref()).toBe(plain);
        expect(ref.get()).toBe(plain);
        expect(registry.unregister(other)).toBe(false);
        map.set(plain, 2);
        set.add(plain);
        expect(map.has(javaObject)).toBe(false);
        expect(map.get(runnable)).toBeUndefined();
        expect(set.has(runnable)).toBe(false);
        expect(registry.unregister(runnable)).toBe(false);
        var fresh = {};
        map.set(fresh, 3);
        set.add(fresh);
        registry.register(fresh, "fresh", fresh);
        expect(new WeakRef(fresh).deref()).toBe(fresh);
        expect(map.getOrInsert ? map.getOrInsert({}, 4) : 4).toBe(4);
        expect(nativeCalls()).toBe(before);
    });

    // The runtime only counts the canaries it builds for keyed objects; the first lookup
    // that misses learns them all with one call.
    it("finds a keyed Java object through canaries built while no lookup ran", function () {
        var map = new WeakMap();
        var set = new WeakSet();
        var registry = new FinalizationRegistry(function () {});
        var rebuilds = runtime.getWrapperLifetimeStats().canaryRebuilds;
        (function () {
            var runnable = heldRunnable();
            map.set(runnable, "value");
            set.add(runnable);
            registry.register(runnable, "held", runnable);
        })();
        collectBoth();
        (function () {
            expect(set.has(Holder.held)).toBe(true);
        })();
        collectBoth();
        // Built and dead again before any lookup saw it.
        (function () {
            Holder.held.hashCode();
        })();
        collectBoth();
        var again = Holder.held;
        if (canary) {
            expect(runtime.getWrapperLifetimeStats().canaryRebuilds).toBeGreaterThan(rebuilds + 2);
        }
        var before = nativeCalls();
        expect(map.has({})).toBe(false);
        if (canary) {
            expect(nativeCalls()).toBe(before + 1);
        }
        expect(map.has({})).toBe(false);
        expect(map.get(again)).toBe("value");
        expect(set.has(again)).toBe(true);
        expect(registry.unregister(again)).toBe(true);
        if (canary) {
            expect(nativeCalls()).toBe(before + 1);
        }
    });

    // Past 64 queued canaries the runtime prunes the queue as it builds more; the latest
    // canary of an object built many times must survive the prunes the others' canaries make.
    it("finds keyed Java objects through canaries built past the queue's prune while no lookup ran", function () {
        var map = new WeakMap();
        var list = new java.util.ArrayList();
        Holder.held = list;
        var count = 8;
        (function () {
            for (var i = 0; i < count; i++) {
                var runnable = new java.lang.Runnable({ run: function () {} });
                list.add(runnable);
                map.set(runnable, i);
            }
        })();
        gc();
        map.has({});
        var rebuilds = runtime.getWrapperLifetimeStats().canaryRebuilds;
        for (var n = 0; n < 63; n++) {
            (function () {
                list.get(0).hashCode();
            })();
            gc();
        }
        var first = list.get(0);
        var others = [];
        for (var i = 1; i < count; i++) {
            others.push(list.get(i));
        }
        if (canary) {
            expect(runtime.getWrapperLifetimeStats().canaryRebuilds).toBeGreaterThan(rebuilds + 64);
        }
        var before = nativeCalls();
        expect(map.get(first)).toBe(0);
        for (var j = 0; j < others.length; j++) {
            expect(map.get(others[j])).toBe(j + 1);
        }
        if (canary) {
            expect(nativeCalls()).toBe(before + 1);
        }
    });

    it("finds a keyed Java object through a canary built and released while no lookup ran", function () {
        var map = new WeakMap();
        var set = new WeakSet();
        (function () {
            var runnable = heldRunnable();
            map.set(runnable, "value");
            set.add(runnable);
        })();
        collectBoth();
        var released = Holder.held;
        __releaseNativeCounterpart(released);
        expect(set.has(released)).toBe(true);
        expect(map.get(released)).toBe("value");
        expect(map.has({})).toBe(false);
    });

    it("installs the canary collections only in canary mode", function () {
        var native = WeakMap.toString().indexOf("[native code]") !== -1;
        expect(native).toBe(!canary);
    });
});
