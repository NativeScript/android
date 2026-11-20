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
        expect(["resurrecting", "canary"]).toContain(mode);
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
            ["canaryInterceptedWrites", "canaryNativeDeaths", "canaryRebuilds", "canaryStates",
             "collected", "releasedAccesses", "releasedExplicitly", "resurrections", "weakened"]);
        Object.keys(stats).forEach(function (key) {
            expect(typeof stats[key]).toBe("number");
        });
        expect(runtime.getWrapperLifetimeStats()).not.toBe(stats);
    });

    it("counts explicit releases, and calls on a released wrapper", function () {
        var before = runtime.getWrapperLifetimeStats();
        var released = new java.lang.Object();
        __releaseNativeCounterpart(released);
        expect(runtime.getWrapperLifetimeStats().releasedExplicitly).toBe(before.releasedExplicitly + 1);
        expect(function () {
            released.hashCode();
        }).toThrowError(/no longer has available Java instance counterpart/);
        expect(runtime.getWrapperLifetimeStats().releasedAccesses).toBe(before.releasedAccesses + 1);

        // Not a wrapper at all: not a released access.
        var hashCode = new java.lang.Object().hashCode;
        [{}, new ArrayBuffer(8), new Int8Array(8)].forEach(function (receiver) {
            expect(function () {
                hashCode.call(receiver);
            }).toThrow();
        });
        expect(runtime.getWrapperLifetimeStats().releasedAccesses).toBe(before.releasedAccesses + 1);
    });

    it("counts field accesses on, and arguments of, a released wrapper", function () {
        var point = new android.graphics.Point(1, 2);
        __releaseNativeCounterpart(point);
        var before = runtime.getWrapperLifetimeStats().releasedAccesses;
        expect(function () {
            return point.x;
        }).toThrowError(/no corresponding Java object/);
        expect(runtime.getWrapperLifetimeStats().releasedAccesses).toBe(before + 1);

        // Resolving an overload looks the argument up too, so one call can count more than once.
        expect(function () {
            java.lang.String.valueOf(point);
        }).toThrow();
        expect(runtime.getWrapperLifetimeStats().releasedAccesses).toBeGreaterThan(before + 1);
    });

    it("reports counters as own properties whatever Object.prototype defines", function () {
        Object.defineProperty(Object.prototype, "collected", {
            set: function () {},
            configurable: true
        });
        try {
            var stats = runtime.getWrapperLifetimeStats();
            expect(Object.prototype.hasOwnProperty.call(stats, "collected")).toBe(true);
            expect(typeof stats.collected).toBe("number");
        } finally {
            delete Object.prototype.collected;
        }
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
