describe("ns:util", function () {
    var util = require("ns:util");

    it("exports a frozen inspect/format pair", function () {
        expect(Object.isFrozen(util)).toBe(true);
        expect(typeof util.inspect).toBe("function");
        expect(typeof util.format).toBe("function");
        expect(util.inspect({ a: 1 })).toBe("{ a: 1 }");
    });

    it("is a singleton per realm", function () {
        expect(require("ns:util")).toBe(util);
    });

    it("throws for an unknown builtin", function () {
        expect(function () {
            require("ns:definitely-not-a-module");
        }).toThrow();

        var error;
        try {
            require("ns:definitely-not-a-module");
        } catch (e) {
            error = e;
        }
        expect(error instanceof Error).toBe(true);
        expect(error.message).toBe("No such built-in module: ns:definitely-not-a-module");
    });

    it("leaves bare specifiers to npm resolution", function () {
        var resolved;
        try {
            resolved = require("util");
        } catch (e) {
            resolved = undefined;
        }
        expect(resolved).not.toBe(util);
        expect(resolved && resolved.format).not.toBe(util.format);
    });

    describe("format", function () {
        var format = util.format;

        it("substitutes %s, %d, %i and %f", function () {
            expect(format("%s", "a")).toBe("a");
            expect(format("%s", 3)).toBe("3");
            expect(format("%s", { a: 1 })).toBe("{ a: 1 }");
            expect(format("%d apples", 3)).toBe("3 apples");
            expect(format("%d", "4.7")).toBe("4.7");
            expect(format("%i", "4.7")).toBe("4");
            expect(format("%f", "4.7")).toBe("4.7");
        });

        it("substitutes %j, %o and %O", function () {
            expect(format("%j", { a: 1 })).toBe('{"a":1}');
            expect(format("%o", { a: 1 })).toBe("{ a: 1 }");
            expect(format("%O", { a: 1 })).toBe("{ a: 1 }");
        });

        it("renders a circular value for %j as [Circular]", function () {
            var circular = {};
            circular.self = circular;
            expect(format("%j", circular)).toBe("[Circular]");
        });

        it("collapses %% and leaves unknown or dangling % verbatim", function () {
            expect(format("100%")).toBe("100%");
            expect(format("100%", "x")).toBe("100% x");
            expect(format("%% done", 1)).toBe("% done 1");
            expect(format("a %x b", 1)).toBe("a %x b 1");
            expect(format("trailing %", 1)).toBe("trailing % 1");
            expect(format("%s %s", "a")).toBe("a %s");
        });

        it("appends extra arguments space-separated", function () {
            expect(format("%s", "a", "b", { c: 1 })).toBe("a b { c: 1 }");
        });

        it("joins every argument when the first is not a format string", function () {
            expect(format({ a: 1 }, "x")).toBe("{ a: 1 } x");
            expect(format(1, 2)).toBe("1 2");
            expect(format()).toBe("");
        });
    });

    it("backs console.* formatting", function () {
        expect(function () {
            console.log("%d apples", 3);
            console.log("100%");
        }).not.toThrow();
    });

    it("resolves through dynamic import with the same members", function (done) {
        import("ns:util").then(function (ns) {
            expect(ns.default).toBe(util);
            expect(ns.inspect).toBe(util.inspect);
            expect(ns.format).toBe(util.format);
            // The synthetic module is cached per realm.
            return import("ns:util").then(function (again) {
                expect(again).toBe(ns);
                done();
            });
        }, function (error) {
            fail("import('ns:util') should resolve: " + error);
            done();
        });
    });

    it("rejects a dynamic import of an unknown builtin", function (done) {
        import("ns:definitely-not-a-module").then(function () {
            fail("import of an unknown builtin should reject");
            done();
        }, function (error) {
            expect(error instanceof Error).toBe(true);
            expect(error.message).toBe("No such built-in module: ns:definitely-not-a-module");
            done();
        });
    });

    it("resolves through a static import", function (done) {
        import("~/testNsUtilImport.mjs").then(function (module) {
            expect(module.nsDefault).toBe(util);
            expect(module.inspect).toBe(util.inspect);
            expect(module.format).toBe(util.format);
            expect(module.nodeDefault).toBe(require("node:util"));
            expect(module.nodeInspect).toBe(util.inspect);
            done();
        }, function (error) {
            fail("static import of ns:util should resolve: " + error);
            done();
        });
    });

    it("gives a worker its own instance", function (done) {
        var worker = new Worker("./nsUtilWorker.js");
        worker.onmessage = function (msg) {
            expect(msg.data.formatted).toBe("3 apples");
            expect(msg.data.singleton).toBe(true);
            expect(msg.data.frozen).toBe(true);
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

describe("node:util", function () {
    var util = require("ns:util");
    var nodeUtil = require("node:util");

    it("is a distinct module object sharing ns:util's members", function () {
        expect(nodeUtil).not.toBe(util);
        expect(Object.isFrozen(nodeUtil)).toBe(true);
        expect(nodeUtil.inspect).toBe(util.inspect);
        expect(nodeUtil.format).toBe(util.format);
    });

    it("is a singleton per realm", function () {
        expect(require("node:util")).toBe(nodeUtil);
    });

    it("only exposes what the shim implements", function () {
        expect(typeof nodeUtil.promisify).toBe("undefined");
    });

    it("throws for a node builtin with no shim", function () {
        var error;
        try {
            require("node:fs");
        } catch (e) {
            error = e;
        }
        expect(error instanceof Error).toBe(true);
        expect(error.message).toBe("No such built-in module: node:fs");
    });

    it("rejects a dynamic import of a node builtin with no shim", function (done) {
        import("node:fs").then(function () {
            fail("import of an unshimmed node builtin should reject");
            done();
        }, function (error) {
            expect(error instanceof Error).toBe(true);
            expect(error.message).toBe("No such built-in module: node:fs");
            done();
        });
    });

    it("resolves through dynamic import with the same members", function (done) {
        import("node:util").then(function (ns) {
            expect(ns.default).toBe(nodeUtil);
            expect(ns.inspect).toBe(util.inspect);
            expect(ns.format).toBe(util.format);
            done();
        }, function (error) {
            fail("import('node:util') should resolve: " + error);
            done();
        });
    });
});
