describe("Tests primitive type conversion", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("convert_char", function () {
		
		var logged =  null;
		
		var logger = new com.tns.tests.CharConversionTest.Logger({
		    print: function(value) {
		        logged = value;
		    }
		});

		com.tns.tests.CharConversionTest.printWithChar(logger, 'x');
		
		expect(logged).toBe('a');
	});
	

});