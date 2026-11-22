// What a canary record keeps between canaries: the names its state has, which decide
// whether a write must look the state up, and its class's rebuild data, which the
// records of one class share. A rebuilt canary shows both.
describe("Wrapper lifetime record sharing", function () {
    var runtime = require("ns:runtime");
    var canary = runtime.getConfig("wrapperLifetimeMode") === "canary";
    var Holder = com.tns.tests.WrapperHolder;
    var hasOwn = Function.prototype.call.bind(Object.prototype.hasOwnProperty);
    var Dummy = com.tns.tests.DummyClass.extend({});

    function rebuilds() {
        return runtime.getWrapperLifetimeStats().canaryRebuilds;
    }

    function collect() {
        for (var i = 0; i < 2; i++) {
            gc();
            java.lang.System.nanoTime();
            java.lang.Runtime.getRuntime().gc();
            java.lang.System.runFinalization();
            gc();
            java.lang.System.nanoTime();
        }
    }

    // plant(object) runs on a new Dummy, then the Dummy is rebuilt from its state.
    function rebuilt(plant) {
        var before = rebuilds();
        (function () {
            var object = new Dummy();
            plant(object);
            Holder.held = object;
        })();
        collect();
        var object = Holder.held;
        if (canary) {
            expect(rebuilds()).toBeGreaterThan(before);
        }
        return object;
    }

    // Rebuilds every object a Java list holds; make(i) runs in a function of its own.
    function rebuiltAll(count, make) {
        var list = new java.util.ArrayList();
        (function () {
            for (var i = 0; i < count; i++) {
                list.add(make(i));
            }
        })();
        var before = rebuilds();
        collect();
        var objects = [];
        for (var i = 0; i < count; i++) {
            objects.push(list.get(i));
        }
        if (canary) {
            expect(rebuilds()).toBeGreaterThan(before);
        }
        return objects;
    }

    afterEach(function () {
        Holder.held = null;
    });

    it("keeps rewrites of names written before and after more names than a record tags", function () {
        var object = rebuilt(function (o) {
            for (var i = 0; i < 10; i++) {
                o["n" + i] = i;
            }
            o.n0 = "first";
            o.n9 = "last";
            o.n5 = "middle";
        });
        expect(Object.keys(object)).toEqual(["n0", "n1", "n2", "n3", "n4", "n5", "n6", "n7", "n8", "n9"]);
        expect([object.n0, object.n5, object.n9, object.n1]).toEqual(["first", "middle", "last", 1]);
    });

    it("keeps a rewrite of a name added by a define", function () {
        var object = rebuilt(function (o) {
            Object.defineProperty(o, "plain", { value: 1, writable: true, enumerable: true, configurable: true });
            o.plain = 2;
            Object.defineProperty(o, "hidden", { value: 1, writable: true, enumerable: false, configurable: true });
            o.hidden = 2;
            o.after = 1;
            o.after = 2;
        });
        expect(Object.getOwnPropertyDescriptor(object, "plain")).toEqual({ value: 2, writable: true, enumerable: true, configurable: true });
        expect(Object.getOwnPropertyDescriptor(object, "hidden")).toEqual({ value: 2, writable: true, enumerable: false, configurable: true });
        expect(object.after).toBe(2);
    });

    it("keeps a rewrite of a name made at run time, across collections", function () {
        var object = rebuilt(function (o) {
            var name = ["late", Date.now() % 7, "name"].join("_");
            o[name] = 1;
            o.marker = name;
            gc();
            gc();
            o[name] = 2;
            o[String(name)] = 3;
        });
        expect(object[object.marker]).toBe(3);
        expect(Object.keys(object)).toEqual([object.marker, "marker"]);
    });

    it("keeps a name deleted and written again, then rewritten, on a later canary", function () {
        var object = rebuilt(function (o) {
            o.a = 1;
            o.b = 1;
            delete o.a;
            o.a = 2;
        });
        (function () {
            var o = Holder.held;
            o.a = 3;
            o.b = 2;
            o.c = 1;
            o.c = 2;
        })();
        object = null;
        collect();
        object = Holder.held;
        expect(Object.keys(object)).toEqual(["b", "a", "c"]);
        expect([object.a, object.b, object.c]).toEqual([3, 2, 2]);
    });

    it("rebuilds instances of one class with the class's prototype and methods", function () {
        var Cls = com.tns.tests.DummyClass.extend({
            tag: function () {
                return "cls";
            }
        });
        var objects = rebuiltAll(4, function (i) {
            var o = new Cls();
            o.index = i;
            return o;
        });
        objects.forEach(function (o, i) {
            expect(Object.getPrototypeOf(o)).toBe(Cls.prototype);
            expect(o instanceof Cls).toBe(true);
            expect(o.tag()).toBe("cls");
            expect(o.index).toBe(i);
            expect(o.getName()).toBe("dummy");
        });
    });

    it("rebuilds interface implementations with their own implementation object", function () {
        var objects = rebuiltAll(3, function (i) {
            return new java.lang.Runnable({
                run: function () {
                    this.ran = i;
                },
                which: i
            });
        });
        objects.forEach(function (o, i) {
            expect(o.which).toBe(i);
            o.run();
            expect(o.ran).toBe(i);
            expect(hasOwn(o, "which")).toBe(false);
        });
        expect(Object.getPrototypeOf(objects[0])).not.toBe(Object.getPrototypeOf(objects[1]));
    });

    it("rebuilds instances of more classes than a record can share data with at once", function () {
        var D = com.tns.tests.DummyClass;
        function k(n) {
            return function () {
                return n;
            };
        }
        // One per line: extend() names a class by where it is called.
        var classes = [
            D.extend({ cls: k(0) }),
            D.extend({ cls: k(1) }),
            D.extend({ cls: k(2) }),
            D.extend({ cls: k(3) }),
            D.extend({ cls: k(4) }),
            D.extend({ cls: k(5) }),
            D.extend({ cls: k(6) }),
            D.extend({ cls: k(7) }),
            D.extend({ cls: k(8) }),
            D.extend({ cls: k(9) }),
            D.extend({ cls: k(10) }),
            D.extend({ cls: k(11) }),
            D.extend({ cls: k(12) }),
            D.extend({ cls: k(13) }),
            D.extend({ cls: k(14) }),
            D.extend({ cls: k(15) }),
            D.extend({ cls: k(16) }),
            D.extend({ cls: k(17) }),
            D.extend({ cls: k(18) }),
            D.extend({ cls: k(19) }),
        ];
        var count = classes.length;
        // Classes interleaved, so instances of one class register between other classes'.
        var objects = rebuiltAll(3 * count, function (i) {
            var o = new classes[i % count]();
            o.index = i;
            return o;
        });
        objects.forEach(function (o, i) {
            expect(Object.getPrototypeOf(o)).toBe(classes[i % count].prototype);
            expect(o.cls()).toBe(i % count);
            expect(o.index).toBe(i);
        });
    });

    it("rebuilds an instance of a class whose earlier instances' records were all freed", function () {
        var Cls = com.tns.tests.DummyClass.extend({
            tag: function () {
                return "again";
            }
        });
        (function () {
            for (var i = 0; i < 50; i++) {
                new Cls().index = i;
            }
        })();
        collect();
        collect();
        var objects = rebuiltAll(2, function (i) {
            var o = new Cls();
            o.index = i;
            return o;
        });
        expect(objects[1].tag()).toBe("again");
        expect(Object.getPrototypeOf(objects[1])).toBe(Cls.prototype);
        expect(objects[1].index).toBe(1);
    });
});
