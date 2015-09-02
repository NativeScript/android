describe("Test Method Resolution", function () {

	it("When_call_overloaded_base_method_it_should_call_base_method1", function () {
		
		__log("TEST: When_call_overloaded_base_method_it_should_call_base_method1");
		
		var d = new com.tns.tests.DerivedClass();

		var value = d.sum(1, 2);
		
		expect(value).toBe(3);
	});
	
	it("When_call_overloaded_base_method_it_should_call_base_method2", function () {
		
		__log("TEST: When_call_overloaded_base_method_it_should_call_base_method2");

		var d = new com.tns.tests.DerivedClass2();

		var value = d.mul(1, 2);
		
		expect(value).toBe(2);
	});

});