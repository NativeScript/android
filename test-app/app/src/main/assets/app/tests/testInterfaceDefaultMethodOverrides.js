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

    it("are called through a class extend that implements the interface", function () {
        if (sdk < 24) {
            return;
        }
        var O = java.lang.Object;
        var removes = 0;
        var WithRemove = O.extend({
            interfaces: [It],
            hasNext: function () { return false; },
            next: function () { return null; },
            remove: function () { removes++; }
        });
        var withRemove = new WithRemove();
        expect(F.isRuntimeGenerated(withRemove)).toBe(true);
        expect(F.remove(withRemove)).toBe("removed");
        expect(removes).toBe(1);

        var WithoutRemove = O.extend({
            interfaces: [It],
            hasNext: function () { return false; },
            next: function () { return null; }
        });
        expect(F.remove(new WithoutRemove())).toBe("java.lang.UnsupportedOperationException");
    });

    it("are called through a class extend whose base class inherits them", function () {
        var hasAsIterator = false;
        try {
            java.util.Enumeration.class.getMethod("asIterator", []);
            hasAsIterator = true;
        } catch (e) {
        }
        if (!hasAsIterator) {
            return;
        }
        var Tokenizer = java.util.StringTokenizer;
        var asIteratorCalls = 0;
        var Tokens = Tokenizer.extend({
            asIterator: function () {
                asIteratorCalls++;
                return new java.util.ArrayList().iterator();
            }
        });
        var tokens = new Tokens("a b");
        expect(F.isRuntimeGenerated(tokens)).toBe(true);
        expect(F.asIterator(tokens)).toBe("java.util.ArrayList$Itr");
        expect(asIteratorCalls).toBe(1);
    });

    it("are called through a class extend of an app class, unless D8 desugared them", function () {
        if (sdk < 24) {
            return;
        }
        var CarProducer = com.tns.tests.interfaces.defaultmethods.impl.CarProducerImpl;
        var produced = 0;
        var Producing = CarProducer.extend({
            produce: function () {
                produced++;
                return "js produce";
            }
        });
        var producing = new Producing();

        if (F.isDesugared()) {
            // D8 gives CarProducerImpl a synthetic produce() forwarding to the moved
            // body; generated classes do not override synthetic methods.
            expect(F.produce(producing)).toBe("default produce in CarProducer");
            expect(produced).toBe(0);
            return;
        }
        expect(F.produce(producing)).toBe("js produce");
        expect(produced).toBe(1);
    });

    function namedIterator(drains) {
        return {
            hasNext: function () { return false; },
            next: function () { return null; },
            forEachRemaining: function () { drains.push("forEachRemaining"); }
        };
    }

    // Computed member: the static binding generator does not see these named extends.
    var ext = "ex" + "tend";

    it("are called through a named class extended from the interface at runtime", function () {
        if (sdk < 24) {
            return;
        }
        var drains = [];
        var Named = It[ext]("com.tns.tests.defaults.NamedRuntimeIterator", namedIterator(drains));
        var named = new Named();

        expect(F.className(named)).toBe("com.tns.tests.defaults.NamedRuntimeIterator");
        expect(F.isRuntimeGenerated(named)).toBe(true);
        expect(F.drain(named)).toBe("");
        expect(drains).toEqual(["forEachRemaining"]);
        expect(F.remove(named)).toBe("java.lang.UnsupportedOperationException");
    });

    it("are called through a named class the static binding generator prebuilt from the interface", function () {
        if (sdk < 24) {
            return;
        }
        var drains = [];
        // An object literal: the static binding generator reads the methods it lists.
        var Named = java.util.Iterator.extend("com.tns.tests.defaults.NamedPrebuiltIterator", {
            hasNext: function () { return false; },
            next: function () { return null; },
            forEachRemaining: function () { drains.push("forEachRemaining"); }
        });
        var named = new Named();

        expect(F.className(named)).toBe("com.tns.tests.defaults.NamedPrebuiltIterator");
        expect(F.isRuntimeGenerated(named)).toBe(false);
        expect(F.drain(named)).toBe("");
        expect(drains).toEqual(["forEachRemaining"]);
        expect(F.remove(named)).toBe("java.lang.UnsupportedOperationException");
    });

    it("are called through a JavaProxy-named class extended from the interface at runtime", function () {
        if (sdk < 24) {
            return;
        }
        var Sized = (function (_super) {
            __extends(Sized, _super);
            function Sized() {
                return _super !== null && _super.apply(this, arguments) || this;
            }
            Sized.prototype.tryAdvance = function () { return false; };
            Sized.prototype.trySplit = function () { return null; };
            Sized.prototype.estimateSize = function () { return 9; };
            Sized.prototype.characteristics = function () { return Split.SIZED; };
            // The Java body would answer estimateSize(), 9.
            Sized.prototype.getExactSizeIfKnown = function () { return 11; };
            return Sized;
        }(Split));
        var Decorated = JavaProxy("com.tns.tests.defaults.DecoratedRuntimeSpliterator")(Sized);
        var decorated = new Decorated();

        expect(F.className(decorated)).toBe("com.tns.tests.defaults.DecoratedRuntimeSpliterator");
        expect(F.isRuntimeGenerated(decorated)).toBe(true);
        expect(F.spliteratorDefaults(decorated)).toBe("11|true|false");
        expect(F.getComparator(decorated)).toBe("java.lang.IllegalStateException");
    });

    it("are called through a named class extended at runtime from a class implementing the interface", function () {
        if (sdk < 24) {
            return;
        }
        var drains = [];
        var implementation = namedIterator(drains);
        implementation.interfaces = [It];
        var Named = java.lang.Object[ext]("com.tns.tests.defaults.NamedRuntimeObjectIterator", implementation);
        var named = new Named();

        expect(F.className(named)).toBe("com.tns.tests.defaults.NamedRuntimeObjectIterator");
        expect(F.isRuntimeGenerated(named)).toBe(true);
        expect(F.drain(named)).toBe("");
        expect(drains).toEqual(["forEachRemaining"]);
        expect(F.remove(named)).toBe("java.lang.UnsupportedOperationException");
    });

    it("are called through a named class the static binding generator prebuilt from a class implementing the interface", function () {
        if (sdk < 24) {
            return;
        }
        var drains = [];
        var Named = java.lang.Object.extend("com.tns.tests.defaults.NamedPrebuiltObjectIterator", {
            interfaces: [java.util.Iterator],
            hasNext: function () { return false; },
            next: function () { return null; },
            forEachRemaining: function () { drains.push("forEachRemaining"); }
        });
        var named = new Named();

        expect(F.className(named)).toBe("com.tns.tests.defaults.NamedPrebuiltObjectIterator");
        expect(F.isRuntimeGenerated(named)).toBe(false);
        expect(F.drain(named)).toBe("");
        expect(drains).toEqual(["forEachRemaining"]);
        expect(F.remove(named)).toBe("java.lang.UnsupportedOperationException");
    });

    it("do not run an accessor's getter while the class is first resolved", function () {
        if (sdk < 24) {
            return;
        }
        // No other spec implements this interface, so its class is resolved here.
        var DoublePred = java.util.function.DoublePredicate;
        var getterCalls = 0;
        var implementation = { test: function (value) { return value > 0; } };
        Object.defineProperty(implementation, "negate", {
            enumerable: true,
            get: function () {
                getterCalls++;
                return function () {
                    return new DoublePred({ test: function () { return true; } });
                };
            }
        });
        var predicate = new DoublePred(implementation);

        expect(getterCalls).toBe(0);
        expect(F.className(predicate)).toBe("com.tns.gen.java.util.function.DoublePredicate" + SUFFIX);
        expect(F.doubleNegate(predicate, 1)).toBe("true|true");
        expect(getterCalls).toBe(1);
    });

    it("construct an instance whose accessor's getter throws", function () {
        if (sdk < 24) {
            return;
        }
        // No other spec implements this interface, so its class is resolved here.
        var LongOperator = java.util.function.LongUnaryOperator;
        var getterCalls = 0;
        var implementation = { applyAsLong: function (value) { return value; } };
        Object.defineProperty(implementation, "andThen", {
            enumerable: true,
            get: function () {
                getterCalls++;
                throw new Error("getter must not run during construction");
            }
        });
        var operator = new LongOperator(implementation);

        expect(getterCalls).toBe(0);
        expect(F.className(operator)).toBe("com.tns.gen.java.util.function.LongUnaryOperator" + SUFFIX);
    });

    it("are called through an extend whose implementation object lists no interfaces", function () {
        if (sdk < 24) {
            return;
        }
        var BiPred = java.util.function.BiPredicate;
        var Same = BiPred[ext]({
            interfaces: [],
            test: function (first, second) { return first === second; },
            negate: function () {
                return new BiPred({ test: function () { return true; } });
            }
        });
        var same = new Same();

        expect(F.className(same)).toBe("com.tns.gen.java.util.function.BiPredicate" + SUFFIX);
        expect(F.biNegate(same, "a", "a")).toBe("true|true");
    });

    it("are called through an extend of a nested interface", function () {
        if (sdk < 24) {
            return;
        }
        var OfDouble = java.util.PrimitiveIterator.OfDouble;
        var removes = 0;
        var Removing = OfDouble[ext]({
            hasNext: function () { return false; },
            nextDouble: function () { return 0; },
            remove: function () { removes++; }
        });
        var removing = new Removing();

        expect(F.className(removing)).toBe("com.tns.gen.java.util.PrimitiveIterator_OfDouble" + SUFFIX);
        expect(F.remove(removing)).toBe("removed");
        expect(removes).toBe(1);
    });

    it("run a default declared in a super-interface the generated class cannot access", function () {
        if (sdk < 24) {
            return;
        }
        var H = com.tns.tests.interfaces.hiddendefaults;
        var Caller = H.VisibleDefaultsCaller;
        var Visible = H.VisibleDefaults;
        var calls = [];
        var visibleOnly = new Visible({
            name: function () { return "js"; },
            visible: function () {
                calls.push("visible");
                return "visible js";
            }
        });
        var hiddenOnly = new Visible({
            name: function () { return "js"; },
            hidden: function () {
                calls.push("hidden");
                return "hidden js";
            }
        });

        if (Caller.isDesugared()) {
            // Below minSdk 24 they are abstract on the device and always reach JS.
            expect(Caller.visible(visibleOnly)).toBe("visible js");
            expect(Caller.hidden(hiddenOnly)).toBe("hidden js");
            expect(calls).toEqual(["visible", "hidden"]);
            return;
        }

        expect(Caller.visible(visibleOnly)).toBe("visible js");
        expect(Caller.hidden(visibleOnly)).toBe("hidden java");
        expect(Caller.narrowed(visibleOnly)).toBe("visible narrowed");
        expect(Caller.hidden(hiddenOnly)).toBe("hidden js");
        expect(Caller.visible(hiddenOnly)).toBe("visible java");
        expect(calls).toEqual(["visible", "hidden"]);
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

    it("run the Java bodies of defaults of every argument and return kind", function () {
        if (sdk < 24) {
            return;
        }
        var D = com.tns.tests.interfaces.defaultshapes;
        if (D.ShapesCaller.isDesugared()) {
            // Below minSdk 24 every method is abstract on the device.
            return;
        }
        var odd = [];
        var shapes = new D.Shapes({
            name: function () { return "js"; },
            odd: function (value) {
                odd.push(value);
                return false;
            }
        });

        expect(F.className(shapes)).toBe("com.tns.gen.com.tns.tests.interfaces.defaultshapes.Shapes" + SUFFIX);
        expect(D.ShapesCaller.all(shapes)).toBe("7028|2.5|0.5|Q|false|-4|42|js:9223372036854775807:-0.25|v");
        expect(odd).toEqual([3]);
        expect(D.ShapesCaller.fail(shapes)).toBe("com.tns.tests.interfaces.defaultshapes.ShapesException: from java");

        // A call of the erased same(Object) runs javac's bridge, which reaches the narrowed default.
        var strings = new D.StringShapes({
            name: function () { return "s"; },
            odd: function () { return true; }
        });
        expect(D.ShapesCaller.all(strings)).toBe("7028|2.5|0.5|Q|true|-4|42|s:9223372036854775807:-0.25|string v");
    });
});

