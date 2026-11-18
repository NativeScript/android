// Interface implementations whose class is not in the APK: a class generated at runtime,
// or a java.lang.reflect.Proxy with the app's android.interfaceProxies option. Both must
// behave the same way when Java calls them.
describe("Interface implementations without a prebuilt class", function () {
    var F = com.tns.tests.InterfaceProxyFixtures;
    var config = require("../package.json").android;
    var proxies = !!config.interfaceProxies;
    var multithreaded = !!config.enableMultithreadedJavascript;
    var sdk = android.os.Build.VERSION.SDK_INT;

    // Computed members keep the static binding generator from prebuilding these classes.
    var ext = "ex" + "tend";
    function hidden(holder, name) {
        return holder[name];
    }

    var Values = hidden(F, "Val" + "ues");
    var Identity = hidden(F, "Iden" + "tity");

    function expectRuntimeClass(instance) {
        var cls = instance.getClass();
        expect(java.lang.reflect.Proxy.isProxyClass(cls)).toBe(proxies);
        if (!proxies) {
            expect(String(cls.getName())).toMatch(/^com\.tns\.gen\./);
        }
    }

    function reportEntriesFor(loadedName) {
        var lines = com.tns.DexFactory.getRuntimeGeneratedClasses();
        var result = [];
        for (var i = 0; i < lines.length; i++) {
            var fields = String(lines[i]).split(" ");
            if (fields[2] === String(loadedName)) {
                result.push({ source: fields[0], kind: fields[1], loaded: fields[2], requested: fields[3] });
            }
        }
        return result;
    }

    function collectJava() {
        for (var i = 0; i < 3; i++) {
            java.lang.Runtime.getRuntime().gc();
            java.lang.System.runFinalization();
        }
    }

    it("is a class of the expected kind and listed in the runtime generation report", function () {
        var instance = new Identity({
            name: function () {
                return "kind";
            }
        });

        expectRuntimeClass(instance);
        var entries = reportEntriesFor(instance.getClass().getName());
        expect(entries.length).toBe(1);
        expect(entries[0].kind).toBe("interface");
        if (proxies) {
            expect(entries[0].source).toBe("proxy");
        } else {
            expect(["generated", "cached"]).toContain(entries[0].source);
        }
    });

    it("passes every argument and return type between Java and JS", function () {
        var ran = false;
        var values = new Values({
            booleanValue: function (v) { return v; },
            byteValue: function (v) { return v; },
            charValue: function (v) { return v; },
            shortValue: function (v) { return v; },
            intValue: function (v) { return v; },
            longValue: function (v) { return v; },
            floatValue: function (v) { return v; },
            doubleValue: function (v) { return v; },
            run: function () { ran = true; },
            stringValue: function (v) { return v; },
            objectValue: function (v) { return v; },
            intArray: function (v) { return v; },
            join: function (parts) {
                var result = [];
                for (var i = 0; i < parts.length; i++) {
                    result.push(parts[i]);
                }
                return result.join("+");
            }
        });

        expectRuntimeClass(values);
        expect(String(F.callAllValues(values))).toBe("true|-5|x|-300|70000|1099511627776|1.5|-2.25|str|obj|3|a+b+c");
        expect(ran).toBe(true);
    });

    it("gives Java the default value for primitives and null for objects when JS returns nothing", function () {
        function nothing() {}
        var values = new Values({
            booleanValue: nothing, byteValue: nothing, charValue: nothing, shortValue: nothing,
            intValue: nothing, longValue: nothing, floatValue: nothing, doubleValue: nothing,
            run: nothing, stringValue: function () { return null; }, objectValue: nothing,
            intArray: function () { return null; }, join: nothing
        });

        expect(String(F.callDefaultedValues(values))).toBe("false|0|0|0|0|0|0.0|0.0|null|null|null");
    });

    it("converts JS return values the way a prebuilt class does", function () {
        var current;
        function implementation() {
            return {
                z: function () { return current(); },
                b: function () { return current(); },
                c: function () { return current(); },
                s: function () { return current(); },
                i: function () { return current(); },
                j: function () { return current(); },
                f: function () { return current(); },
                d: function () { return current(); }
            };
        }
        var prebuilt = new com.tns.tests.PrimitiveReturnTest.Returns(implementation());
        var Hidden = hidden(F, "PrimitiveRe" + "turns");
        var runtime = new Hidden(implementation());
        expectRuntimeClass(runtime);

        var values = [
            function () { return 1; }, function () { return -1.5; }, function () { return 2147483648; },
            function () { return 1e-40; }, function () { return NaN; }, function () { return true; },
            function () { return "a"; }, function () { return null; }, function () { return undefined; },
            function () { return new Number(5); }, function () { return long(5); }, function () { return byte(2); },
            function () { return short(3); }, function () { return char("a"); }, function () { return float(1.5); },
            function () { return double(2); }, function () { return {}; }
        ];
        var kinds = ["z", "b", "c", "s", "i", "j", "f", "d"];
        values.forEach(function (value) {
            current = value;
            kinds.forEach(function (kind) {
                var expected = String(com.tns.tests.PrimitiveReturnTest.call(prebuilt, kind));
                var actual = String(com.tns.tests.PrimitiveReturnTest.call(runtime, kind));
                expect(value.toString() + " " + kind + ": " + actual).toBe(value.toString() + " " + kind + ": " + expected);
            });
        });
    });

    it("throws to Java what the JS method throws", function () {
        var values = new Values({
            stringValue: function () {
                throw new Error("thrown by the implementation");
            }
        });

        var result = String(F.callStringValue(values, "x"));
        expect(result).toMatch(/^com\.tns\.NativeScriptException: /);
        expect(result).toContain("thrown by the implementation");
    });

    it("fails a call to an abstract method the JS implementation lacks", function () {
        var values = new Values({});

        var result = String(F.callRun(values));
        expect(result).toMatch(/^com\.tns\.NativeScriptException: /);
        expect(result).toContain("java.lang.AbstractMethodError");
    });

    it("gives equals, hashCode and toString identity semantics, as Java sees them", function () {
        var calledFromJava = [];
        var implementation = {
            name: function () { return "identity"; },
            equals: function () { calledFromJava.push("equals"); return false; },
            hashCode: function () { calledFromJava.push("hashCode"); return 1; },
            toString: function () { calledFromJava.push("toString"); return "js"; }
        };
        var first = new Identity(implementation);
        var second = new Identity({ name: function () { return "second"; } });

        expect(F.hashCodeIsIdentity(first)).toBe(true);
        expect(F.equalsIsIdentity(first)).toBe(true);
        expect(F.toStringIsIdentity(first)).toBe(true);
        expect(F.worksAsHashMapKey(first, second)).toBe(true);
        expect(calledFromJava).toEqual([]);
    });

    it("hands JS the same object back when Java returns the instance", function () {
        var instance = new Identity({ name: function () { return "echo"; } });

        expect(F.echo(instance) === instance).toBe(true);
        expect(F.echo(F.echo(instance)) === instance).toBe(true);
    });

    it("implements an interface that extends another", function () {
        var Derived = hidden(F, "Der" + "ived");
        var instance = new Derived({
            base: function () { return "base"; },
            derived: function () { return "derived"; }
        });

        expectRuntimeClass(instance);
        expect(String(F.callDerived(instance))).toBe("base|derived");
    });

    it("implements a generic interface", function () {
        var Generic = hidden(F, "Gen" + "eric");
        var instance = new Generic({
            apply: function (value) { return value + "!"; }
        });

        expectRuntimeClass(instance);
        expect(String(F.applyGeneric(instance, "generic"))).toBe("generic!");
    });

    it("implements a Kotlin function type", function () {
        var Function1 = hidden(kotlin.jvm.functions, "Func" + "tion1");
        var instance = new Function1({
            invoke: function (value) { return "kotlin " + value; }
        });

        expectRuntimeClass(instance);
        expect(String(F.invokeFunction1(instance, "lambda"))).toBe("kotlin lambda");
    });

    it("keeps each extend's implementation when one interface is extended twice", function () {
        var First = Identity[ext]({ name: function () { return "first"; } });
        var Second = Identity[ext]({ name: function () { return "second"; } });
        var first = new First();
        var second = new Second();

        expect(Second).not.toBe(First);
        expectRuntimeClass(first);
        expect(String(second.getClass().getName())).toBe(String(first.getClass().getName()));
        expect(String(second.getClass().getMethod("name", []).invoke(second, []))).toBe("second");
        expect(String(first.getClass().getMethod("name", []).invoke(first, []))).toBe("first");
    });

    it("calls the JS implementations of platform default methods, and the Java bodies of the others", function () {
        if (sdk < 24) {
            return;
        }
        var D = com.tns.tests.InterfaceProxyDefaultMethodFixtures;
        var Iterator = hidden(java.util, "Iter" + "ator");
        var jsRemoveCalls = 0;
        function iterator(withRemove) {
            var items = ["a", "b", "c"];
            var index = 0;
            var implementation = {
                hasNext: function () { return index < items.length; },
                next: function () { return items[index++]; }
            };
            if (withRemove) {
                implementation.remove = function () { jsRemoveCalls++; };
            }
            return new Iterator(implementation);
        }

        var fromJava = iterator(true);
        expectRuntimeClass(fromJava);
        expect(String(D.remove(fromJava))).toBe("removed");
        expect(jsRemoveCalls).toBe(1);
        // forEachRemaining is not implemented in JS: its Java body calls hasNext and next.
        expect(String(D.drain(fromJava))).toBe("a,b,c");

        var withoutRemove = iterator(false);
        expectRuntimeClass(withoutRemove);
        expect(String(D.remove(withoutRemove))).toBe("java.lang.UnsupportedOperationException");
        expect(jsRemoveCalls).toBe(1);

        // JS calling a default the implementation lacks gets the Java body too.
        var fromJs = iterator(true);
        var seen = [];
        fromJs.forEachRemaining(new java.util.function.Consumer({
            accept: function (item) { seen.push(item); }
        }));
        expect(seen).toEqual(["a", "b", "c"]);

        var Predicate = hidden(java.util["func" + "tion"], "Pred" + "icate");
        var negations = 0;
        var predicate = new Predicate({
            test: function (value) { return value === "yes"; },
            negate: function () {
                negations++;
                return new Predicate({ test: function () { return true; } });
            }
        });
        expect(String(D.negate(predicate, "yes"))).toBe("true|true");
        expect(negations).toBe(1);

        // Primitive arguments and results of default methods: getExactSizeIfKnown runs in JS,
        // hasCharacteristics runs its Java body, which calls characteristics in JS.
        var Spliterator = hidden(java.util, "Split" + "erator");
        var spliterator = new Spliterator({
            tryAdvance: function () { return false; },
            trySplit: function () { return null; },
            estimateSize: function () { return 3; },
            characteristics: function () { return Spliterator.SIZED; },
            getExactSizeIfKnown: function () { return 42; }
        });
        expect(String(D.spliteratorDefaults(spliterator))).toBe("42|true|false");

        // OfInt.next() is a default narrowing the abstract Iterator.next(), and its
        // forEachRemaining(Consumer) overrides Iterator's default.
        var OfInt = hidden(java.util.PrimitiveIterator, "Of" + "Int");
        var value = 0;
        var javaNext = new OfInt({
            hasNext: function () { return value < 3; },
            nextInt: function () { return ++value; }
        });
        expectRuntimeClass(javaNext);
        expect(String(D.next(javaNext))).toBe("1");
        expect(String(D.drain(javaNext))).toBe("2,3");

        var jsNext = new OfInt({
            hasNext: function () { return false; },
            nextInt: function () { throw new Error("JS nextInt must not run"); },
            next: function () { return new java.lang.Integer(7); }
        });
        expectRuntimeClass(jsNext);
        expect(String(D.next(jsNext))).toBe("7");
    });

    it("calls the JS implementation of an app's default method, which D8 may have made abstract", function () {
        var Producer = hidden(com.tns.tests.interfaces.defaultmethods.contract, "Pro" + "ducer");
        // Below minSdk 24 D8 desugars the default: at runtime produce() is abstract and its body
        // lives elsewhere, so an implementation that lacks it fails the call.
        var desugared = sdk < 24 || !Producer.class.getMethod("produce", []).isDefault();

        expect(String(F.produce(new Producer({ produce: function () { return "from js"; } })))).toBe("from js");
        expect(String(F.produce(new Producer({})))).toBe(desugared ? "com.tns.NativeScriptException" : "default produce in Producer");
    });

    it("generates a class for an interface a proxy cannot implement", function () {
        var Incompatible = hidden(F, "ProxyIncom" + "patible");
        var instance = new Incompatible({
            describe: function () { return "generated"; }
        });

        expect(String(F.describe(instance))).toBe("generated");
        expect(java.lang.reflect.Proxy.isProxyClass(instance.getClass())).toBe(false);
        expect(String(instance.getClass().getName())).toBe("com.tns.gen.com.tns.tests.InterfaceProxyFixtures_ProxyIncompatible");
        var entries = reportEntriesFor(instance.getClass().getName());
        expect(entries.length).toBe(1);
        expect(["generated", "cached"]).toContain(entries[0].source);
    });

    it("generates the shared class for an anonymous extend of an interface a proxy cannot implement", function () {
        var Incompatible = hidden(F, "ProxyIncompatible" + "Extended");
        var Extended = Incompatible[ext]({
            describe: function () { return "extended"; }
        });
        var instance = new Extended();

        expect(String(F.describe(instance))).toBe("extended");
        expect(java.lang.reflect.Proxy.isProxyClass(instance.getClass())).toBe(false);
        expect(String(instance.getClass().getName())).toBe("com.tns.gen.com.tns.tests.InterfaceProxyFixtures_ProxyIncompatibleExtended");
        var entries = reportEntriesFor(instance.getClass().getName());
        expect(entries.length).toBe(1);
        expect(entries[0].requested).not.toBe(entries[0].loaded);
    });

    it("routes a call from another Java thread to the runtime that created the instance", function (done) {
        var Reporter = hidden(F, "Thread" + "Reporter");
        var reporter = new Reporter({
            report: function (caller) {
                return caller + ">" + java.lang.Thread.currentThread().getName();
            }
        });
        expectRuntimeClass(reporter);

        F.startReportOnNewThread(reporter);
        var deadline = Date.now() + 10000;
        (function poll() {
            var report = F.threadReport();
            if (report === null && Date.now() < deadline) {
                setTimeout(poll, 10);
                return;
            }
            // With multithreaded JS the call runs on the calling thread under the isolate lock;
            // without it, it is posted to this runtime's thread.
            expect(String(report)).toBe("InterfaceProxyFixtures-caller>" + (multithreaded ? "InterfaceProxyFixtures-caller" : "main"));
            done();
        })();
    }, 20000);

    it("works in a worker, and from another runtime that wraps the worker's instance", function (done) {
        var Holder = com.tns.tests.RuntimeBoundHolder;
        var worker = new Worker("./interfaceProxyWorker.js");
        worker.onmessage = function (msg) {
            var data = msg.data;
            expect(data.error).toBeUndefined();
            expect(data.isProxy).toBe(proxies);
            expect(data.fromJava).toBe("worker:" + data.thread);

            var target = Holder.get("interfaceProxyWorkerTarget");
            var expected = "worker:" + (multithreaded ? "main" : data.thread);
            expect(String(F.describeWorkerTarget(target))).toBe(expected);
            expect(String(target.describe())).toBe(expected);

            Holder.clear();
            worker.terminate();
            done();
        };
    });

    it("lets Java collect an instance neither JS nor Java references", function (done) {
        var ref = (function () {
            return F.weakly(new Identity({ name: function () { return "collectable"; } }));
        })();

        var deadline = Date.now() + 10000;
        (function poll() {
            gc();
            collectJava();
            if (F.isCleared(ref) || Date.now() > deadline) {
                expect(F.isCleared(ref)).toBe(true);
                done();
                return;
            }
            setTimeout(poll, 20);
        })();
    }, 20000);

    it("keeps the JS implementation of an instance only Java references", function () {
        (function () {
            var state = "kept";
            F.hold(new Identity({ name: function () { return state; } }));
        })();

        for (var i = 0; i < 3; i++) {
            gc();
            collectJava();
        }

        expect(String(F.callHeld())).toBe("kept");
        F.release();
    });
});

