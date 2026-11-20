describe("Wrapper lifetime", function () {
    var runtime = require("ns:runtime");
    var mode = runtime.getConfig("wrapperLifetimeMode");
    var scope = runtime.getConfig("wrapperLifetimeScope");
    var Holder = com.tns.tests.WrapperHolder;

    function collectJava() {
        for (var i = 0; i < 3; i++) {
            java.lang.Runtime.getRuntime().gc();
            java.lang.System.runFinalization();
        }
    }

    // A JS->Java call: settles the wrappers the last collection queued.
    function crossIntoJava() {
        java.lang.System.nanoTime();
    }

    afterEach(function () {
        Holder.held = null;
    });

    it("reports the configured mode and scope (" + mode + "/" + scope + ")", function () {
        expect(["resurrecting"]).toContain(mode);
        expect(["extended", "all"]).toContain(scope);
    });

    it("rejects writes to the mode and scope", function () {
        expect(function () {
            runtime.setConfig("wrapperLifetimeMode", "resurrecting");
        }).toThrowError(TypeError, "'wrapperLifetimeMode' is read-only; set \"wrapperLifetime\" in the app config");
        expect(function () {
            runtime.setConfig("wrapperLifetimeScope", "all");
        }).toThrowError(TypeError, "'wrapperLifetimeScope' is read-only; set \"wrapperLifetime\" in the app config");
    });

    it("exposes numeric counters", function () {
        var stats = runtime.getWrapperLifetimeStats();
        expect(Object.keys(stats).sort()).toEqual(
            ["collected", "releasedAccesses", "releasedExplicitly", "resurrections", "weakened"]);
        Object.keys(stats).forEach(function (key) {
            expect(typeof stats[key]).toBe("number");
        });
        expect(runtime.getWrapperLifetimeStats()).not.toBe(stats);
    });

    it("counts explicit releases", function () {
        var before = runtime.getWrapperLifetimeStats().releasedExplicitly;
        __releaseNativeCounterpart(new java.lang.Object());
        expect(runtime.getWrapperLifetimeStats().releasedExplicitly).toBe(before + 1);
    });

    if (mode !== "resurrecting") {
        return;
    }

    it("keeps a collected wrapper until Java has collected its object", function () {
        var before = runtime.getWrapperLifetimeStats();
        (function () {
            for (var i = 0; i < 20; i++) {
                new java.lang.Object();
            }
        })();
        gc();
        crossIntoJava();
        var weakened = runtime.getWrapperLifetimeStats();
        expect(weakened.resurrections).toBeGreaterThan(before.resurrections);
        expect(weakened.weakened).toBeGreaterThan(before.weakened);

        collectJava();
        gc();
        crossIntoJava();
        expect(runtime.getWrapperLifetimeStats().collected).toBeGreaterThan(before.collected);
    });

    // The shape the "canary" mode removes: a wrapper resurrected because Java holds its
    // object keeps its JS properties, but the Java objects those properties reference
    // were weakened in the same collection, and Java is free to collect them.
    it("lets Java collect an object only a resurrected wrapper's JS property references", function () {
        (function () {
            var holder = new java.lang.StringBuilder("holder");
            holder.child = new java.lang.StringBuilder("child");
            Holder.held = holder;
        })();
        gc();
        crossIntoJava();
        collectJava();

        var holder = Holder.held;
        expect(holder.toString()).toBe("holder");
        var before = runtime.getWrapperLifetimeStats().releasedAccesses;
        expect(function () {
            holder.child.toString();
        }).toThrowError(/Attempt to use cleared object reference/);
        expect(runtime.getWrapperLifetimeStats().releasedAccesses).toBe(before + 1);
    });
});
