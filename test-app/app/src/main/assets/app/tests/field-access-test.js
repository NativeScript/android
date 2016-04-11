describe("Tests Java object field access", function () {
	it("should not crash when Java object protytype is enumerated", function () {
		var count = 0;
		var obj = android.view.ViewGroup.LayoutParams.prototype;
		for(var propertyName in obj) {
			__log(">>obj." + propertyName + "=" + obj[propertyName]);
			++count;
		}
		expect(count > 0).toBe(true);
	});
	
	it("should not crash when Java property is acceess through plain JavaScritp object", function () {
		function createLayoutParams() {}
		createLayoutParams.prototype = new android.view.ViewGroup.LayoutParams(1, 2);
		var exceptionCaught = false;
		try {
			var p = createLayoutParams();
			p.width = 1;
		} catch(e) {
			exceptionCaught = true;
		}
		expect(exceptionCaught).toBe(true);
	});
});