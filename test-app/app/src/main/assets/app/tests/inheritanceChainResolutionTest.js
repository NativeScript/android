describe("Tests inheritance chain resolution", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("TestCallWithStringArgumentWhenThereIsStringOverload", function () {
		
		__log("TEST: TestCallWithStringArgumentWhenThereIsStringOverload");
		
		var i = new com.tns.tests.InheritanceChainResolutionTest();
		
		var s = i.echo1("test123");
		
		expect(s).toBe("String=test123");
	});
	
	it("TestCallWithStringArgumentWhenThereIsNoStringOverload", function () {
		
		__log("TEST: TestCallWithStringArgumentWhenThereIsNoStringOverload");
		
		var i = new com.tns.tests.InheritanceChainResolutionTest();
		
		var s = i.echo2("test123");
		
		expect(s).toBe("Object=test123");
	});
});