// The class a process uses for every interface when its invoke-super probe fails, requested
// here for interfaces no other spec uses, before their classes are first resolved.
describe("Interface default methods run through the runtime", function () {
    var sdk = android.os.Build.VERSION.SDK_INT;
    var F = com.tns.tests.DefaultMethodFixtures;
    var SUFFIX = "-defaults-jni";
    var DoubleOp = java.util.function.DoubleUnaryOperator;

    it("run a default the JS object lacks on the calling thread, and one it implements in JS", function () {
        if (sdk < 24) {
            return;
        }
        F.useRuntimeCallStubs(DoubleOp.class);
        var composed = [];
        var op = new DoubleOp({
            applyAsDouble: function (value) { return value * 3; },
            compose: function (before) {
                composed.push(before !== null);
                return null;
            }
        });

        expect(F.className(op)).toBe("com.tns.gen.java.util.function.DoubleUnaryOperator" + SUFFIX);
        // This thread waits inside the fixture, so a call that needed it would time out.
        expect(F.doubleAndThenOnBackgroundThread(op, 5000)).toBe(F.CALLER_THREAD + "|true");
        expect(F.doubleAndThenApply(op, 2)).toBe("6.0");
        expect(F.doubleCompose(op)).toBe("null");
        expect(composed).toEqual([true]);
    }, 15000);

    it("pass a wide argument from a default's Java body back to JS", function () {
        if (sdk < 26) {
            return;
        }
        var TemporalType = java.time.temporal.Temporal;
        F.useRuntimeCallStubs(TemporalType.class);
        var added = [];
        var temporal = new TemporalType({
            plus: function (amount, unit) {
                added.push(amount + " " + unit);
                return this;
            }
        });

        expect(F.className(temporal)).toBe("com.tns.gen.java.time.temporal.Temporal" + SUFFIX);
        expect(com.tns.tests.TemporalDefaultFixtures.minusDays(temporal, 5)).toBe(true);
        expect(added).toEqual(["-5 Days"]);
    });

    it("run defaults of wide, generic and bridged signatures through the runtime", function () {
        if (sdk < 24) {
            return;
        }
        var R = com.tns.tests.interfaces.runtimecalldefaults;
        if (R.RuntimeCallCaller.isDesugared()) {
            // Below minSdk 24 every method is abstract on the device.
            return;
        }
        F.useRuntimeCallStubs(R.RuntimeCallShapes.class);
        F.useRuntimeCallStubs(R.RuntimeCallStringShapes.class);
        var odd = [];
        var shapes = new R.RuntimeCallShapes({
            name: function () { return "js"; },
            odd: function (value) {
                odd.push(value);
                return false;
            }
        });

        expect(F.className(shapes)).toBe("com.tns.gen.com.tns.tests.interfaces.runtimecalldefaults.RuntimeCallShapes" + SUFFIX);
        expect(R.RuntimeCallCaller.all(shapes)).toBe("7028|2.5|false|v");
        expect(odd).toEqual([3]);
        expect(R.RuntimeCallCaller.wideOnBackgroundThread(shapes, 5000)).toBe(F.CALLER_THREAD + "|9223372036854774996");

        // A call of the erased same(Object) runs javac's bridge, which reaches the narrowed default.
        var strings = new R.RuntimeCallStringShapes({
            name: function () { return "s"; },
            odd: function () { return true; }
        });
        expect(F.className(strings)).toBe("com.tns.gen.com.tns.tests.interfaces.runtimecalldefaults.RuntimeCallStringShapes" + SUFFIX);
        expect(R.RuntimeCallCaller.all(strings)).toBe("7028|2.5|true|string v");
    }, 15000);

    it("run a default declared in a non-public super-interface through the runtime", function () {
        if (sdk < 24) {
            return;
        }
        var R = com.tns.tests.interfaces.runtimecalldefaults;
        if (R.RuntimeCallCaller.isDesugared()) {
            return;
        }
        F.useRuntimeCallStubs(R.RuntimeCallVisible.class);
        var visibleOnly = new R.RuntimeCallVisible({
            name: function () { return "js"; },
            visible: function () { return "visible js"; }
        });

        expect(F.className(visibleOnly)).toBe("com.tns.gen.com.tns.tests.interfaces.runtimecalldefaults.RuntimeCallVisible" + SUFFIX);
        expect(R.RuntimeCallCaller.visible(visibleOnly)).toBe("visible js");
        expect(R.RuntimeCallCaller.hidden(visibleOnly)).toBe("hidden java");
    });
});
