var CAST_NAMES = ["long", "byte", "short", "char", "float", "double"];

describe("ns:interop", function () {
    var nsInterop = require("ns:interop");

    // Android has none of the cross-platform interop members iOS exports.
    it("is an empty frozen namespace", function () {
        expect(Object.isFrozen(nsInterop)).toBe(true);
        expect(Object.keys(nsInterop)).toEqual([]);
    });

    it("is a singleton across require calls", function () {
        expect(require("ns:interop")).toBe(nsInterop);
    });
});

describe("ns:interop/android", function () {
    var android = require("ns:interop/android");

    it("exposes exactly the declared surface, frozen", function () {
        expect(Object.isFrozen(android)).toBe(true);
        expect(Object.keys(android).sort()).toEqual(
            ["byte", "char", "double", "float", "long", "newArray", "short"]);
        Object.keys(android).forEach(function (name) {
            expect(typeof android[name]).toBe("function");
        });
    });

    it("is a singleton across require calls", function () {
        expect(require("ns:interop/android")).toBe(android);
    });

    it("backs the cast globals with the same function objects", function () {
        CAST_NAMES.forEach(function (name) {
            expect(globalThis[name]).toBe(android[name]);
        });
    });

    it("backs Array.create with newArray", function () {
        expect(Array.create).toBe(android.newArray);
    });

    it("keeps the globals ordinary writable, enumerable, configurable properties", function () {
        CAST_NAMES.forEach(function (name) {
            var descriptor = Object.getOwnPropertyDescriptor(globalThis, name);
            expect(descriptor.writable).toBe(true);
            expect(descriptor.enumerable).toBe(true);
            expect(descriptor.configurable).toBe(true);
        });
        var create = Object.getOwnPropertyDescriptor(Array, "create");
        expect(create.writable).toBe(true);
        expect(create.enumerable).toBe(true);
        expect(create.configurable).toBe(true);
    });

    it("marks numbers for Java overload resolution", function () {
        var n = new com.tns.tests.NumericConversionTest();
        expect(n.method5(android.long("123456789012"))).toBe("long=123456789012");
        expect(n.method5(android.float(1.23))).toBe("float=1.23");
        expect(n.method5(android.double(1))).toBe("double=1.0");
        expect(n.method2(android.double(65536 + 2))).toBe("double=65538.0");
        expect(n.method2(android.byte(65536 + 2))).toBe("float=2.0");
        expect(n.method2(android.short(65536 + 2))).toBe("float=2.0");
    });

    it("rejects a cast with the wrong arity", function () {
        expect(function () {
            android.long();
        }).toThrow();
    });

    it("creates Java arrays from a primitive type name or a class", function () {
        var ints = android.newArray("int", 3);
        expect(ints.length).toBe(3);
        expect(ints.getClass().getName()).toBe("[I");

        var strings = android.newArray(java.lang.String, 2);
        expect(strings.length).toBe(2);
        expect(strings.getClass().getName()).toBe("[Ljava.lang.String;");
    });

    it("rejects bad newArray arguments", function () {
        expect(function () {
            android.newArray("int");
        }).toThrow();
        expect(function () {
            android.newArray("int", -1);
        }).toThrow();
    });

    it("resolves through a static import", function (done) {
        import("~/testNsInteropImport.mjs").then(function (module) {
            expect(module.interopDefault).toBe(require("ns:interop"));
            expect(module.androidDefault).toBe(android);
            expect(module.long).toBe(android.long);
            expect(module.newArray).toBe(android.newArray);
            expect(module.escapeException).toBe(require("ns:runtime").escapeException);
            done();
        }, function (error) {
            fail("static import of ns:interop/android should resolve: " + error);
            done();
        });
    });

    it("resolves through a dynamic import", function (done) {
        import("ns:interop/android").then(function (namespace) {
            expect(namespace.default).toBe(android);
            expect(namespace.long).toBe(android.long);
            expect(namespace.newArray).toBe(android.newArray);
            done();
        }, function (error) {
            fail("dynamic import of ns:interop/android should resolve: " + error);
            done();
        });
    });

    it("gives a worker its own instance behind its own globals", function (done) {
        var worker = new Worker("./nsInteropWorker.js");
        worker.onmessage = function (msg) {
            expect(msg.data).toEqual({
                casts: true,
                newArray: true,
                escapeException: true,
                arrayLength: 4,
            });
            worker.terminate();
            done();
        };
        worker.onerror = function (error) {
            fail("worker failed: " + error.message);
            worker.terminate();
            done();
        };
        worker.postMessage("go");
    });
});

describe("interop global", function () {
    it("holds exactly ns:runtime's escapeException", function () {
        expect(Object.keys(interop)).toEqual(["escapeException"]);
        expect(interop.escapeException).toBe(require("ns:runtime").escapeException);
    });

    it("is an ordinary writable, enumerable, configurable global", function () {
        var descriptor = Object.getOwnPropertyDescriptor(globalThis, "interop");
        expect(descriptor.writable).toBe(true);
        expect(descriptor.enumerable).toBe(true);
        expect(descriptor.configurable).toBe(true);
    });
});
