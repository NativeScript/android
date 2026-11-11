describe("Worker nested console call", function () {
	it("should keep running when formatting a logged argument logs again", function (done) {
		var worker = new Worker("./workerNestedConsoleWorker.js");
		// A worker stuck in ConsoleLog holds the inspector lock that
		// terminate() takes, so only the success path may terminate it.
		worker.onmessage = function (e) {
			expect(e.data).toBe("alive");
			worker.terminate();
			done();
		};
		worker.onerror = function (e) {
			done.fail(e.message);
		};
	});
});
