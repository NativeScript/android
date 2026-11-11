describe("Typed arrays passed to Java parameters", function () {
	var T = com.tns.tests.TypedArrayArgs;

	function toArray(javaArray) {
		var result = [];
		for (var i = 0; i < javaArray.length; i++) {
			result.push(javaArray[i]);
		}
		return result;
	}

	it("copies a Uint8Array into a byte[] parameter", function () {
		var u8 = new Uint8Array([1, 2, 127, 128, 255]);
		var bb = java.nio.ByteBuffer.wrap(u8);

		expect(bb.isDirect()).toBe(false);
		expect(bb.capacity()).toBe(5);
		expect(toArray(bb.array())).toEqual([1, 2, 127, -128, -1]);

		u8[0] = 9;
		expect(bb.array()[0]).toBe(1);
		bb.array()[1] = 42;
		expect(u8[1]).toBe(2);
	});

	it("copies only the bytes a view covers", function () {
		var all = new Uint8Array([0, 1, 2, 3, 4, 5, 6, 7]);
		expect(toArray(T.echoBytes(new Uint8Array(all.buffer, 2, 3)))).toEqual([2, 3, 4]);
		expect(toArray(T.echoBytes(all.subarray(6)))).toEqual([6, 7]);
		expect(toArray(T.echoInts(new Int32Array(new ArrayBuffer(16), 4, 2)))).toEqual([0, 0]);
	});

	it("copies byte-sized arrays, ArrayBuffers and DataViews into byte[]", function () {
		var bytes = [1, -1, 127, -128];
		var buffer = new Int8Array(bytes).buffer;

		expect(toArray(T.echoBytes(new Int8Array(bytes)))).toEqual(bytes);
		expect(toArray(T.echoBytes(new Uint8ClampedArray([1, 255, 127, 128])))).toEqual(bytes);
		expect(toArray(T.echoBytes(buffer))).toEqual(bytes);
		expect(toArray(T.echoBytes(new DataView(buffer, 1, 2)))).toEqual([-1, 127]);

		var shared = new SharedArrayBuffer(2);
		new Uint8Array(shared).set([5, 6]);
		expect(toArray(T.echoBytes(shared))).toEqual([5, 6]);
	});

	it("copies each typed array into the primitive array of its element type", function () {
		expect(toArray(T.echoShorts(new Int16Array([-32768, 1, 32767])))).toEqual([-32768, 1, 32767]);
		expect(toArray(T.echoShorts(new Uint16Array([65535, 2])))).toEqual([-1, 2]);
		expect(toArray(T.echoChars(new Uint16Array([0x41, 0x7a])))).toEqual(["A", "z"]);
		expect(toArray(T.echoInts(new Int32Array([-2147483648, 3, 2147483647])))).toEqual([-2147483648, 3, 2147483647]);
		expect(toArray(T.echoInts(new Uint32Array([4294967295, 4])))).toEqual([-1, 4]);
		expect(toArray(T.echoLongs(new BigInt64Array([-5n, 6n])))).toEqual([-5, 6]);
		expect(toArray(T.echoLongs(new BigUint64Array([7n])))).toEqual([7]);
		expect(toArray(T.echoFloats(new Float32Array([0.5, -1.25])))).toEqual([0.5, -1.25]);
		expect(toArray(T.echoDoubles(new Float64Array([0.1, -1e300])))).toEqual([0.1, -1e300]);
	});

	it("converts other element types the way a plain array with the same values converts", function () {
		var values = [1.9, -2.5, 300, 70000, -70000];
		expect(toArray(T.echoInts(new Float64Array(values)))).toEqual(toArray(T.echoInts(values)));
		expect(toArray(T.echoBytes(new Float64Array(values)))).toEqual(toArray(T.echoBytes(values)));
		expect(toArray(T.echoShorts(new Int32Array([70000, -1])))).toEqual(toArray(T.echoShorts([70000, -1])));
		expect(toArray(T.echoBytes(new Int32Array([1, 255, 256, -1])))).toEqual([1, -1, 0, -1]);
		expect(toArray(T.echoLongs(new Int32Array([-5, 7])))).toEqual([-5, 7]);
		expect(toArray(T.echoDoubles(new Float32Array([0.5, 1.25])))).toEqual([0.5, 1.25]);
		expect(toArray(T.echoBooleans(new Uint8Array([0, 1, 2])))).toEqual([false, true, true]);
	});

	it("converts a typed array for an object array parameter the way a plain array converts", function () {
		expect(T.describeObjects(new Int32Array([1, 2]))).toBe(T.describeObjects([1, 2]));
		expect(T.describeObjects(new Float64Array([0.5]))).toBe(T.describeObjects([0.5]));
	});

	it("rejects element types that have no conversion to the parameter's", function () {
		expect(function () { T.echoInts(new BigInt64Array(2)); }).toThrow();
		expect(function () { T.echoInts(new ArrayBuffer(8)); }).toThrow();
		expect(function () { T.echoInts(new DataView(new ArrayBuffer(8))); }).toThrow();
		expect(function () { T.describeObjects(new ArrayBuffer(8)); }).toThrow();
		expect(function () { T.describeObjects(new BigInt64Array(1)); }).toThrow();
		expect(function () { T.countRows(new Int32Array(4)); }).toThrow();
	});

	it("copies empty typed arrays", function () {
		expect(T.echoBytes(new Uint8Array(0)).length).toBe(0);
		expect(T.echoDoubles(new Float64Array(0)).length).toBe(0);
		expect(T.echoBytes(new ArrayBuffer(0)).length).toBe(0);
		expect(T.echoInts([]).length).toBe(0);
	});

	it("still passes a typed array to an object parameter as a buffer over its memory", function () {
		var ints = new Int32Array(4);
		expect(T.capacityOf(ints)).toBe(4);
		expect(T.capacityOf(new Uint8Array(3))).toBe(3);
		// A typed array that is already linked to a buffer is still copied into byte[].
		var bytes = new Uint8Array([3, 4]);
		expect(T.capacityOf(bytes)).toBe(2);
		expect(toArray(T.echoBytes(bytes))).toEqual([3, 4]);
	});

	// Each of these used to pin a JNI global reference, and ART aborts the process once
	// its global reference table (51200 entries) overflows.
	it("passes more typed arrays to object parameters than fit in the global reference table", function () {
		var count = 60000;
		var total = 0;
		for (var i = 0; i < count; i++) {
			total += T.capacityOf(new Uint8Array(16));
		}
		expect(total).toBe(count * 16);
	});

	it("passes more typed arrays as Object[] elements than fit in the global reference table", function () {
		var count = 60000;
		var items = new Array(count);
		for (var i = 0; i < count; i++) {
			items[i] = new Float32Array(2);
		}
		expect(T.totalCapacity(items)).toBe(count * 2);
	});

	it("lets Java collect the buffer once JS drops the typed array", function (done) {
		var weak = (function () {
			return new java.lang.ref.WeakReference(new Uint8Array(1024));
		})();
		var attempts = 0;
		function collect() {
			gc();
			java.lang.System.gc();
			if (weak.get() === null) {
				done();
			} else if (++attempts >= 40) {
				expect(weak.get()).toBeNull();
				done();
			} else {
				setTimeout(collect, 50);
			}
		}
		setTimeout(collect, 0);
	});

	it("keeps the typed array's memory alive while Java holds the buffer", function () {
		var holder = new com.tns.tests.ByteBufferHolder();
		(function () {
			var bytes = new Uint8Array(4096);
			bytes[0] = 42;
			bytes[4095] = 7;
			holder.hold(bytes);
		})();
		for (var i = 0; i < 3; i++) {
			gc();
			java.lang.System.gc();
		}
		expect(holder.get(0)).toBe(42);
		expect(holder.get(4095)).toBe(7);
	});

	it("converts a JS array too large for the thread's stack", function () {
		var count = 4 * 1024 * 1024;
		var values = new Array(count);
		var expected = 0;
		for (var i = 0; i < count; i++) {
			values[i] = i & 0xff;
			expected += i & 0xff;
		}
		expect(T.sumInts(values)).toBe(expected);
	});
});
