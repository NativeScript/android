describe("Worker platform options", function () {
    var entry = "./workerOptionsPriorityWorker.js";

    // Jasmine arms a spec's async timeout before calling it, so the interval
    // has to be raised ahead of the spec, not inside it. A thread niced down
    // to 19 boots a whole isolate on whatever CPU is left over; on a loaded
    // host that has taken well over 10 s.
    var originalTimeout;
    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 60000;
    });
    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
    });

    var reportPriority = function (options, done, check) {
        var worker = options === undefined ? new Worker(entry) : new Worker(entry, options);
        var settled = false;
        var finish = function () {
            if (settled) {
                return;
            }
            settled = true;
            worker.terminate();
            done();
        };
        // A throw inside either handler must still settle the spec and
        // terminate the worker; Jasmine only guards the spec body itself.
        worker.onmessage = function (msg) {
            try {
                check(msg.data.priority);
            } finally {
                finish();
            }
        };
        worker.onerror = function (e) {
            try {
                expect(String(e && e.message ? e.message : e)).toBe("<no worker error>");
            } finally {
                finish();
            }
        };
    };

    // Only the non-negative nice values are asserted exactly: lowering a
    // thread's nice value needs a privilege the app may not hold, so the
    // negative names are covered below by starting a worker instead.
    var priorities = [
        ["lowest", 19],
        ["background", 10],
        ["lessFavorable", 1],
        ["default", 0]
    ];

    priorities.forEach(function (pair) {
        it("runs the worker thread at " + pair[0] + " priority", function (done) {
            reportPriority({ android: { priority: pair[0] } }, done, function (priority) {
                expect(priority).toBe(pair[1]);
            });
        });
    });

    it("accepts a negative priority name", function () {
        var worker;
        expect(function () {
            worker = new Worker(entry, { android: { priority: "urgentAudio" } });
        }).not.toThrow();
        worker.terminate();
    });

    it("accepts a raw nice value", function (done) {
        reportPriority({ android: { priority: 12 } }, done, function (priority) {
            expect(priority).toBe(12);
        });
    });

    it("clamps a nice value above the kernel range", function (done) {
        reportPriority({ android: { priority: 100 } }, done, function (priority) {
            expect(priority).toBe(19);
        });
    });

    it("still honors the deprecated androidPriority option", function (done) {
        reportPriority({ androidPriority: "lowest" }, done, function (priority) {
            expect(priority).toBe(19);
        });
    });

    it("prefers android.priority over androidPriority when both are given", function (done) {
        reportPriority({ android: { priority: "default" }, androidPriority: "lowest" }, done,
                       function (priority) {
            expect(priority).toBe(0);
        });
    });

    it("ignores unknown keys inside android", function (done) {
        reportPriority({ android: { priority: "lowest", somethingElse: 42 } }, done,
                       function (priority) {
            expect(priority).toBe(19);
        });
    });

    it("starts a worker given no options at all", function (done) {
        reportPriority(undefined, done, function (priority) {
            expect(typeof priority).toBe("number");
        });
    });

    it("treats android: null like an absent android", function (done) {
        reportPriority({ android: null, androidPriority: "lowest" }, done, function (priority) {
            expect(priority).toBe(19);
        });
    });

    it("propagates the error thrown by an option getter", function () {
        var boom = new Error("boom");
        var options = new Proxy({}, {
            get: function (target, key) {
                if (key === "android") {
                    throw boom;
                }
                return undefined;
            }
        });
        var thrown;
        try {
            new Worker(entry, options);
        } catch (e) {
            thrown = e;
        }
        expect(thrown).toBe(boom);
    });

    it("throws a TypeError when android is not an object", function () {
        expect(function () {
            new Worker(entry, { android: 42 });
        }).toThrowError(TypeError, /"android"/);
    });

    it("throws a TypeError for an unknown android.priority", function () {
        expect(function () {
            new Worker(entry, { android: { priority: "highest" } });
        }).toThrowError(TypeError, /"android\.priority"/);
    });

    it("throws a TypeError for an android.priority that is neither a name nor a number",
       function () {
        expect(function () {
            new Worker(entry, { android: { priority: {} } });
        }).toThrowError(TypeError, /"android\.priority"/);
    });
});
