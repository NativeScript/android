describe("inspect", function () {
    it("formats primitives and plain objects", function () {
        expect(__inspect(42)).toBe("42");
        expect(__inspect("hi")).toBe('"hi"');
        expect(__inspect({ a: 1, b: "x" })).toBe('{ a: 1, b: "x" }');
        expect(__inspect([1, [2, 3]])).toBe("[ 1, [ 2, 3 ] ]");
    });

    it("limits depth", function () {
        expect(__inspect({ a: { b: { c: { d: 1 } } } })).toBe("{ a: { b: { c: [Object] } } }");
        expect(__inspect({ a: { b: { c: { d: 1 } } } }, { depth: 3 })).toBe("{ a: { b: { c: { d: 1 } } } }");
    });

    it("reports true cycles and only true cycles", function () {
        var cyc = {};
        cyc.self = cyc;
        expect(__inspect(cyc)).toBe("{ self: [Circular] }");

        var shared = { x: 1 };
        expect(__inspect({ a: shared, b: shared })).toBe("{ a: { x: 1 }, b: { x: 1 } }");
    });

    it("caps arrays and total output", function () {
        var big = [];
        for (var i = 0; i < 250; i++) {
            big[i] = i;
        }
        expect(__inspect(big).indexOf("... 150 more items")).toBeGreaterThan(-1);

        var huge = {};
        for (var k = 0; k < 100000; k++) {
            huge["key" + k] = k;
        }
        var start = Date.now();
        var out = __inspect(huge);
        var elapsed = Date.now() - start;
        expect(out.length).toBeLessThan(20000);
        // The old JSON path would serialize all 100k keys; the budgeted
        // formatter must not take anywhere near a second.
        expect(elapsed).toBeLessThan(1000);
    });

    it("caps long strings", function () {
        var long = new Array(12001).join("a");
        var out = __inspect(long);
        expect(out.indexOf("... 2000 more characters")).toBeGreaterThan(-1);
    });

    it("never invokes getters", function () {
        var invoked = false;
        var obj = {};
        Object.defineProperty(obj, "x", {
            enumerable: true,
            get: function () {
                invoked = true;
                return 1;
            }
        });
        expect(__inspect(obj)).toBe("{ x: [Getter] }");
        expect(invoked).toBe(false);
    });

    it("never invokes toJSON", function () {
        var invoked = false;
        var obj = {
            a: 1,
            toJSON: function () {
                invoked = true;
                throw new Error("toJSON must not run");
            }
        };
        expect(__inspect(obj)).toBe("{ a: 1, toJSON: [Function: toJSON] }");
        expect(invoked).toBe(false);
    });

    it("caps map and set entries", function () {
        var map = new Map();
        var set = new Set();
        for (var i = 0; i < 150; i++) {
            map.set("k" + i, i);
            set.add(i);
        }

        var mapOut = __inspect(map);
        expect(mapOut.indexOf("Map(150) { ")).toBe(0);
        expect(mapOut.indexOf('"k99" => 99')).toBeGreaterThan(-1);
        expect(mapOut.indexOf('"k100" => 100')).toBe(-1);
        expect(mapOut.indexOf("... 50 more items")).toBeGreaterThan(-1);

        var setOut = __inspect(set);
        expect(setOut.indexOf("Set(150) { ")).toBe(0);
        expect(setOut.indexOf("... 50 more items")).toBeGreaterThan(-1);
    });

    it("does not iterate a large collection past the entry cap", function () {
        var map = new Map();
        for (var i = 0; i < 100000; i++) {
            map.set(i, i);
        }
        var start = Date.now();
        var out = __inspect(map);
        var elapsed = Date.now() - start;
        expect(out.indexOf("Map(100000) { ")).toBe(0);
        expect(out.indexOf("... 99900 more items")).toBeGreaterThan(-1);
        // The entry count comes from the size accessor, so only the first 100
        // entries are ever walked.
        expect(elapsed).toBeLessThan(1000);
    });

    it("formats collections, dates, regexes, errors and functions", function () {
        expect(__inspect(new Map([["k", 1]]))).toBe('Map(1) { "k" => 1 }');
        expect(__inspect(new Set([1, 2]))).toBe("Set(2) { 1, 2 }");
        expect(__inspect(new Date(0))).toBe("1970-01-01T00:00:00.000Z");
        expect(__inspect(/ab+c/gi)).toBe("/ab+c/gi");
        expect(__inspect(function foo() {})).toBe("[Function: foo]");
        expect(__inspect(class Foo {})).toBe("[class Foo]");
        expect(__inspect(new Uint8Array(3))).toBe("Uint8Array(3)");
        expect(__inspect(10n)).toBe("10n");

        var errOut = __inspect(new Error("boom"));
        expect(errOut.indexOf("Error: boom")).toBe(0);
    });

    it("identifies java objects without walking them", function () {
        var out = __inspect(new java.lang.Object());
        expect(out.indexOf("[")).toBe(0);
        expect(out.indexOf("java.lang.Object")).toBeGreaterThan(-1);

        var listOut = __inspect(new java.util.ArrayList());
        expect(listOut.indexOf("java.util.ArrayList")).toBeGreaterThan(-1);

        // The wrapper hint replaces the whole graph, so a java object nested in
        // a plain object stays a single token.
        var nested = __inspect({ v: new java.lang.Object() });
        expect(nested.indexOf("{ v: [")).toBe(0);
    });

    it("does not materialize java packages", function () {
        // Package children are native data properties, so reading descriptors
        // off one would build every class it contains.
        var start = Date.now();
        var out = __inspect(java);
        expect(out.indexOf("[package java")).toBe(0);
        expect(__inspect(java.lang).indexOf("[package java.lang")).toBe(0);
        expect(Date.now() - start).toBeLessThan(1000);
    });

    it("renders java classes as callables, not as graphs", function () {
        // Class wrappers are constructor functions, so they take the callable
        // branch before the native-wrapper hint is consulted.
        var out = __inspect(java.lang.Object);
        expect(/^\[(Function|class)\b/.test(out)).toBe(true);
    });

    it("leaves plain javascript objects to structural rendering", function () {
        expect(__inspect({ a: 1 })).toBe("{ a: 1 }");
        expect(__inspect([1])).toBe("[ 1 ]");
    });

    it("console.log of a huge cyclic object completes quickly", function () {
        var huge = { name: "root" };
        var cursor = huge;
        for (var i = 0; i < 5000; i++) {
            cursor = cursor.child = { i: i, parent: huge };
        }
        huge.self = huge;
        var start = Date.now();
        console.log(huge);
        expect(Date.now() - start).toBeLessThan(1000);
    });

    it("honors custom toString overrides (NativeScript core convention)", function () {
        function ViewLike() { this.id = 42; }
        ViewLike.prototype.toString = function () { return "Button(42)"; };
        expect(__inspect(new ViewLike())).toBe("Button(42)");
        expect(__inspect({ v: new ViewLike() })).toBe("{ v: Button(42) }");
        expect(__inspect({ toString: function () { return "custom!"; } })).toBe("custom!");
        // A broken override degrades to structural rendering instead of hiding data.
        var broken = { a: 1, toString: function () { throw new Error("x"); } };
        expect(__inspect(broken).indexOf("a: 1")).toBeGreaterThan(-1);
    });

    it("finds toString deep in the prototype chain (core layout depth)", function () {
        // Core view chains run ~10 prototypes deep (GridLayout -> ... -> ViewBase,
        // which holds toString); the walk cap must clear them.
        function ViewBase() {}
        ViewBase.prototype.toString = function () { return "GridLayout(7)"; };
        var proto = ViewBase.prototype;
        for (var i = 0; i < 9; i++) {
            proto = Object.create(proto);
        }
        var deep = Object.create(proto);
        expect(__inspect(deep)).toBe("GridLayout(7)");
    });

    it("formats under tampered prototypes", function () {
        var savedSlice = Array.prototype.slice;
        var savedIndexOf = Array.prototype.indexOf;
        var savedKeys = Object.keys;
        var savedStringify = JSON.stringify;
        var boom = function () { throw new Error("tampered"); };
        var out;
        try {
            Array.prototype.slice = boom;
            Array.prototype.indexOf = boom;
            Object.keys = boom;
            JSON.stringify = boom;
            out = __inspect({ a: [1, 2], m: new Map([[1, 2]]) });
        } finally {
            Array.prototype.slice = savedSlice;
            Array.prototype.indexOf = savedIndexOf;
            Object.keys = savedKeys;
            JSON.stringify = savedStringify;
        }
        expect(out).toBe("{ a: [ 1, 2 ], m: Map(1) { 1 => 2 } }");
    });
});

