describe("Worker terminate inside a require the worker guards", function () {
	var START_DEADLINE = 3000;
	// Long enough for the worker to get from its flag into the module body.
	var TERMINATE_AFTER = 150;
	var SETTLE_AFTER = 300;
	var timers = [];

	afterEach(function () {
		timers.forEach(clearTimeout);
		timers = [];
	});

	function later(fn, ms) {
		timers.push(setTimeout(fn, ms));
	}

	it("does not let the worker catch the interruption and carry on", function (done) {
		var shared = new Int32Array(new SharedArrayBuffer(8));
		var worker = new Worker("./workerTerminateCaughtRequireWorker.js");
		worker.onerror = function () {
			return true;
		};
		worker.postMessage(shared.buffer);
		var started = Date.now();

		(function waitForRequire() {
			if (Atomics.load(shared, 0) === 0) {
				if (Date.now() - started > START_DEADLINE) {
					expect("the worker never reached its require").toBeNull();
					worker.terminate();
					done();
					return;
				}
				later(waitForRequire, 20);
				return;
			}
			later(function () {
				worker.terminate();
				later(function () {
					expect(Atomics.load(shared, 1)).toBe(0);
					done();
				}, SETTLE_AFTER);
			}, TERMINATE_AFTER);
		})();
	});
});
