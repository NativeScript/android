describe("Tests array operations", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("TestArraySize", function () {
		
		var size = 12345;
		
		var arr = java.lang.reflect.Array.newInstance(java.lang.Object.class, size);
		
		expect(arr.length).toBe(size);
	});
	
	it("TestArrayOperations", function () {
		
		// keep 'size' value large (>512) in order to test for JNI reference leaks as well
		var size = 10000;
		
		var arr = java.lang.reflect.Array.newInstance(java.lang.Integer.class, size);
		
		expect(arr.length).toBe(size);
		
		for (var i = 0; i < size; i++) {
			arr[i] = java.lang.Integer.valueOf(i);
		}
		
		for (var i = 0; i < size; i++) {
			var n = arr[i].intValue();
			expect(n).toBe(i);
		}
	});

});