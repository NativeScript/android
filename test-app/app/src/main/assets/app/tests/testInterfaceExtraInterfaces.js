// An extend() of a Java interface that lists more interfaces in `interfaces`. Every spec uses
// a base interface no other spec touches: the class generated for an interface without extra
// interfaces is shared by every runtime of the process under one name. The call sites reach
// the app's interfaces through `X`, so the static binding generator does not prebuild their
// classes; the specs at the end name platform interfaces in full on purpose.
describe("Interface extends with extra interfaces", function () {
    var sdk = android.os.Build.VERSION.SDK_INT;
    var X = com.tns.tests.interfaces.extras;
    var F = X.ExtrasFixtures;
    var Extra = X.Extra;
    var PKG = "com.tns.tests.interfaces.extras.";
    var EXTRA = PKG + "Extra";
    var OTHER = PKG + "OtherExtra";

    function sharedClassName(baseName) {
        return "com.tns.gen." + baseName.replace(/\$/g, "_");
    }

    function expectCallSiteClass(instance, baseName) {
        var className = String(F.className(instance));
        expect(className.indexOf(sharedClassName(baseName) + "_")).toBe(0);
    }

    it("implement the base and the extra interfaces in Java", function () {
        var B = X.MainBase;
        var runs = 0;
        var C = B.extend({
            interfaces: [Extra, java.lang.Runnable],
            base: function () { return "js base"; },
            extra: function () { return "js extra"; },
            run: function () { runs++; }
        });
        var instance = new C();

        expectCallSiteClass(instance, PKG + "MainBase");
        expect(F.isInstance(instance, PKG + "MainBase")).toBe(true);
        expect(F.isInstance(instance, EXTRA)).toBe(true);
        expect(F.isInstance(instance, "java.lang.Runnable")).toBe(true);
        expect(String(F.interfaceNames(instance))).toBe(EXTRA + ",com.tns.tests.interfaces.extras.MainBase,java.lang.Runnable");
        expect(String(F.call(instance, PKG + "MainBase", "base"))).toBe("js base");
        expect(String(F.call(instance, EXTRA, "extra"))).toBe("js extra");
        expect(F.isRuntimeGenerated(instance)).toBe(true);

        // Typed parameters are not checked before the call, so only an instance of the type
        // may be passed.
        if (F.isInstance(instance, EXTRA)) {
            expect(String(F.runExtra(instance))).toBe("js extra");
        }
        if (F.isInstance(instance, "java.lang.Runnable")) {
            new java.lang.Thread(instance).run();
            expect(runs).toBe(1);
        }
    });

    it("get a class per extend call, apart from the base's shared class", function () {
        var B = X.TwoSitesBase;
        var WithExtra = B.extend({
            interfaces: [Extra],
            base: function () { return "with extra"; },
            extra: function () { return "extra of the first"; }
        });
        var WithOther = B.extend({
            interfaces: [X.OtherExtra],
            base: function () { return "with other"; },
            other: function () { return "other of the second"; }
        });
        var Without = B.extend({
            base: function () { return "without"; }
        });
        var withExtra = new WithExtra();
        var withOther = new WithOther();
        var without = new Without();

        expect(WithExtra).not.toBe(WithOther);
        expectCallSiteClass(withExtra, PKG + "TwoSitesBase");
        expectCallSiteClass(withOther, PKG + "TwoSitesBase");
        expect(String(F.className(withExtra))).not.toBe(String(F.className(withOther)));
        expect(String(F.className(without))).toBe(sharedClassName(PKG + "TwoSitesBase"));

        expect(F.isInstance(withExtra, EXTRA)).toBe(true);
        expect(F.isInstance(withExtra, OTHER)).toBe(false);
        expect(F.isInstance(withOther, OTHER)).toBe(true);
        expect(F.isInstance(withOther, EXTRA)).toBe(false);
        expect(F.isInstance(without, EXTRA)).toBe(false);

        expect(String(F.call(withExtra, PKG + "TwoSitesBase", "base"))).toBe("with extra");
        expect(String(F.call(withOther, PKG + "TwoSitesBase", "base"))).toBe("with other");
        expect(String(F.call(withOther, OTHER, "other"))).toBe("other of the second");
        expect(String(F.call(without, PKG + "TwoSitesBase", "base"))).toBe("without");
    });

    it("leave the base's shared class alone when it was created first", function () {
        var B = X.PlainFirstBase;
        var plain = new B({ base: function () { return "plain"; } });
        var C = B.extend({
            interfaces: [Extra],
            base: function () { return "extras"; },
            extra: function () { return "js extra"; }
        });
        var withExtras = new C();

        expect(String(F.className(plain))).toBe(sharedClassName(PKG + "PlainFirstBase"));
        expect(F.isInstance(plain, EXTRA)).toBe(false);
        expectCallSiteClass(withExtras, PKG + "PlainFirstBase");
        expect(F.isInstance(withExtras, EXTRA)).toBe(true);
        expect(String(F.call(withExtras, EXTRA, "extra"))).toBe("js extra");
    });

    it("leave the base's shared class alone when they are created first", function () {
        var B = X.ExtrasFirstBase;
        var C = B.extend({
            interfaces: [Extra],
            base: function () { return "extras"; },
            extra: function () { return "js extra"; }
        });
        var withExtras = new C();
        var plain = new B({ base: function () { return "plain"; } });

        expectCallSiteClass(withExtras, PKG + "ExtrasFirstBase");
        expect(F.isInstance(withExtras, EXTRA)).toBe(true);
        expect(String(F.className(plain))).toBe(sharedClassName(PKG + "ExtrasFirstBase"));
        expect(F.isInstance(plain, EXTRA)).toBe(false);
        expect(String(F.call(plain, PKG + "ExtrasFirstBase", "base"))).toBe("plain");
    });

    it("call the JS implementations of default methods of the base and of the extras", function () {
        if (sdk < 24) {
            return;
        }
        var B = X.DefaultsBase;
        var removes = 0;
        var Overriding = B.extend({
            interfaces: [Extra, java.util.Iterator],
            base: function () { return "b"; },
            extra: function () { return "e"; },
            hasNext: function () { return false; },
            next: function () { return null; },
            baseDefault: function () { return "js baseDefault"; },
            extraDefault: function () { return "js extraDefault"; },
            remove: function () { removes++; }
        });
        var NotOverriding = B.extend({
            interfaces: [Extra, java.util.Iterator],
            base: function () { return "b"; },
            extra: function () { return "e"; },
            hasNext: function () { return false; },
            next: function () { return null; }
        });
        var overriding = new Overriding();
        var notOverriding = new NotOverriding();

        expectCallSiteClass(overriding, PKG + "DefaultsBase");
        expect(String(F.className(overriding)).indexOf("-defaults")).toBe(-1);
        expect(String(F.call(overriding, PKG + "DefaultsBase", "baseDefault"))).toBe("js baseDefault");
        expect(String(F.call(overriding, EXTRA, "extraDefault"))).toBe("js extraDefault");
        F.call(overriding, "java.util.Iterator", "remove");
        expect(removes).toBe(1);
        expect(String(F.call(notOverriding, "java.util.Iterator", "remove"))).toBe("java.lang.UnsupportedOperationException");
        if (F.areDefaultsDesugared()) {
            return;
        }
        expect(String(F.call(notOverriding, PKG + "DefaultsBase", "baseDefault"))).toBe("java");
        expect(String(F.call(notOverriding, EXTRA, "extraDefault"))).toBe("java");
    });

    it("call the JS implementation of a default method the base and an extra both declare", function () {
        if (sdk < 24) {
            return;
        }
        var B = X.ConflictBase;
        var C = B.extend({
            interfaces: [X.ConflictExtra],
            base: function () { return "b"; },
            shared: function () { return "js shared"; }
        });
        var instance = new C();

        expect(F.isInstance(instance, PKG + "ConflictExtra")).toBe(true);
        expect(String(F.call(instance, PKG + "ConflictBase", "shared"))).toBe("js shared");
        expect(String(F.call(instance, PKG + "ConflictExtra", "shared"))).toBe("js shared");
    });

    it("implement an interface listed twice, or the base listed again, once", function () {
        var B = X.DuplicateBase;
        var C = B.extend({
            interfaces: [B, Extra, Extra],
            base: function () { return "b"; },
            extra: function () { return "e"; }
        });
        var instance = new C();

        expectCallSiteClass(instance, PKG + "DuplicateBase");
        expect(String(F.interfaceNames(instance))).toBe("com.tns.tests.interfaces.extras.DuplicateBase," + EXTRA);
        expect(String(F.call(instance, EXTRA, "extra"))).toBe("e");
    });

    it("keep a class listed as an extra interface out of the implemented interfaces", function () {
        var B = X.NotAnInterfaceBase;
        var C = B.extend({
            interfaces: [X.AbstractExtra],
            base: function () { return "b"; },
            abstractExtra: function () { return "a"; }
        });
        var instance = new C();

        expect(String(F.interfaceNames(instance))).toBe("com.tns.tests.interfaces.extras.NotAnInterfaceBase");
        expect(F.isInstance(instance, PKG + "AbstractExtra")).toBe(false);
        expect(String(F.call(instance, PKG + "NotAnInterfaceBase", "base"))).toBe("b");
    });

    it("implement the extra interfaces of a nested base interface", function () {
        var B = X.Bases.Nested;
        var C = B.extend({
            interfaces: [Extra],
            base: function () { return "nested"; },
            extra: function () { return "nested extra"; }
        });
        var instance = new C();

        expectCallSiteClass(instance, PKG + "Bases$Nested");
        expect(F.isInstance(instance, EXTRA)).toBe(true);
        expect(String(F.call(instance, PKG + "Bases$Nested", "base"))).toBe("nested");
    });

    it("implement the extra interfaces of a named extend", function () {
        var B = X.NamedBase;
        var C = B.extend("com.tns.tests.extras.NamedRuntimeExtras", {
            interfaces: [Extra],
            base: function () { return "named base"; },
            extra: function () { return "named extra"; },
            extraDefault: function () { return "named extraDefault"; }
        });
        var instance = new C();

        expect(String(F.className(instance))).toBe("com.tns.tests.extras.NamedRuntimeExtras");
        expect(F.isRuntimeGenerated(instance)).toBe(true);
        expect(F.isInstance(instance, EXTRA)).toBe(true);
        expect(String(F.call(instance, EXTRA, "extra"))).toBe("named extra");
        if (sdk >= 24) {
            expect(String(F.call(instance, EXTRA, "extraDefault"))).toBe("named extraDefault");
        }
    });

    it("implement the extra interfaces of a TypeScript class decorated with Interfaces", function () {
        var TSBase = X.TypeScriptBase;
        var Decorated = (function (_super) {
            __extends(Decorated, _super);
            function Decorated() {
                return _super.apply(this, arguments) || this;
            }
            Decorated.prototype.base = function () { return "ts base"; };
            Decorated.prototype.extra = function () { return "ts extra"; };
            Decorated = __decorate([
                Interfaces([X.Extra])
            ], Decorated);
            return Decorated;
        }(TSBase));
        var instance = new Decorated();

        expectCallSiteClass(instance, PKG + "TypeScriptBase");
        expect(F.isInstance(instance, EXTRA)).toBe(true);
        expect(String(F.call(instance, EXTRA, "extra"))).toBe("ts extra");
        expect(String(F.call(instance, PKG + "TypeScriptBase", "base"))).toBe("ts base");
    });

    it("cache their dex under the class's own name", function () {
        var B = X.CacheBase;
        var C = B.extend({
            interfaces: [Extra],
            base: function () { return "b"; },
            extra: function () { return "e"; }
        });
        var className = String(F.className(new C()));

        // The dex is looked up by this name on the next launch.
        expect(F.cachedDexFiles(className + "-").length).toBe(1);
    });

    it("leave the interfaces of new Interface({...}) out of the shared class", function () {
        var B = X.NewWithInterfacesBase;
        var instance = new B({
            interfaces: [Extra],
            base: function () { return "b"; },
            extra: function () { return "e"; }
        });

        expect(String(F.className(instance))).toBe(sharedClassName(PKG + "NewWithInterfacesBase"));
        expect(F.isInstance(instance, EXTRA)).toBe(false);
        expect(String(F.call(instance, PKG + "NewWithInterfacesBase", "base"))).toBe("b");
    });

    it("implement the extra interfaces in a worker", function (done) {
        var DEADLINE_MS = 20000;
        var finished = false;
        var worker = new Worker("./interfaceExtraInterfacesWorker.js");

        function finish(result) {
            if (finished) {
                return;
            }
            finished = true;
            clearTimeout(timer);
            worker.terminate();

            expect(result.error).toBeUndefined();
            expect(String(result.className).indexOf(sharedClassName(PKG + "WorkerBase") + "_")).toBe(0);
            expect(result.isExtra).toBe(true);
            expect(result.extra).toBe("worker extra");

            // The last use of WorkerBase: the shared class still serves new WorkerBase({...}).
            var plain = new X.WorkerBase({ base: function () { return "main"; } });
            expect(String(F.className(plain))).toBe(sharedClassName(PKG + "WorkerBase"));
            expect(F.isInstance(plain, EXTRA)).toBe(false);
            expect(String(F.call(plain, PKG + "WorkerBase", "base"))).toBe("main");
            done();
        }

        var timer = setTimeout(function () {
            finish({ error: "the worker did not report within " + DEADLINE_MS + "ms" });
        }, DEADLINE_MS);
        worker.onmessage = function (msg) {
            finish(msg.data);
        };
        worker.onerror = function (e) {
            finish({ error: "worker error: " + (e && e.message) });
        };
    }, 30000);

    // Named in full, so the static binding generator prebuilds their classes.
    it("are prebuilt under the call-site class name, apart from the base's shared class", function () {
        var Prebuilt = java.util.function.LongSupplier.extend({
            interfaces: [java.util.function.IntSupplier],
            getAsLong: function () { return 41; },
            getAsInt: function () { return 42; }
        });
        var plain = new java.util.function.LongSupplier({
            getAsLong: function () { return 43; }
        });
        var prebuilt = new Prebuilt();

        expectCallSiteClass(prebuilt, "java.util.function.LongSupplier");
        expect(F.isRuntimeGenerated(prebuilt)).toBe(false);
        expect(F.isInstance(prebuilt, "java.util.function.IntSupplier")).toBe(true);
        expect(String(F.call(prebuilt, "java.util.function.IntSupplier", "getAsInt"))).toBe("42");

        expect(String(F.className(plain))).toBe("com.tns.gen.java.util.function.LongSupplier");
        expect(F.isInstance(plain, "java.util.function.IntSupplier")).toBe(false);
        expect(String(F.call(plain, "java.util.function.LongSupplier", "getAsLong"))).toBe("43");
    });

    it("are prebuilt with the extra interfaces of a named extend", function () {
        var Named = java.util.function.DoubleSupplier.extend("com.tns.tests.extras.NamedPrebuiltExtras", {
            interfaces: [java.util.function.BooleanSupplier],
            getAsDouble: function () { return 1.5; },
            getAsBoolean: function () { return true; }
        });
        var instance = new Named();

        expect(String(F.className(instance))).toBe("com.tns.tests.extras.NamedPrebuiltExtras");
        expect(F.isRuntimeGenerated(instance)).toBe(false);
        expect(F.isInstance(instance, "java.util.function.BooleanSupplier")).toBe(true);
        expect(String(F.call(instance, "java.util.function.BooleanSupplier", "getAsBoolean"))).toBe("true");
    });
});
