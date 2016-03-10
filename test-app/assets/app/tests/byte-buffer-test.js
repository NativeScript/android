describe("Tests mapped ByteBuffer conversion", function () {
	it("should convert ByteBuffer to ArrayBuffer", function () {
		var bb = com.tns.Platform.allocateByteBuffer(12);
		var ab = ArrayBuffer.from(bb);
		var int8arr = new Int8Array(ab);
		expect(int8arr.length).toBe(12);
		var int32arr = new Int32Array(ab);
		expect(int32arr.length).toBe(3);
	});

	it("should keep original ByteBuffer after conversion", function () {
		var bb = com.tns.Platform.allocateByteBuffer(12);
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