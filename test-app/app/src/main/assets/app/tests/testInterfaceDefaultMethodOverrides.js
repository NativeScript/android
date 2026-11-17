// The interfaces here are reached through aliases and never named in full in a `new` or
// `extend` call (except where a spec says otherwise), so the static binding generator does
// not prebuild their classes and the runtime generates them on the device.
describe("Interface default methods implemented in JS", function () {
    var sdk = android.os.Build.VERSION.SDK_INT;
    var F = com.tns.tests.DefaultMethodFixtures;
    var SUFFIX = "-defaults";

    var It = java.util.Iterator;
    var Pred = java.util.function.Predicate;
    var Split = java.util.Spliterator;

    function emptyIterator(members) {
        var implementation = {
            hasNext: function () { return false; },
            next: function () { return null; }
        };
        for (var name in members) {
            implementation[name] = members[name];
        }
        return implementation;
    }

    it("are called from Java on a class generated at runtime", function () {
        if (sdk < 24) {
            return;
        }
        var calls = [];
        var it = new It(emptyIterator({
            remove: function () { calls.push("remove"); },
            forEachRemaining: function (consumer) {
                calls.push("forEachRemaining");
                consumer.accept("js");
            }
        }));

        expect(F.isRuntimeGenerated(it)).toBe(true);
        expect(F.className(it)).toBe("com.tns.gen.java.util.Iterator" + SUFFIX);
        expect(F.remove(it)).toBe("removed");
        expect(F.drain(it)).toBe("js");
        expect(calls).toEqual(["remove", "forEachRemaining"]);
    });

    it("pass arguments and return primitive and object results", function () {
        if (sdk < 24) {
            return;
        }
        var seenCharacteristics = [];
        var spliterator = new Split({
            tryAdvance: function () { return false; },
            trySplit: function () { return null; },
            estimateSize: function () { return 3; },
            characteristics: function () { return Split.SIZED; },
            getExactSizeIfKnown: function () { return 42; },
            hasCharacteristics: function (characteristics) {
                seenCharacteristics.push(characteristics);
                return characteristics === Split.SORTED;
            }
        });
        expect(F.spliteratorDefaults(spliterator)).toBe("42|false|true");
        expect(seenCharacteristics).toEqual([Split.SIZED, Split.SORTED]);

        var negated = new Pred({ test: function (value) { return value === "no"; } });
        var andArguments = [];
        var predicate = new Pred({
            test: function (value) { return value === "yes"; },
            negate: function () { return negated; },
            and: function (other) {
                andArguments.push(other);
                return other;
            }
        });
        expect(F.negate(predicate, "yes")).toBe("true|false");
        expect(F.negate(predicate, "no")).toBe("false|true");
        expect(F.and(predicate, negated, "no")).toBe("true");
        expect(andArguments.length).toBe(1);
        expect(andArguments[0]).toBe(negated);
    });

    it("cover a default declared in a super-interface, an overloaded name and a narrowed return type", function () {
        if (sdk < 24) {
            return;
        }
        var OfInt = java.util.PrimitiveIterator.OfInt;
        var calls = [];
        var ofInt = new OfInt({
            hasNext: function () { return false; },
            nextInt: function () { return 0; },
            // Iterator's default
            remove: function () { calls.push("remove"); },
            // forEachRemaining(IntConsumer) and forEachRemaining(Consumer) share the name
            forEachRemaining: function () { calls.push("forEachRemaining"); },
            // Integer next() is a default narrowing Iterator's abstract Object next()
            next: function () {
                calls.push("next");
                return new java.lang.Integer(5);
            }
        });

        expect(F.className(ofInt)).toBe("com.tns.gen.java.util.PrimitiveIterator_OfInt" + SUFFIX);
        expect(F.remove(ofInt)).toBe("removed");
        expect(F.drainInts(ofInt)).toBe("");
        expect(F.drain(ofInt)).toBe("");
        expect(F.next(ofInt)).toBe("5");
        expect(calls).toEqual(["remove", "forEachRemaining", "forEachRemaining", "next"]);
    });

    it("leave the defaults the JS object lacks to their Java bodies, on the same instance", function () {
        if (sdk < 24) {
            return;
        }
        var drains = 0;
        var it = new It(emptyIterator({
            forEachRemaining: function () { drains++; }
        }));

        expect(F.className(it)).toBe("com.tns.gen.java.util.Iterator" + SUFFIX);
        // Iterator.remove() throws this itself, so it arrives with its own type.
        expect(F.remove(it)).toBe("java.lang.UnsupportedOperationException");
        F.drain(it);
        expect(drains).toBe(1);
    });

    it("run a missing default on the calling thread without the JS thread", function () {
        if (sdk < 24) {
            return;
        }
        var drains = 0;
        var removes = 0;
        var it = new It(emptyIterator({
            forEachRemaining: function () { drains++; }
        }));
        // This thread waits inside the fixture, so a call that needed it would time out.
        expect(F.removeOnBackgroundThread(it, 5000)).toBe(F.CALLER_THREAD + "|java.lang.UnsupportedOperationException|true");
        expect(drains).toBe(0);

        var withRemove = new It(emptyIterator({
            remove: function () { removes++; }
        }));
        expect(F.removeOnBackgroundThread(withRemove, 300)).toBe("timeout");
    }, 15000);

    it("keep the class an instance gets today when its JS object defines no default method", function () {
        if (sdk < 24) {
            return;
        }
        var IntPred = java.util.function.IntPredicate;
        var predicate = new IntPred({ test: function (value) { return value > 0; } });

        expect(F.className(predicate)).toBe("com.tns.gen.java.util.function.IntPredicate");
        expect(F.isClassLoaded("com.tns.gen.java.util.function.IntPredicate" + SUFFIX, predicate)).toBe(false);
        expect(F.intNegate(predicate, 1)).toBe("true|false");

        var plain = new Pred({ test: function () { return true; } });
        var overriding = new Pred({
            test: function () { return true; },
            negate: function () { return plain; }
        });
        expect(F.className(plain)).toBe("com.tns.gen.java.util.function.Predicate");
        expect(F.className(overriding)).toBe("com.tns.gen.java.util.function.Predicate" + SUFFIX);
        expect(F.negate(plain, "x")).toBe("true|false");
        expect(F.negate(overriding, "x")).toBe("true|true");
    });

    it("count an own function or accessor as an implementation, and nothing else", function () {
        if (sdk < 24) {
            return;
        }
        var notAFunction = new It(emptyIterator({ remove: 42 }));
        expect(F.className(notAFunction)).toBe("com.tns.gen.java.util.Iterator");
        expect(F.remove(notAFunction)).toBe("java.lang.UnsupportedOperationException");

        var getterCalls = 0;
        var removes = 0;
        var withAccessor = emptyIterator({});
        Object.defineProperty(withAccessor, "remove", {
            get: function () {
                getterCalls++;
                return function () { removes++; };
            }
        });
        var viaAccessor = new It(withAccessor);
        expect(getterCalls).toBe(0);
        expect(F.className(viaAccessor)).toBe("com.tns.gen.java.util.Iterator" + SUFFIX);
        expect(F.remove(viaAccessor)).toBe("removed");
        expect(getterCalls).toBe(1);
        expect(removes).toBe(1);

        // Read at call time like any JS implementation: a getter returning something else
        // fails that call, as an abstract method's non-function implementation does.
        var withBadAccessor = emptyIterator({});
        Object.defineProperty(withBadAccessor, "remove", {
            get: function () { return 42; }
        });
        expect(F.remove(new It(withBadAccessor))).toBe("com.tns.NativeScriptException");
    });

    it("are called through a class from extend()", function () {
        if (sdk < 24) {
            return;
        }
        var LongPred = java.util.function.LongPredicate;
        var negations = 0;
        var Positive = LongPred.extend({
            test: function (value) { return value > 0; },
            negate: function () {
                negations++;
                return new LongPred({ test: function () { return true; } });
            }
        });
        var first = new Positive();
        var second = new Positive();

        expect(F.className(first)).toBe("com.tns.gen.java.util.function.LongPredicate" + SUFFIX);
        expect(F.longNegate(first, 5)).toBe("true|true");
        expect(F.longNegate(second, -5)).toBe("false|true");
        expect(negations).toBe(2);
    });

    it("are called from Java in a worker", function (done) {
        if (sdk < 24) {
            done();
            return;
        }
        var worker = new Worker("./defaultMethodOverridesWorker.js");
        worker.onmessage = function (msg) {
            worker.terminate();
            expect(msg.data.error).toBeUndefined();
            expect(msg.data.newClassName).toBe("com.tns.gen.java.util.Iterator" + SUFFIX);
            expect(msg.data.remove).toBe("removed");
            expect(msg.data.removes).toBe(1);
            expect(msg.data.missingDefault).toBe("java.lang.UnsupportedOperationException");
            expect(msg.data.extendClassName).toBe("com.tns.gen.java.util.Spliterator" + SUFFIX);
            expect(msg.data.spliterator).toBe("7|true|false");
            done();
        };
    }, 15000);

    it("leave a prebuilt class of the interface in charge", function () {
        if (sdk < 24) {
            return;
        }
        // Full name: the static binding generator prebuilds this interface's class with
        // the methods this call site lists.
        var prebuiltRemoves = 0;
        var prebuilt = new java.util.PrimitiveIterator.OfLong({
            hasNext: function () { return false; },
            nextLong: function () { return 0; },
            remove: function () { prebuiltRemoves++; }
        });
        expect(F.isRuntimeGenerated(prebuilt)).toBe(false);
        expect(F.remove(prebuilt)).toBe("removed");
        expect(prebuiltRemoves).toBe(1);

        var OfLong = java.util.PrimitiveIterator.OfLong;
        var aliasedRemoves = 0;
        var aliased = new OfLong({
            hasNext: function () { return false; },
            nextLong: function () { return 0; },
            remove: function () { aliasedRemoves++; }
        });
        expect(F.className(aliased)).toBe(F.className(prebuilt));
        expect(F.remove(aliased)).toBe("removed");
        expect(aliasedRemoves).toBe(1);
    });

    it("reach JS for an interface whose methods are all default methods, unless D8 desugared them", function () {
        if (sdk < 24) {
            return;
        }
        var L = com.tns.tests.interfaces.defaultmethods.contract.AllDefaultListener;
        var calls = [];
        var listener = new L({
            onText: function (text) { calls.push("text:" + text); },
            describe: function () { return "js describe"; }
        });

        if (F.isDesugared()) {
            // Below minSdk 24 the methods are abstract on the device: every one reaches JS,
            // and one the JS object lacks has no Java body to fall back to.
            expect(F.fire(listener)).toBe("com.tns.NativeScriptException");
            expect(calls).toEqual(["text:a"]);
            return;
        }

        expect(F.className(listener)).toBe("com.tns.gen.com.tns.tests.interfaces.defaultmethods.contract.AllDefaultListener" + SUFFIX);
        expect(F.fire(listener)).toBe("js describe");
        expect(calls).toEqual(["text:a"]);

        var onlyText = new L({ onText: function () {} });
        expect(F.describeOnBackgroundThread(onlyText, 5000)).toBe("java default on " + F.CALLER_THREAD);
    }, 15000);
});
