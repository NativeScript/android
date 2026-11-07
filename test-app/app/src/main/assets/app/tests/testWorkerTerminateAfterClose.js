describe("Worker terminate after close", function () {
	var START_DEADLINE = 3000;
	var SETTLE_AFTER = 300;
	// [0] counts the worker's loop iterations; [1] stops the loop, so a worker
	// that terminate() failed to stop does not outlive the spec.
	var shared;
	var timers = [];
	var originalTimeout;

	beforeEach(function () {
		originalTimeout = jasmine.DEFAULT_TIMEOUT_INTERVAL;
		jasmine.DEFAULT_TIMEOUT_INTERVAL = 10000;
	});

	afterEach(function () {
		timers.forEach(clearTimeout);
		timers = [];
		if (shared) {
			Atomics.store(shared, 1, 1);
			shared = null;
		}
		jasmine.DEFAULT_TIMEOUT_INTERVAL = originalTimeout;
	});

	function later(fn, ms) {
		timers.push(setTimeout(fn, ms));
	}

	it("stops a worker that keeps running after it called close()", function (done) {
		shared = new Int32Array(new SharedArrayBuffer(8));
		var counter = shared;
		var worker = new Worker("./workerCloseThenSpinWorker.js");
		worker.postMessage(counter.buffer);
		var started = Date.now();

		(function waitForSpin() {
			if (Atomics.load(counter, 0) === 0) {
				if (Date.now() - started > START_DEADLINE) {
					expect("the worker never started running").toBeNull();
					done();
					return;
				}
				later(waitForSpin, 20);
				return;
			}
			worker.terminate();
			later(function () {
				var afterTerminate = Atomics.load(counter, 0);
				later(function () {
					expect(Atomics.load(counter, 0)).toBe(afterTerminate);
					done();
				}, SETTLE_AFTER);
			}, SETTLE_AFTER);
		})();
	});
});
