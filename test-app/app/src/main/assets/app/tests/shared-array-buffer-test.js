describe("Tests SharedArrayBuffer conversion", function () {
	it("should pass a SharedArrayBuffer to a Java method expecting ByteBuffer", function () {
		var sab = new SharedArrayBuffer(8);
		var view = new Uint8Array(sab);
		for (var i = 0; i < 8; i++) {
			view[i] = i + 1;
		}

		// resolves the ByteBuffer.put(ByteBuffer) overload and copies from the
		// direct buffer created over the SharedArrayBuffer's memory
		var bb = java.nio.ByteBuffer.allocateDirect(8);
		bb.put(sab);
		bb.flip();

		var roundTripped = new Uint8Array(ArrayBuffer.from(bb));
		for (var i = 0; i < 8; i++) {
			expect(roundTripped[i]).toBe(i + 1);
		}
	});

	it("should respect byteOffset and length of typed array views over a SharedArrayBuffer", function () {
		var sab = new SharedArrayBuffer(16);
		var full = new Uint8Array(sab);
		for (var i = 0; i < 16; i++) {
			full[i] = i;
		}

		var slice = new Uint8Array(sab, 4, 8); // bytes 4..11

		var bb = java.nio.ByteBuffer.allocateDirect(8);
		bb.put(slice);
		bb.flip();

		var roundTripped = new Uint8Array(ArrayBuffer.from(bb));
		for (var i = 0; i < 8; i++) {
			expect(roundTripped[i]).toBe(i + 4);
		}
	});

	it("should share memory between the SharedArrayBuffer and the Java buffer (no copy)", function () {
		var sab = new SharedArrayBuffer(4);
		var view = new Uint8Array(sab);
		view[0] = 42;

		// requireNonNull returns its argument, handing back the direct
		// ByteBuffer the runtime created over the SharedArrayBuffer's memory
		var javaBuf = java.util.Objects.requireNonNull(sab);
		expect(javaBuf.get(0)).toBe(42);

		// mutations after the call are visible through the Java buffer
		view[0] = 99;
		expect(javaBuf.get(0)).toBe(99);
	});

	it("should share a SharedArrayBuffer's memory with a worker and a Java buffer at once", function (done) {
		var sab = new SharedArrayBuffer(4);
		var view = new Int32Array(sab);
		view[0] = 0;

		var javaBuf = java.util.Objects.requireNonNull(sab);

		var worker = new Worker("../shared/Workers/EvalWorker.js");
		worker.postMessage({
			value: sab,
			eval: "var v = new Int32Array(value); v[0] = 1234; postMessage('written');"
		});
		worker.onmessage = function (msg) {
			expect(msg.data).toBe("written");
			// the worker's write is visible both to this isolate and to Java
			expect(view[0]).toBe(1234);
			expect(javaBuf.getInt(0)).toBe(1234);
			worker.terminate();
			done();
		};
	});
});
