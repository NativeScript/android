describe("Worker resourceLimits", function () {
    var echoEntry = "./workerResourceLimitsEchoWorker.js";
    var oomEntry = "./workerResourceLimitsOomWorker.js";

    // Jasmine arms a spec's async timeout before calling it, so the interval
    // has to be raised ahead of the spec, not inside it.
    var originalTimeout;
    beforeEach(function () {
        originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
        jasmine.DEFAULT_TIMEOUT_INTERVAL = 60000;
    });
    afterEach(function () {
        jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
    });

    var expectStarts = function (options, done) {
        var worker = options === undefined ? new Worker(echoEntry) : new Worker(echoEntry, options);
        var settled = false;
        var finish = function () {
            if (settled) {
                return;
            }
            settled = true;
            worker.terminate();
            done();
        };
        worker.onmessage = function (msg) {
            expect(msg.data.started).toBe(true);
            finish();
        };
        worker.onerror = function (e) {
            expect(String(e && e.message ? e.message : e)).toBe("<no worker error>");
            finish();
        };
    };

    it("starts a worker under a maxYoungGenerationSizeMb cap", function (done) {
        expectStarts({ resourceLimits: { maxYoungGenerationSizeMb: 8 } }, done);
    });

    it("starts a worker under both heap caps", function (done) {
        expectStarts({ resourceLimits: { maxOldGenerationSizeMb: 64, maxYoungGenerationSizeMb: 8 } },
                     done);
    });

    it("treats resourceLimits: null like an absent resourceLimits", function (done) {
        expectStarts({ resourceLimits: null }, done);
    });

    it("ignores unknown keys inside resourceLimits", function (done) {
        expectStarts({ resourceLimits: { maxOldGenerationSizeMb: 64, somethingElse: 42 } }, done);
    });

    it("reports a worker that runs out of heap through onerror", function (done) {
        var worker = new Worker(oomEntry, { resourceLimits: { maxOldGenerationSizeMb: 32 } });
        var settled = false;

        // The entry never returns, so nothing can post: a message here means the
        // cap was not applied at all.
        worker.onmessage = function () {
            expect("worker posted a message").toBe("worker exhausted its heap");
        };

        worker.onerror = function (e) {
            if (settled) {
                return;
            }
            settled = true;
            var message = String(e && e.message ? e.message : e);
            expect(message).toMatch(/out of memory/i);
            // Naming the cap is what tells this apart from any other failure
            // the worker could have reported.
            expect(message).toMatch(/maxOldGenerationSizeMb: 32/);
            worker.terminate();
            done();
        };
    });

    it("throws a TypeError when resourceLimits is not an object", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: 5 });
        }).toThrowError(TypeError, /"resourceLimits"/);
    });

    it("throws a TypeError for a non-numeric maxOldGenerationSizeMb", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: { maxOldGenerationSizeMb: "64" } });
        }).toThrowError(TypeError, /"resourceLimits\.maxOldGenerationSizeMb"/);
    });

    it("throws a RangeError for a maxOldGenerationSizeMb of zero", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: { maxOldGenerationSizeMb: 0 } });
        }).toThrowError(RangeError, /"resourceLimits\.maxOldGenerationSizeMb"/);
    });

    it("throws a RangeError for a NaN maxYoungGenerationSizeMb", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: { maxYoungGenerationSizeMb: NaN } });
        }).toThrowError(RangeError, /"resourceLimits\.maxYoungGenerationSizeMb"/);
    });

    it("throws a RangeError for a maxOldGenerationSizeMb too large to hold in bytes", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: { maxOldGenerationSizeMb: Number.MAX_VALUE } });
        }).toThrowError(RangeError, /"resourceLimits\.maxOldGenerationSizeMb"/);
    });

    it("throws a RangeError for a maxYoungGenerationSizeMb below one byte", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: { maxYoungGenerationSizeMb: 1e-9 } });
        }).toThrowError(RangeError, /"resourceLimits\.maxYoungGenerationSizeMb"/);
    });

    it("propagates the error thrown by a resourceLimits getter", function () {
        var boom = new Error("boom");
        var options = { resourceLimits: new Proxy({}, {
            get: function (target, key) {
                if (key === "maxOldGenerationSizeMb") {
                    throw boom;
                }
                return undefined;
            }
        }) };
        var thrown;
        try {
            new Worker(echoEntry, options);
        } catch (e) {
            thrown = e;
        }
        expect(thrown).toBe(boom);
    });

    it("throws a RangeError for a jsDispatchTableSizeMb above the ceiling", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: { jsDispatchTableSizeMb: 300 } });
        }).toThrowError(RangeError, /"resourceLimits\.jsDispatchTableSizeMb"/);
    });

    it("throws a RangeError for a fractional jsDispatchTableSizeMb", function () {
        expect(function () {
            new Worker(echoEntry, { resourceLimits: { jsDispatchTableSizeMb: 1.5 } });
        }).toThrowError(RangeError, /"resourceLimits\.jsDispatchTableSizeMb"/);
    });

    it("starts a worker under a jsDispatchTableSizeMb reservation", function (done) {
        expectStarts({ resourceLimits: { jsDispatchTableSizeMb: 64 } }, done);
    });

    it("starts a worker under the smallest jsDispatchTableSizeMb reservation", function (done) {
        expectStarts({ resourceLimits: { jsDispatchTableSizeMb: 1 } }, done);
    });
});
