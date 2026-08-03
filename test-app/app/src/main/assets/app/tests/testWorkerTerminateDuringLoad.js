describe("Worker terminate during module load", function () {
	var ITERATIONS = 3;
	// Long enough to outlast the worker's isolate setup, short enough to keep
	// the spec well inside the jasmine timeout.
	var TERMINATE_AFTER = 150;
	var SETTLE_AFTER = 250;

	it("should not report an error or crash when terminate() interrupts the worker's module body", function (done) {
		var errors = [];

		function iteration(remaining) {
			if (remaining === 0) {
				expect(errors).toEqual([]);
				done();
				return;
			}

			var worker = new Worker("./workerTerminateDuringLoadWorker.js");
			worker.onerror = function (e) {
				errors.push(e.message);
			};

			setTimeout(function () {
				worker.terminate();
				setTimeout(function () {
					iteration(remaining - 1);
				}, SETTLE_AFTER);
			}, TERMINATE_AFTER);
		}

		iteration(ITERATIONS);
	});
});
