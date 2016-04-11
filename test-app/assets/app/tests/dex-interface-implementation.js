describe("Tests partial interface implementation", function () {
	it("should throw an exception when call unimplemented method of a partial implemented interface", function () {
		var exceptionCaught = false;
		var m = new java.util.Map({
			clear: function() {}
		});
		try {
			m.size()
		} catch (e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
	});

	it("should call implemented method of a partial implemented interface", function () {
		var m = new java.util.Map({
			hashCode: function() {
				return 123;
			}
		});
		var hash = m.hashCode();
		expect(hash).toBe(123);
	});
});