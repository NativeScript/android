describe("Anonymous extends of an interface", function () {
    var ext = "ex" + "tend";

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
        expect(reportEntriesFor("com.tns.gen.java.security.PrivilegedAction").length).toBe(1);
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
        expect(reportEntriesFor("com.tns.gen.java.lang.AutoCloseable").length).toBe(1);
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
            expect(msg.data.extendedClass).toBe("com.tns.gen.java.security.PrivilegedExceptionAction");
            expect(msg.data.implementedClass).toBe(msg.data.extendedClass);
            done();
        };
    });
});
