describe("Tests mapped ByteBuffer conversion", function () {
	it("should convert ByteBuffer to ArrayBuffer [Direct ByteBuffer]", function () {
		var bb = java.nio.ByteBuffer.allocateDirect(12);
		var ab = ArrayBuffer.from(bb);
		var int8arr = new Int8Array(ab);
		expect(int8arr.length).toBe(12);
		var int32arr = new Int32Array(ab);
		expect(int32arr.length).toBe(3);
	});

	it("should convert ByteBuffer to ArrayBuffer [Indirect ByteBuffer]", function () {
        var bb = java.nio.ByteBuffer.allocate(12);
        var ab = ArrayBuffer.from(bb);
        var int8arr = new Int8Array(ab);
        expect(int8arr.length).toBe(12);
        var int32arr = new Int32Array(ab);
        expect(int32arr.length).toBe(3);
    });

	it("should share the same memory of all typed arrays [Direct ByteBuffer]", function () {
		var bb = java.nio.ByteBuffer.allocateDirect(12);
		var ab = ArrayBuffer.from(bb);
		var int8arr = new Int8Array(ab);
		expect(int8arr.length).toBe(12);
		var int32arr = new Int32Array(ab);
		expect(int32arr.length).toBe(3);
		int8arr[0] = 0x11;
		int8arr[1] = 0x22;
		int8arr[2] = 0x33;
		int8arr[3] = 0x44;
		var value = int32arr[0];
		expect(value).toBe(0x44332211);
	});

	it("should share the same memory of all typed arrays [Indirect ByteBuffer]", function () {
        var bb = java.nio.ByteBuffer.allocate(12);
        var ab = ArrayBuffer.from(bb);
        var int8arr = new Int8Array(ab);
        expect(int8arr.length).toBe(12);
        var int32arr = new Int32Array(ab);
        expect(int32arr.length).toBe(3);
        int8arr[0] = 0x11;
        int8arr[1] = 0x22;
        int8arr[2] = 0x33;
        int8arr[3] = 0x44;
        var value = int32arr[0];
        expect(value).toBe(0x44332211);
    });

	it("should keep original ByteBuffer after conversion [Direct ByteBuffer]", function () {
		var bb = java.nio.ByteBuffer.allocateDirect(12);
		var ab = ArrayBuffer.from(bb);
		var same = bb === ab.nativeObject;
		expect(same).toBe(true);
	});

	it("should keep original ByteBuffer after conversion [Indirect ByteBuffer]", function () {
        var bb = java.nio.ByteBuffer.allocate(12);
        var ab = ArrayBuffer.from(bb);
        var same = bb === ab.nativeObject;
        expect(same).toBe(true);
    });

	it("should throw exception when ArrayBuffer.from is called with wrong number of arguments", function () {
		var exceptionCaught = false;
		try {
			var ab = ArrayBuffer.from(1, 2);
		} catch(e) {
			exceptionCaught = true
		}
		expect(exceptionCaught).toBe(true);
	});

	it("should throw exception when ArrayBuffer.from is called with wrong argument type", function () {
		var exceptionCaught = false;
		try {
			var ab = ArrayBuffer.from({});
		} catch(e) {
			exceptionCaught = true
		}
		expect(exceptionCaught).toBe(true);
	});
});