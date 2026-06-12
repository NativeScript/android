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

		// the holder keeps the direct ByteBuffer the runtime created over the
		// SharedArrayBuffer's memory, so Java reads/writes go to the same bytes
		var holder = new com.tns.tests.ByteBufferHolder();
		holder.hold(sab);
		expect(holder.get(0)).toBe(42);

		// JS mutations after the call are visible through the Java buffer
		view[0] = 99;
		expect(holder.get(0)).toBe(99);

		// and Java mutations are visible through the SharedArrayBuffer
		holder.put(1, 77);
		expect(view[1]).toBe(77);
	});

	it("should share a SharedArrayBuffer's memory with a worker and a Java buffer at once", function (done) {
		var sab = new SharedArrayBuffer(4);
		var view = new Uint8Array(sab);
		view[0] = 0;

		var holder = new com.tns.tests.ByteBufferHolder();
		holder.hold(sab);

		var worker = new Worker("../shared/Workers/EvalWorker.js");
		worker.postMessage({
			value: sab,
			eval: "var v = new Uint8Array(value); v[0] = 42; v[3] = 99; postMessage('written');"
		});
		// fail fast instead of waiting for the jasmine timeout if the worker
		// errors before posting back
		worker.onerror = function (e) {
			expect("worker error: " + e.message).toBe("");
			worker.terminate();
			done();
		};
		worker.onmessage = function (msg) {
			expect(msg.data).toBe("written");
			// the worker's write is visible both to this isolate and to Java
			expect(view[0]).toBe(42);
			expect(view[3]).toBe(99);
			expect(holder.get(0)).toBe(42);
			expect(holder.get(3)).toBe(99);
			worker.terminate();
			done();
		};
	});
});
