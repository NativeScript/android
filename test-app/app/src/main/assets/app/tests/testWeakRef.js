describe("Test WeakRef ", function () {
	it("Test if WeakRef gets cleared after gc", function (done) {
		__log("TEST: TestWeakRefGetsClearedAfterGC");

		var wr = new WeakRef({ someProp: 12345 });

		var val = wr.get().someProp;
		expect(val).toBe(12345);
		gc();

		setTimeout(() => {
			gc();
			var val = wr.get();
			expect(val).toBe(undefined);
			done();
		});
	});

	it("Test if WeakRef can create multiple instances", function (done) {
		__log("TEST: TestWeakRefCanCreateMultipleInstances");

		var target = { someProp: 54321 };

		var wr1 = new WeakRef(target);
		var wr2 = new WeakRef(target);

		target = null;
		setTimeout(() => {
			gc();

			var val1 = wr1.get();
			expect(val1).toBe(undefined);

			var val2 = wr2.get();
			expect(val2).toBe(undefined);
			done();
		});
	});

	it("Test if WeakRef throws exception when constructed with wrong number of parameters", function () {
		__log(
			"TEST: TestWeakRefThrowsExceptionWhenConstructedWithWrongNumberOfParameters"
		);

		var exceptionCaught = false;
		try {
			new WeakRef();
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			new WeakRef(1, 2);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
	});

	it("Test if WeakRef throws exception when constructed with non object", function () {
		__log("TEST: TestWeakRefThrowsExceptionWhenConstructedWithNonObject");

		var exceptionCaught = false;
		try {
			new WeakRef(1);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			new WeakRef(false);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			new WeakRef(null);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);

		exceptionCaught = false;
		try {
			new WeakRef(undefined);
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
	});
});
