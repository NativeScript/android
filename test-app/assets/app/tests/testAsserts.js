describe("Tests that app does not crashes (no hard-fail asserts)", function () {
	
	it("When_interface_is_constucted_with_wrong_number_of_arguments", function () {
		var exceptionThrown = false;
		try {
			var r = new java.lang.Runnable(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var r = new java.lang.Runnable(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});
	
	it("When_interface_is_constucted_with_wrong_type_of_arguments", function () {
		var exceptionThrown = false;
		try {
			var r = new java.lang.Runnable(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var r = new java.lang.Runnable("", null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_class_is_extended_with_wrong_number_of_arguments", function () {
		var exceptionThrown = false;
		try {
			var O = java.lang.Object.extend(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var O = java.lang.Object.extend(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});
	
	it("When_class_is_extended_with_wrong_type_of_arguments", function () {
		var exceptionThrown = false;
		try {
			var O = java.lang.Object.extend(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var O = java.lang.Object.extend("", null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});
	
	it("When_byte_cast_function_is_called_with_wrong_number_of_arguments", function () {
		var f = byte;
		var exceptionThrown = false;
		try {
			var val = f(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_byte_cast_function_is_called_with_wrong_type_of_arguments", function () {
		var f = byte;
		var exceptionThrown = false;
		try {
			var val = f(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f({});
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(undefined);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(false);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(true);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_short_cast_function_is_called_with_wrong_number_of_arguments", function () {
		var f = short;
		var exceptionThrown = false;
		try {
			var val = f(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_short_cast_function_is_called_with_wrong_type_of_arguments", function () {
		var f = short;
		var exceptionThrown = false;
		try {
			var val = f(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f({});
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(undefined);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(false);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(true);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_long_cast_function_is_called_with_wrong_number_of_arguments", function () {
		var f = long;
		var exceptionThrown = false;
		try {
			var val = f(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_long_cast_function_is_called_with_wrong_type_of_arguments", function () {
		var f = long;
		var exceptionThrown = false;
		try {
			var val = f(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f({});
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(undefined);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(false);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(true);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});
	
	it("When_float_cast_function_is_called_with_wrong_number_of_arguments", function () {
		var f = float;
		var exceptionThrown = false;
		try {
			var val = f(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_float_cast_function_is_called_with_wrong_type_of_arguments", function () {
		var f = float;
		var exceptionThrown = false;
		try {
			var val = f(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f({});
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(undefined);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(false);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(true);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_double_cast_function_is_called_with_wrong_number_of_arguments", function () {
		var f = double;
		var exceptionThrown = false;
		try {
			var val = f(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_double_cast_function_is_called_with_wrong_type_of_arguments", function () {
		var f = double;
		var exceptionThrown = false;
		try {
			var val = f(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f({});
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(undefined);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(false);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(true);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});
	
	it("When_char_cast_function_is_called_with_wrong_number_of_arguments", function () {
		var f = char;
		var exceptionThrown = false;
		try {
			var val = f(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_char_cast_function_is_called_with_wrong_type_of_arguments", function () {
		var f = char;
		var exceptionThrown = false;
		try {
			var val = f(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f({});
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(undefined);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(false);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(true);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(0);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f('abc');
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});
	
	it("When_require_is_called_with_wrong_number_of_arguments", function () {
		var exceptionThrown = false;
		try {
			var r = require(/* no arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var r = require(1, 2 /* two arguments */);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});

	it("When_require_is_called_with_wrong_type_of_arguments", function () {
		var f = require;
		var exceptionThrown = false;
		try {
			var val = f(null);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f({});
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(undefined);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(false);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
		
		exceptionThrown = false;
		try {
			var val = f(true);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);

		exceptionThrown = false;
		try {
			var val = f(0);
		} catch(e) {
			exceptionThrown = true;
		}
		expect(exceptionThrown).toBe(true);
	});
});