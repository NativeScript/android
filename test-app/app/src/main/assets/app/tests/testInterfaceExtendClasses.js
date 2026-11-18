describe("Anonymous extends of an interface", function () {
    var ext = "ex" + "tend";
    // With this option an interface without a prebuilt class gets a java.lang.reflect.Proxy class.
    var proxies = !!require("../package.json").android.interfaceProxies;

    // Reflection makes Java, not JS, call the implementation.
    function callFromJava(instance, methodName) {
        return instance.getClass().getMethod(methodName, []).invoke(instance, []);
    }

    function reportEntriesFor(loadedName) {
        var lines = com.tns.DexFactory.getRuntimeGeneratedClasses();
        var result = [];
        for (var i = 0; i < lines.length; i++) {
            var fields = String(lines[i]).split(" ");
            if (fields[2] === loadedName) {
                result.push({ source: fields[0], kind: fields[1], loaded: fields[2], requested: fields[3] });
            }
        }
        return result;
    }

    it("load the interface's class from the APK when it is prebuilt", function () {
        var Extended = java.lang.Runnable.extend({
            run: function () {}
        });
        var instance = new Extended();

        expect(instance.getClass().getName()).toBe("com.tns.gen.java.lang.Runnable");
        expect(reportEntriesFor("com.tns.gen.java.lang.Runnable")).toEqual([]);
    });

    it("load the prebuilt class a name under com.tns.gen asks for", function () {
        var calls = [];
        var Extended = java.lang.Runnable.extend("com.tns.gen.tests.ExplicitlyNamedRunnable", {
            run: function () {
                calls.push("explicit");
            }
        });
        var instance = new Extended();

        callFromJava(instance, "run");
        expect(calls).toEqual(["explicit"]);
        expect(instance.getClass().getName()).toBe("com.tns.gen.tests.ExplicitlyNamedRunnable");
        expect(reportEntriesFor("com.tns.gen.tests.ExplicitlyNamedRunnable")).toEqual([]);
    });

    it("keep each call site's implementation when they extend the same interface", function () {
        var calls = [];
        var First = java.lang.Runnable.extend({
            run: function () {
                calls.push("first");
            }
        });
        var Second = java.lang.Runnable.extend({
            run: function () {
                calls.push("second");
            }
        });

        expect(Second).not.toBe(First);
        callFromJava(new Second(), "run");
        callFromJava(new First(), "run");
        expect(calls).toEqual(["second", "first"]);
    });

    it("keep each call site's implementation when they both override a default method of the same interface", function () {
        if (android.os.Build.VERSION.SDK_INT < 24) {
            return;
        }
        // An alias: no class of the interface is prebuilt, and no other spec implements it.
        var Operator = java.util.function["IntUnary" + "Operator"];
        var calls = [];
        var First = Operator[ext]({
            applyAsInt: function (value) { return value + 1; },
            andThen: function (after) {
                calls.push("first andThen");
                return after;
            }
        });
        var Second = Operator[ext]({
            applyAsInt: function (value) { return value * 10; },
            andThen: function (after) {
                calls.push("second andThen");
                return after;
            }
        });
        expect(Second).not.toBe(First);

        // Reflection makes Java, not JS, call the implementations.
        var applyAsInt = Operator.class.getMethod("applyAsInt", [java.lang.Integer.TYPE]);
        var andThen = Operator.class.getMethod("andThen", [Operator.class]);
        function apply(instance, value) {
            return String(applyAsInt.invoke(instance, [new java.lang.Integer(value)]));
        }

        var second = new Second();
        var first = new First();
        expect(apply(first, 4)).toBe("5");
        expect(apply(second, 4)).toBe("40");
        andThen.invoke(second, [first]);
        andThen.invoke(first, [second]);
        expect(calls).toEqual(["second andThen", "first andThen"]);

        var firstAgain = new First();
        var secondAgain = new Second();
        expect(apply(secondAgain, 2)).toBe("20");
        expect(apply(firstAgain, 2)).toBe("3");
        andThen.invoke(firstAgain, [secondAgain]);
        expect(calls).toEqual(["second andThen", "first andThen", "first andThen"]);
        expect(String(second.getClass().getName())).toBe(String(first.getClass().getName()));
    });

    it("return the same constructor each time one call site runs", function () {
        function extendRunnable(implementation) {
            return java.lang.Runnable.extend(implementation);
        }
        var calls = [];
        var First = extendRunnable({
            run: function () {
                calls.push("first");
            }
        });
        var Again = extendRunnable({
            run: function () {
                calls.push("again");
            }
        });

        expect(Again).toBe(First);
        callFromJava(new Again(), "run");
        expect(calls).toEqual(["first"]);
    });

    it("give call sites that pass the same implementation object the first one's constructor", function () {
        // No other spec extends this interface, whose first extend in the process decides
        // which constructor the call sites below share.
        var Filter = java.io.FilenameFilter;
        var calls = [];
        var implementation = {
            accept: function (dir, name) {
                calls.push(String(name));
                return true;
            }
        };
        var First = Filter.extend(implementation);
        var Second = Filter.extend(implementation);
        var Named = Filter.extend("SharedImplementationFilter", implementation);

        expect(Second).toBe(First);
        expect(Named).toBe(First);
        var accept = Filter.class.getMethod("accept", [java.io.File.class, java.lang.String.class]);
        accept.invoke(new Second(), [null, "second"]);
        accept.invoke(new Named(), [null, "named"]);
        accept.invoke(new First(), [null, "first"]);
        expect(calls).toEqual(["second", "named", "first"]);
    });

    it("refuse one implementation object for extends of two interfaces", function () {
        var implementation = {
            accept: function () {
                return true;
            },
            newThread: function () {
                return null;
            }
        };
        java.io.FileFilter.extend(implementation);

        expect(function () {
            java.util.concurrent.ThreadFactory.extend(implementation);
        }).toThrowError(/This object is used to extend another class/);
    });

    it("work with a generated class after new Interface({...}) of the same interface", function () {
        // Computed members hide both uses from the static binding generator.
        var Action = java.security.PrivilegedAction;
        var implemented = new Action({
            run: function () {
                return "implemented";
            }
        });
        var Extended = Action[ext]({
            run: function () {
                return "extended";
            }
        });
        var extended = new Extended();

        expect(String(callFromJava(implemented, "run"))).toBe("implemented");
        expect(String(callFromJava(extended, "run"))).toBe("extended");
        expect(String(extended.getClass().getName())).toBe(String(implemented.getClass().getName()));
        expect(reportEntriesFor(String(implemented.getClass().getName())).length).toBe(1);
    });

    it("work with a generated class before new Interface({...}) of the same interface", function () {
        var Closeable = java.lang.AutoCloseable;
        var calls = [];
        var Extended = Closeable[ext]({
            close: function () {
                calls.push("extended");
            }
        });
        var extended = new Extended();
        var implemented = new Closeable({
            close: function () {
                calls.push("implemented");
            }
        });

        callFromJava(extended, "close");
        callFromJava(implemented, "close");
        expect(calls).toEqual(["extended", "implemented"]);
        expect(String(implemented.getClass().getName())).toBe(String(extended.getClass().getName()));
        expect(reportEntriesFor(String(implemented.getClass().getName())).length).toBe(1);
    });

    it("keep each implementation when new and extend of a prebuilt interface are mixed", function () {
        var calls = [];
        var implementedBefore = new java.lang.Runnable({
            run: function () {
                calls.push("new before");
            }
        });
        var Extended = java.lang.Runnable.extend({
            run: function () {
                calls.push("extend");
            }
        });
        var extended = new Extended();
        var implementedAfter = new java.lang.Runnable({
            run: function () {
                calls.push("new after");
            }
        });

        callFromJava(implementedAfter, "run");
        callFromJava(extended, "run");
        callFromJava(implementedBefore, "run");
        expect(calls).toEqual(["new after", "extend", "new before"]);
        expect(extended.getClass().getName()).toBe("com.tns.gen.java.lang.Runnable");
        expect(implementedAfter.getClass().getName()).toBe("com.tns.gen.java.lang.Runnable");
    });

    it("load the generated class of a nested interface", function () {
        var calledF = false;
        var Extended = com.tns.tests.ImplementInterfaceTest.I3.extend({
            f: function () {
                calledF = true;
            },
            g: function () {}
        });
        var instance = new Extended();

        com.tns.tests.ImplementInterfaceTest.triggerF(instance);
        expect(calledF).toBe(true);
        expect(instance.getClass().getName()).toBe("com.tns.gen.com.tns.tests.ImplementInterfaceTest_I3");
    });

    it("work with a class generated in a worker", function (done) {
        var worker = new Worker("./interfaceExtendWorker.js");
        worker.onmessage = function (msg) {
            worker.terminate();
            expect(msg.data.error).toBeUndefined();
            expect(msg.data.extendedRun).toBe("worker extend");
            expect(msg.data.implementedRun).toBe("worker new");
            if (!proxies) {
                expect(msg.data.extendedClass).toBe("com.tns.gen.java.security.PrivilegedExceptionAction");
            }
            expect(msg.data.implementedClass).toBe(msg.data.extendedClass);
            done();
        };
    });

    it("reuse the cached class of an interface whose implementations define different methods", function (done) {
        var worker = new Worker("./interfaceExtendWorker.js");
        worker.onmessage = function () {
            worker.terminate();
            // Only the worker has loaded this class, so this runtime has to load
            // the dex the worker cached, although this object defines more methods.
            // An extend, unlike new Action({...}), cannot reuse the class the
            // worker left in the process-wide JNI class cache.
            var Action = java.security.PrivilegedExceptionAction;
            var Extended = Action[ext]({
                run: function () {
                    return "main extend";
                },
                describe: function () {
                    return "not a Java method";
                }
            });

            var extended = new Extended();
            expect(String(callFromJava(extended, "run"))).toBe("main extend");
            var entries = reportEntriesFor(String(extended.getClass().getName())).filter(function (entry) {
                return entry.requested.indexOf("_tests_testInterfaceExtendClasses_") > 0;
            });
            expect(entries.length).toBe(1);
            expect(entries[0].source).toBe(proxies ? "proxy" : "cached");
            done();
        };
    });
});
