// Writes, deletes and defines on a canary must reach its state exactly as they reach the
// canary itself: what a rebuilt canary shows is the state. Each spec writes in a function
// of its own, so no register keeps the canary alive across the collection.
describe("Wrapper lifetime state writes", function () {
    var runtime = require("ns:runtime");
    var canary = runtime.getConfig("wrapperLifetimeMode") === "canary";
    var Holder = com.tns.tests.WrapperHolder;
    var hasOwn = Function.prototype.call.bind(Object.prototype.hasOwnProperty);

    var Dummy = com.tns.tests.DummyClass.extend({});
    Object.defineProperty(Dummy.prototype, "acc", {
        get: function () {
            return this.accSeen;
        },
        set: function (value) {
            this.accSeen = value;
        },
        enumerable: false,
        configurable: true
    });

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

    function data(value, writable, enumerable, configurable) {
        return {
            value: value,
            writable: writable !== false,
            enumerable: enumerable !== false,
            configurable: configurable !== false
        };
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

    afterEach(function () {
        Holder.held = null;
    });

    it("keeps repeat writes of a name across writes of other names, a Java field and a prototype accessor", function () {
        var symbol = Symbol("repeat");
        var object = rebuilt(function (o) {
            o.a = 1;
            o.b = 1;
            o.a = 2;
            o.nameField = "java";
            o.a = 3;
            o.acc = 5;
            o.a = 4;
            o[symbol] = 1;
            o.b = 2;
            o[symbol] = 2;
            // More names written in turn than one object's cache entries hold.
            for (var i = 0; i < 10; i++) {
                o.a = i;
                o.c = i;
                o.d = i;
                o.e = i;
                o.f = i;
                o.g = i;
            }
            o.nameField = "java again";
            o.acc = 6;
        });
        expect(Object.keys(object).sort()).toEqual(["a", "accSeen", "b", "c", "d", "e", "f", "g"]);
        expect(Object.getOwnPropertyDescriptor(object, "a")).toEqual(data(9));
        expect(object.b).toBe(2);
        expect(object.g).toBe(9);
        expect(object[symbol]).toBe(2);
        expect(object.accSeen).toBe(6);
        expect(hasOwn(object, "acc")).toBe(false);
        expect(hasOwn(object, "nameField")).toBe(false);
        expect(object.getName()).toBe("java again");
    });

    it("keeps a write after a define that changed the property's attributes", function () {
        var object = rebuilt(function (o) {
            o.a = 1;
            o.a = 2;
            Object.defineProperty(o, "a", { writable: false });
            o.a = 3;
            o.b = 1;
            o.b = 2;
            Object.defineProperty(o, "b", { enumerable: false });
            o.b = 3;
            o.c = 1;
            o.c = 2;
            Object.defineProperty(o, "c", { value: 3, writable: true, enumerable: true, configurable: true });
            o.c = 4;
        });
        expect(Object.getOwnPropertyDescriptor(object, "a")).toEqual(data(2, false));
        expect(Object.getOwnPropertyDescriptor(object, "b")).toEqual(data(3, true, false));
        expect(Object.getOwnPropertyDescriptor(object, "c")).toEqual(data(4));
        expect(function () {
            "use strict";
            object.a = 5;
        }).toThrow();
        object.b = 4;
        expect(object.b).toBe(4);
    });

    it("sends a write past a deleted own property that shadowed a Java field or a prototype accessor", function () {
        var object = rebuilt(function (o) {
            Object.defineProperty(o, "nameField", data("own"));
            o.nameField = "own 2";
            o.nameField = "own 3";
            expect(o.getName()).toBe("dummy");
            delete o.nameField;
            o.nameField = "java";
            o.nameField = "java 2";
            Object.defineProperty(o, "acc", data(1));
            o.acc = 2;
            o.acc = 3;
            delete o.acc;
            o.acc = 4;
            o.acc = 5;
        });
        expect(hasOwn(object, "nameField")).toBe(false);
        expect(object.nameField).toBe("java 2");
        expect(object.getName()).toBe("java 2");
        expect(hasOwn(object, "acc")).toBe(false);
        expect(object.accSeen).toBe(5);
        expect(Object.keys(object)).toEqual(["accSeen"]);
    });

    it("keeps a name deleted and added again, and drops one deleted for good", function () {
        var symbol = Symbol("deleted");
        var object = rebuilt(function (o) {
            o.a = 1;
            o.a = 2;
            delete o.a;
            o.a = 3;
            o.a = 4;
            o.b = 1;
            o.b = 2;
            delete o.b;
            o[symbol] = 1;
            o[symbol] = 2;
            delete o[symbol];
            o[symbol] = 3;
        });
        expect(Object.keys(object)).toEqual(["a"]);
        expect(object.a).toBe(4);
        expect("b" in object).toBe(false);
        expect(object[symbol]).toBe(3);
    });

    it("keeps one name written on two objects in turn apart", function () {
        var before = rebuilds();
        (function () {
            var first = new Dummy();
            var second = new Dummy();
            first.x = 1;
            second.x = 1;
            first.x = 2;
            second.x = 2;
            delete first.x;
            second.x = 3;
            first.x = 4;
            first.y = 1;
            second.y = 1;
            first.y = 2;
            var list = new java.util.ArrayList();
            list.add(first);
            list.add(second);
            Holder.held = list;
        })();
        collect();
        var first = Holder.held.get(0);
        var second = Holder.held.get(1);
        if (canary) {
            expect(rebuilds()).toBeGreaterThan(before + 1);
        }
        expect(Object.keys(first)).toEqual(["x", "y"]);
        expect(first.x).toBe(4);
        expect(first.y).toBe(2);
        expect(Object.keys(second)).toEqual(["x", "y"]);
        expect(second.x).toBe(3);
        expect(second.y).toBe(1);
    });

    it("keeps a canary's state across more than one rebuild", function () {
        var object = rebuilt(function (o) {
            o.a = 1;
            o.b = 1;
        });
        (function () {
            var o = Holder.held;
            o.a = 2;
            o.a = 3;
            o.c = 1;
            delete o.b;
            o.b = 2;
        })();
        object = null;
        collect();
        object = Holder.held;
        expect(Object.keys(object)).toEqual(["a", "c", "b"]);
        expect([object.a, object.b, object.c]).toEqual([3, 2, 1]);
    });

    // Class fields define their property, the way Object.defineProperty does: one shadowing a
    // Java field is an own property, and the Java field keeps its value.
    var fieldSymbol = Symbol("field");
    function Base() {
        return new Dummy();
    }
    class WithFields extends Base {
        plain = 1;
        ["computed"] = 2;
        [fieldSymbol] = 3;
        nameField = "field";
        acc = 4;
    }
    var TsFields = (function (_super) {
        __extends(TsFields, _super);
        function TsFields() {
            var _this = _super.call(this) || this;
            Object.defineProperty(_this, "plain", { enumerable: true, configurable: true, writable: true, value: 1 });
            Object.defineProperty(_this, "nameField", { enumerable: true, configurable: true, writable: true, value: "field" });
            return __native(_this);
        }
        return TsFields;
    })(com.tns.tests.DummyClass);

    function expectFields(object) {
        expect(Object.getOwnPropertyDescriptor(object, "plain")).toEqual(data(1));
        expect(Object.getOwnPropertyDescriptor(object, "computed")).toEqual(data(2));
        expect(Object.getOwnPropertyDescriptor(object, fieldSymbol)).toEqual(data(3));
        expect(Object.getOwnPropertyDescriptor(object, "nameField")).toEqual(data("field"));
        expect(Object.getOwnPropertyDescriptor(object, "acc")).toEqual(data(4));
        expect(object.accSeen).toBeUndefined();
        expect(object.getName()).toBe("dummy");
    }

    function plantFields(warm) {
        var before = rebuilds();
        (function () {
            var made;
            for (var i = 0; i < warm; i++) {
                made = new WithFields();
            }
            made = new WithFields();
            expectFields(made);
            Holder.held = made;
        })();
        collect();
        var object = Holder.held;
        if (canary) {
            expect(rebuilds()).toBeGreaterThan(before);
        }
        return object;
    }

    it("keeps class fields defined on a JS-extended object, cold", function () {
        expectFields(plantFields(0));
    });

    it("keeps class fields defined on a JS-extended object, warm", function () {
        var object = plantFields(3000);
        expectFields(object);
        object.plain = 5;
        object = null;
        collect();
        expect(Holder.held.plain).toBe(5);
    });

    it("keeps fields a TypeScript-style subclass defines in its constructor", function () {
        var before = rebuilds();
        (function () {
            var made;
            for (var i = 0; i < 1000; i++) {
                made = new TsFields();
            }
            Holder.held = made;
        })();
        collect();
        var object = Holder.held;
        if (canary) {
            expect(rebuilds()).toBeGreaterThan(before);
        }
        expect(Object.getOwnPropertyDescriptor(object, "plain")).toEqual(data(1));
        expect(Object.getOwnPropertyDescriptor(object, "nameField")).toEqual(data("field"));
        expect(object.getName()).toBe("dummy");
    });
});
