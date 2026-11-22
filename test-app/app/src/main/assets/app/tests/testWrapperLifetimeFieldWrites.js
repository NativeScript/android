// A canary's write handler runs a Java field's setter itself and caches where the field
// is, by name and the canary's prototype. Each spec writes once to fill that cache, then
// changes what the write should reach, then checks the next write reached it.
describe("Wrapper lifetime Java field writes", function () {
    var runtime = require("ns:runtime");
    var mode = runtime.getConfig("wrapperLifetimeMode");
    var scope = runtime.getConfig("wrapperLifetimeScope");
    var canary = mode === "canary";
    var Fixtures = com.tns.tests.CanaryFieldFixtures;
    var Base = Fixtures.Base;
    var Leaf = Fixtures.Leaf;
    var Holder = com.tns.tests.WrapperHolder;
    var hasOwn = Function.prototype.call.bind(Object.prototype.hasOwnProperty);
    var ShadowBase = Fixtures.ShadowBase;
    // Calls the Java getter even on an object whose prototype chain no longer has it.
    function javaF(object) {
        return (object instanceof ShadowBase ? ShadowBase : Base).prototype.getF.call(object);
    }

    function cachedFieldWrites() {
        return runtime.getWrapperLifetimeStats().canaryCachedFieldWrites;
    }

    // Whether writes to `object` go through the canary write handler.
    function handled(extended) {
        return canary && (scope === "all" || extended);
    }

    // Runs write() twice, expecting the cache to place at least one of them: a
    // collection that moves the prototype in between turns a hit into a refill.
    function expectCached(write, extended) {
        var before = cachedFieldWrites();
        write();
        write();
        var hits = cachedFieldWrites() - before;
        if (handled(extended)) {
            expect(hits).toBeGreaterThan(0);
        } else {
            expect(hits).toBe(0);
        }
    }

    // Fills the cache with a write of object.f, then writes it from the cache.
    function warm(object, extended) {
        object.f = 10;
        expectCached(function () {
            object.f = 11;
        }, extended);
        expect(javaF(object)).toBe(11);
    }

    function accessor(log) {
        return {
            get: function () {
                return "js";
            },
            set: function (value) {
                log.push(value);
            },
            configurable: true
        };
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

    afterEach(function () {
        Holder.held = null;
    });

    it("writes a field declared on the own, an extended or a farther class prototype from the cache", function () {
        warm(new Base(), false);
        warm(new Leaf(), false);
        var Ext = Base.extend({});
        warm(new Ext(), true);
        var LeafExt = Leaf.extend({});
        var deep = new LeafExt();
        warm(deep, true);
        expect(Object.keys(deep)).toEqual([]);
    });

    it("writes one field name through more prototypes than a cache set has ways from the cache", function () {
        var BaseExt = Base.extend({});
        var MidExt = Fixtures.Mid.extend({});
        var LeafExt = Leaf.extend({});
        var objects = [[new Base(), false], [new Fixtures.Mid(), false], [new Leaf(), false],
                       [new BaseExt(), true], [new MidExt(), true], [new LeafExt(), true]];
        objects.forEach(function (pair) {
            pair[0].f = 1;
        });
        var before = cachedFieldWrites();
        for (var pass = 0; pass < 2; pass++) {
            objects.forEach(function (pair, i) {
                pair[0].f = 100 + i;
            });
        }
        objects.forEach(function (pair, i) {
            expect(javaF(pair[0])).toBe(100 + i);
        });
        var expected = objects.filter(function (pair) {
            return handled(pair[1]);
        }).length;
        var hits = cachedFieldWrites() - before;
        if (expected > 0) {
            expect(hits).not.toBeLessThan(expected);
        } else {
            expect(hits).toBe(0);
        }
    });

    it("sends a write to an accessor defined later on a Java class prototype between the canary and the field", function () {
        var ShadowMid = Fixtures.ShadowMid;
        var leaf = new Fixtures.ShadowLeaf();
        var ext = new (Fixtures.ShadowLeaf.extend({}))();
        warm(leaf, false);
        warm(ext, true);
        var log = [];
        Object.defineProperty(ShadowMid.prototype, "f", accessor(log));
        try {
            leaf.f = 20;
            ext.f = 21;
            expect(log).toEqual([20, 21]);
            expect(leaf.getF()).toBe(11);
            expect(ext.getF()).toBe(11);
        } finally {
            delete ShadowMid.prototype.f;
        }
        leaf.f = 22;
        ext.f = 23;
        expect(leaf.getF()).toBe(22);
        expect(ext.getF()).toBe(23);
        expect(hasOwn(leaf, "f")).toBe(false);
    });

    it("sends a write to an accessor or a data property defined later on the extended class prototype", function () {
        var WithAccessor = Base.extend({});
        var withAccessor = new WithAccessor();
        warm(withAccessor, true);
        var log = [];
        Object.defineProperty(WithAccessor.prototype, "f", accessor(log));
        withAccessor.f = 30;
        expect(log).toEqual([30]);
        expect(withAccessor.getF()).toBe(11);

        var WithData = Base.extend({});
        var withData = new WithData();
        warm(withData, true);
        Object.defineProperty(WithData.prototype, "f", { value: 0, writable: true, configurable: true });
        withData.f = 31;
        expect(hasOwn(withData, "f")).toBe(true);
        expect(withData.getF()).toBe(11);
        // The own property the write made now shadows the field.
        delete WithData.prototype.f;
        withData.f = 32;
        expect(withData.f).toBe(32);
        expect(withData.getF()).toBe(11);
        delete withData.f;
        withData.f = 33;
        expect(withData.getF()).toBe(33);
        expect(hasOwn(withData, "f")).toBe(false);
    });

    it("writes an own property defined on the instance, and the field again once it is deleted", function () {
        [[new Base(), false], [new (Base.extend({}))(), true]].forEach(function (pair) {
            var object = pair[0];
            warm(object, pair[1]);
            Object.defineProperty(object, "f", { value: 40, writable: true, enumerable: true, configurable: true });
            object.f = 41;
            expect(Object.getOwnPropertyDescriptor(object, "f").value).toBe(41);
            expect(javaF(object)).toBe(11);
            delete object.f;
            expectCached(function () {
                object.f = 42;
            }, pair[1]);
            expect(javaF(object)).toBe(42);
            expect(hasOwn(object, "f")).toBe(false);
        });
    });

    it("writes an own property an object with JS state has, whichever object filled the cache", function () {
        var Ext = Base.extend({});
        var other = new Ext();
        var own = new Ext();
        own.tag = "state";
        Object.defineProperty(own, "f", { value: 50, writable: true, enumerable: true, configurable: true });
        warm(other, true);
        own.f = 51;
        expect(own.f).toBe(51);
        expect(own.getF()).toBe(1);
        own.tag = "more";
        warm(other, true);
        own.f = 52;
        expect(own.f).toBe(52);
        expect(own.getF()).toBe(1);
    });

    it("keeps apart a plain object of a class and an extended one whose prototype has its own accessor", function () {
        var log = [];
        var Own = Base.extend({});
        Object.defineProperty(Own.prototype, "f", accessor(log));

        var plainFirst = new Base();
        var extendedSecond = new Own();
        warm(plainFirst, false);
        extendedSecond.f = 60;
        extendedSecond.f = 61;

        var extendedFirst = new Own();
        extendedFirst.f = 62;
        extendedFirst.f = 63;
        var plainSecond = new Base();
        warm(plainSecond, false);

        expect(log).toEqual([60, 61, 62, 63]);
        expect(extendedSecond.getF()).toBe(1);
        expect(extendedFirst.getF()).toBe(1);
        expect(plainFirst.getF()).toBe(11);
    });

    it("writes the own property a rebuilt canary got from its state", function () {
        var Rebuilt = Base.extend({});
        (function () {
            var object = new Rebuilt();
            Object.defineProperty(object, "f", { value: 70, writable: true, enumerable: true, configurable: true });
            Holder.held = object;
        })();
        collect();
        var object = Holder.held;
        expect(hasOwn(object, "f")).toBe(true);
        warm(new Rebuilt(), true);
        object.f = 71;
        expect(object.f).toBe(71);
        expect(object.getF()).toBe(1);
    });

    it("throws for a final field on a cached write as on the first", function () {
        [[new Base(), false], [new (Base.extend({}))(), true]].forEach(function (pair) {
            var object = pair[0];
            var message = /final field/;
            expect(function () {
                object.fixed = 4;
            }).toThrowError(message);
            expectCached(function () {
                expect(function () {
                    object.fixed = 5;
                }).toThrowError(message);
            }, pair[1]);
            expect(object.fixed).toBe(3);
        });
    });

    it("follows a prototype chain that changed under a cached write", function () {
        var plain = new Base();
        warm(plain, false);
        Object.setPrototypeOf(plain, Object.prototype);
        plain.f = 80;
        expect(hasOwn(plain, "f")).toBe(true);
        expect(javaF(plain)).toBe(11);

        var Detached = Base.extend({});
        var detached = new Detached();
        warm(detached, true);
        Object.setPrototypeOf(Detached.prototype, Object.prototype);
        detached.f = 81;
        expect(hasOwn(detached, "f")).toBe(true);
        expect(javaF(detached)).toBe(11);

        var Longer = Base.extend({});
        var longer = new Longer();
        warm(longer, true);
        var log = [];
        var inserted = Object.create(Base.prototype, { f: accessor(log) });
        Object.setPrototypeOf(Longer.prototype, inserted);
        longer.f = 82;
        expect(log).toEqual([82]);
        expect(javaF(longer)).toBe(11);
    });

    it("writes fields from a worker, each runtime with its own cache", function (done) {
        var worker = new Worker("./fieldWritesWorker.js");
        var main = new Leaf();
        warm(main, false);
        worker.onmessage = function (msg) {
            worker.terminate();
            expect(msg.data.error).toBeUndefined();
            expect(msg.data.values).toEqual([2, 3, 4, 5]);
            main.f = 12;
            expect(main.getF()).toBe(12);
            done();
        };
        worker.onerror = function (e) {
            worker.terminate();
            done.fail(String(e && e.message || e));
        };
    });
});