// Runs after the other specs in the suite, so it sees what all of them resolved.
describe("Runtime generation report under the interfaceProxies option", function () {
    it("lists no class generated for an interface that a proxy could implement", function () {
        var lines = Array.prototype.map.call(com.tns.DexFactory.getRuntimeGeneratedClasses(), String);
        android.util.Log.i("JS", "Runtime generation report:\n" + lines.join("\n"));
        if (!require("../package.json").android.interfaceProxies) {
            return;
        }

        var prefix = "com.tns.gen.";
        var defaultsSuffix = "-defaults";
        var appClassLoader = com.tns.Runtime.class.getClassLoader();
        function loadClass(name) {
            try {
                return java.lang.Class.forName(name, false, appClassLoader);
            } catch (e) {
                return null;
            }
        }
        // The same proxy class DexFactory.getInterfaceProxyClass asks for.
        var runtimeBound = loadClass("com.tns.NativeScriptRuntimeBound");
        function canProxy(iface) {
            var loader = iface.getClassLoader().equals(java.lang.Object.class.getClassLoader()) ? runtimeBound.getClassLoader() : iface.getClassLoader();
            var types = Array.create(java.lang.Class, 2);
            types[0] = iface;
            types[1] = runtimeBound;
            try {
                java.lang.reflect.Proxy.getProxyClass(loader, types);
                return true;
            } catch (e) {
                return false;
            }
        }
        // An interface's shared class is com.tns.gen. and its binary name with `$` as `_`. The
        // class itself is not looked up: one a worker generated is in no loader JS can name.
        function interfaceOfSharedClass(loaded) {
            var name = loaded.substring(prefix.length);
            var simpleStart = name.lastIndexOf(".") + 1;
            var parts = name.substring(simpleStart).split("_");
            var separators = parts.length - 1;
            if (separators > 10) {
                return null;
            }
            for (var mask = 0; mask < (1 << separators); mask++) {
                var candidate = name.substring(0, simpleStart) + parts[0];
                for (var i = 1; i < parts.length; i++) {
                    candidate += ((mask >> (i - 1)) & 1 ? "$" : "_") + parts[i];
                }
                var iface = loadClass(candidate);
                if (iface !== null && iface.isInterface()) {
                    return iface;
                }
            }
            return null;
        }

        var generatedInterfaces = lines.filter(function (line) {
            var fields = line.split(" ");
            return fields[0] !== "proxy" && fields[1] === "interface";
        });
        var unexpected = generatedInterfaces.filter(function (line) {
            var fields = line.split(" ");
            var loaded = fields[2];
            var requested = fields[3];
            // A named extend, Interface.extend("a.b.C", {...}), asks for the class it names.
            if (requested.indexOf(prefix) !== 0) {
                return false;
            }
            var name = requested.substring(prefix.length);
            if (name.slice(-defaultsSuffix.length) === defaultsSuffix) {
                name = name.slice(0, -defaultsSuffix.length);
            }
            var iface = loadClass(name);
            // An extend that lists extra interfaces asks for, and loads, a class named after
            // its call site, which no proxy of the one interface stands in for. The call-site
            // name of an extend without them names no Java type either, but loads the
            // interface's shared class, which a proxy replaces when it can.
            if (iface === null) {
                if (loaded === requested.replace(/\$/g, "_")) {
                    return false;
                }
                var shared = loaded.indexOf(prefix) === 0 ? interfaceOfSharedClass(loaded) : null;
                return shared === null || canProxy(shared);
            }
            // Requested by the interface's name: its shared or default-overriding class.
            return canProxy(iface);
        });
        expect(unexpected).toEqual([]);
    });
});
