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

    it("When resolving class with incorrect number of parameters expect exception to be thrown", function () {
    	__log("TEST: When resolving class with incorrect number of parameters expect exception to be thrown");

        var exceptionCaught = false;

        try {
    	    var d = new com.tns.tests.DummyClass(true, false, true);
        } catch (e) {
            exceptionCaught = true;
        }

        expect(exceptionCaught).toBe(true);
    });


    it("When resolving class with correct number of compliant parameters", function () {
    	__log("TEST: When resolving class with correct number of compliant parameters");

        var exceptionCaught = false;
        var d;

        try {
    	    d = new com.tns.tests.DummyClass(new com.tns.tests.DummyClass(), new java.lang.Integer(42));
        } catch (e) {
            exceptionCaught = true;
        }

        expect(exceptionCaught).toBe(false);
        expect(d).not.toEqual(null);
    });
});