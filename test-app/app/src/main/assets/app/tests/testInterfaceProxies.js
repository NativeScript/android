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

    it("runs a platform default method's Java body, even when JS defines it", function () {
        if (sdk < 24) {
            return;
        }
        var D = com.tns.tests.InterfaceProxyDefaultMethodFixtures;
        var Iterator = hidden(java.util, "Iter" + "ator");
        var jsRemoveCalls = 0;
        function iterator() {
            var items = ["a", "b", "c"];
            var index = 0;
            return new Iterator({
                hasNext: function () { return index < items.length; },
                next: function () { return items[index++]; },
                remove: function () { jsRemoveCalls++; }
            });
        }

        var fromJava = iterator();
        expectRuntimeClass(fromJava);
        expect(String(D.drain(fromJava))).toBe("a,b,c");
        expect(String(D.remove(fromJava))).toBe("java.lang.UnsupportedOperationException");
        expect(jsRemoveCalls).toBe(0);

        // JS calling a default the implementation lacks gets the Java body too.
        var fromJs = iterator();
        var seen = [];
        fromJs.forEachRemaining(new java.util.function.Consumer({
            accept: function (item) { seen.push(item); }
        }));
        expect(seen).toEqual(["a", "b", "c"]);

        var Predicate = hidden(java.util["func" + "tion"], "Pred" + "icate");
        var predicate = new Predicate({
            test: function (value) { return value === "yes"; },
            negate: function () { throw new Error("JS negate must not run"); }
        });
        expect(String(D.negate(predicate, "yes"))).toBe("true|false");

        // Primitive arguments and results of a default method.
        var Spliterator = hidden(java.util, "Split" + "erator");
        var spliterator = new Spliterator({
            tryAdvance: function () { return false; },
            trySplit: function () { return null; },
            estimateSize: function () { return 3; },
            characteristics: function () { return Spliterator.SIZED; },
            getExactSizeIfKnown: function () { throw new Error("JS getExactSizeIfKnown must not run"); }
        });
        expect(String(D.spliteratorDefaults(spliterator))).toBe("3|true|false");

        // OfInt.next() is a default overriding the abstract Iterator.next(), and its
        // forEachRemaining(Consumer) overrides Iterator's default.
        var OfInt = hidden(java.util.PrimitiveIterator, "Of" + "Int");
        var value = 0;
        var ofInt = new OfInt({
            hasNext: function () { return value < 3; },
            nextInt: function () { return ++value; },
            next: function () { throw new Error("JS next must not run"); }
        });
        expectRuntimeClass(ofInt);
        expect(String(D.next(ofInt))).toBe("1");
        expect(String(D.drain(ofInt))).toBe("2,3");
    });

    it("sends a default method compiled for minSdk 21 to JS like an abstract one", function () {
        // Desugared: at runtime Producer.produce() is abstract and its body lives elsewhere.
        var Producer = hidden(com.tns.tests.interfaces.defaultmethods.contract, "Pro" + "ducer");

        expect(String(F.produce(new Producer({ produce: function () { return "from js"; } })))).toBe("from js");
        expect(String(F.produce(new Producer({})))).toBe("com.tns.NativeScriptException");
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

        var generatedInterfaces = lines.filter(function (line) {
            var fields = line.split(" ");
            return fields[0] !== "proxy" && fields[1] === "interface";
        });
        // Named extends (Interface.extend("a.b.C", {...})) keep their own class, and so does
        // the interface the spec above makes incompatible on purpose.
        var unexpected = generatedInterfaces.filter(function (line) {
            var requested = line.split(" ")[3];
            return requested.indexOf("com.tns.gen.") === 0 && requested.indexOf("InterfaceProxyFixtures$ProxyIncompatible") < 0;
        });
        expect(unexpected).toEqual([]);
    });
});
