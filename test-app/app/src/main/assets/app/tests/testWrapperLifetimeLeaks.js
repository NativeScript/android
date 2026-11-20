describe("Wrapper lifetime leak probe", function () {
    var runtime = require("ns:runtime");
    var mode = runtime.getConfig("wrapperLifetimeMode");
    var canary = mode === "canary";
    var Holder = com.tns.tests.WrapperHolder;
    // WeakRefs, so the spec itself keeps nothing it planted alive.
    var planted = [];
    // Alive for the whole suite: what leaks is the entry's value.
    var probeMap = new WeakMap();
    var report;

    function cyclesThrough(edge) {
        return report.cycles.filter(function (cycle) {
            return cycle.path.indexOf("--" + edge + "-->") !== -1;
        });
    }

    // Each probe dumps the Java heap, so every shape is planted once and probed once, on a
    // later task: a WeakRef keeps its target alive until the job that made it ends.
    beforeAll(function (done) {
        (function () {
            // A JS property referencing its own object.
            var selfReferencing = new java.lang.Runnable({ run: function () {} });
            selfReferencing.probeSelf = selfReferencing;
            planted.push(new WeakRef(selfReferencing));

            var twin = new java.lang.Runnable({ run: function () {} });
            twin.probeSelf = twin;
            planted.push(new WeakRef(twin));

            // A cycle through Java: the object's state holds a list that holds the object.
            var listed = new java.lang.Runnable({ run: function () {} });
            var list = new java.util.ArrayList();
            list.add(listed);
            listed.probeList = list;
            planted.push(new WeakRef(listed));

            // Held by a Java static: alive for a reason of its own, not a leak.
            var held = new java.lang.Runnable({ run: function () {} });
            held.probeHeld = held;
            Holder.held = held;
            planted.push(new WeakRef(held));

            // A WeakMap value referencing its own key: in canary mode the key is the
            // object's lifetime key, which its state holds.
            var mapped = new java.lang.Runnable({ run: function () {} });
            probeMap.set(mapped, { probeMapped: mapped });
            planted.push(new WeakRef(mapped));

            // A back reference through a WeakRef keeps nothing.
            var weak = new java.lang.Runnable({ run: function () {} });
            weak.probeWeak = new WeakRef(weak);
        })();
        setTimeout(function () {
            gc();
            java.lang.System.nanoTime();
            report = runtime.findLeakCycles();
            console.log("leak probe: " + JSON.stringify({
                leaked: report.leaked,
                leakedOutsideCycles: report.leakedOutsideCycles,
                probed: report.probed,
                cycles: report.cycles.length,
                durationMs: report.durationMs,
                dumpBytes: report.dumpBytes
            }));
            report.cycles.forEach(function (cycle) {
                console.log("leak probe cycle: " + cycle.signature);
            });
            done();
        }, 0);
    });

    afterAll(function () {
        Holder.held = null;
        planted.forEach(function (ref) {
            var object = ref.deref();
            if (object !== undefined) {
                __releaseNativeCounterpart(object);
            }
        });
        planted = [];
        report = null;
    });

    it("reports the heaps it joined (" + mode + ")", function () {
        expect(report.mode).toBe(mode);
        expect(Array.isArray(report.cycles)).toBe(true);
        if (!canary) {
            // Nothing to look for: no dump is taken.
            expect(report.leaked).toBe(0);
            expect(report.javaObjects).toBeUndefined();
            return;
        }
        expect(report.javaObjects).toBeGreaterThan(0);
        expect(report.jsNodes).toBeGreaterThan(0);
        expect(report.probed).toBeGreaterThan(0);
        expect(report.truncated).toBe(false);
        expect(report.durationMs).toBeGreaterThan(0);
    });

    it("finds an object whose JS state references it", function () {
        var cycles = cyclesThrough("probeSelf");
        if (!canary) {
            // Resurrection breaks it: nothing keeps the object once JS lets go.
            expect(cycles.length).toBe(0);
            return;
        }
        expect(cycles.length).toBe(2);
        expect(cycles[0].path).toContain("--ns:record-->");
        expect(cycles[0].path).toContain("--ns:wrapper-->");
        expect(cycles[0].members.length).toBe(1);
    });

    it("gives the same shape the same signature, free of ids", function () {
        if (!canary) {
            return;
        }
        var cycles = cyclesThrough("probeSelf");
        expect(cycles[0].signature).toBe(cycles[1].signature);
    });

    it("finds a cycle through a Java collection", function () {
        var cycles = cyclesThrough("probeList");
        if (!canary) {
            expect(cycles.length).toBe(0);
            return;
        }
        expect(cycles.length).toBe(1);
        expect(cycles[0].path).toContain("--java.util.ArrayList.elementData-->");
    });

    it("finds a cycle through a WeakMap value that references its key", function () {
        var cycles = cyclesThrough("probeMapped");
        if (!canary) {
            expect(cycles.length).toBe(0);
            return;
        }
        expect(cycles.length).toBe(1);
        expect(cycles[0].path).toContain("--WeakMap value-->");
    });

    it("does not report an object Java holds for a reason of its own", function () {
        expect(cyclesThrough("probeHeld").length).toBe(0);
    });

    it("does not report a back reference through a WeakRef", function () {
        expect(cyclesThrough("probeWeak").length).toBe(0);
    });
});
