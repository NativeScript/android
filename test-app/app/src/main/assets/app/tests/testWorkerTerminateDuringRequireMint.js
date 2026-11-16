describe("Worker terminate during a require mint", function () {
	// One terminate() lands inside a mint only some of the time, so the spec
	// takes enough shots for a miss on every one of them to be implausible.
	var ITERATIONS = 25;
	var SETTLE_AFTER = 60;
	var originalTimeout;

	beforeEach(function () {
		originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
		jasmine.DEFAULT_TIMEOUT_INTERVAL = 30000;
	});

	afterEach(function () {
		jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
	});

	it("should not report an error or crash when terminate() interrupts createRequire", function (done) {
		var errors = [];

		function iteration(remaining) {
			if (remaining === 0) {
				expect(errors).toEqual([]);
				done();
				return;
			}

			var worker = new Worker("./workerRequireMintSpinWorker.js");
			worker.onerror = function (e) {
				errors.push(e.message);
				return true;
			};
			worker.onmessage = function () {
				// Spread over a few milliseconds so the shots do not all hit
				// the same phase of the worker's loop.
				setTimeout(function () {
					worker.terminate();
					setTimeout(function () {
						iteration(remaining - 1);
					}, SETTLE_AFTER);
				}, remaining % 4);
			};
		}

		iteration(ITERATIONS);
	});
});
