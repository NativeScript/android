//describe("Tests that app does not crashes (no hard-fail asserts)", function () {
//
//	it("When_interface_is_constucted_with_wrong_number_of_arguments", function () {
//		var exceptionThrown = false;
//		try {
//			var r = new java.lang.Runnable(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var r = new java.lang.Runnable(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_interface_is_constucted_with_wrong_type_of_arguments", function () {
//		var exceptionThrown = false;
//		try {
//			var r = new java.lang.Runnable(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var r = new java.lang.Runnable("", null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_class_is_extended_with_wrong_number_of_arguments", function () {
//		var exceptionThrown = false;
//		try {
//			var O = java.lang.Object.extend(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var O = java.lang.Object.extend(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_class_is_extended_with_wrong_type_of_arguments", function () {
//		var exceptionThrown = false;
//		try {
//			var O = java.lang.Object.extend(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var O = java.lang.Object.extend("", null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_byte_cast_function_is_called_with_wrong_number_of_arguments", function () {
//		var f = byte;
//		var exceptionThrown = false;
//		try {
//			var val = f(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_byte_cast_function_is_called_with_wrong_type_of_arguments", function () {
//		var f = byte;
//		var exceptionThrown = false;
//		try {
//			var val = f(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f({});
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(undefined);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(false);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(true);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_short_cast_function_is_called_with_wrong_number_of_arguments", function () {
//		var f = short;
//		var exceptionThrown = false;
//		try {
//			var val = f(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_short_cast_function_is_called_with_wrong_type_of_arguments", function () {
//		var f = short;
//		var exceptionThrown = false;
//		try {
//			var val = f(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f({});
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(undefined);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(false);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(true);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_long_cast_function_is_called_with_wrong_number_of_arguments", function () {
//		var f = long;
//		var exceptionThrown = false;
//		try {
//			var val = f(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_long_cast_function_is_called_with_wrong_type_of_arguments", function () {
//		var f = long;
//		var exceptionThrown = false;
//		try {
//			var val = f(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f({});
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(undefined);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(false);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(true);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_float_cast_function_is_called_with_wrong_number_of_arguments", function () {
//		var f = float;
//		var exceptionThrown = false;
//		try {
//			var val = f(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_float_cast_function_is_called_with_wrong_type_of_arguments", function () {
//		var f = float;
//		var exceptionThrown = false;
//		try {
//			var val = f(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f({});
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(undefined);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(false);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(true);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_double_cast_function_is_called_with_wrong_number_of_arguments", function () {
//		var f = double;
//		var exceptionThrown = false;
//		try {
//			var val = f(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_double_cast_function_is_called_with_wrong_type_of_arguments", function () {
//		var f = double;
//		var exceptionThrown = false;
//		try {
//			var val = f(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f({});
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(undefined);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(false);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(true);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_char_cast_function_is_called_with_wrong_number_of_arguments", function () {
//		var f = char;
//		var exceptionThrown = false;
//		try {
//			var val = f(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_char_cast_function_is_called_with_wrong_type_of_arguments", function () {
//		var f = char;
//		var exceptionThrown = false;
//		try {
//			var val = f(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f({});
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(undefined);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(false);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(true);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(0);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f('abc');
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_require_is_called_with_wrong_number_of_arguments", function () {
//		var exceptionThrown = false;
//		try {
//			var r = require(/* no arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var r = require(1, 2 /* two arguments */);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("When_require_is_called_with_wrong_type_of_arguments", function () {
//		var f = require;
//		var exceptionThrown = false;
//		try {
//			var val = f(null);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f({});
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(undefined);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(false);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(true);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//
//		exceptionThrown = false;
//		try {
//			var val = f(0);
//		} catch(e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("should throw an exception when trying to link invalid this to a Java object", function () {
//		var __extends = function (d, b) {
//			   for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
//			   function __() { this.constructor = d; }
//			   d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
//			};
//		var MyObject = (function (_super) {
//		    __extends(MyObject, _super);
//		    function MyObject(name) {
//		        return _super.call(this) || this;
//		    }
//		    return MyObject;
//		})(java.lang.Object);
//
//		var exceptionThrown;
//		try {
//			var o = new MyObject();
//			exceptionThrown = false;
//		} catch (e) {
//			exceptionThrown = true;
//		}
//		expect(exceptionThrown).toBe(true);
//	});
//
//	it("should not crash the app when __native(this) call is missed in TypeScript constructor", function () {
//		var MyObject = (function (_super) {
//		    __extends(MyObject, _super);
//		    function MyObject(name) {
//		        _super.call(this);
//		    }
//		    MyObject.prototype.hashCode = function () {
//		    	return 123;
//		    }
//		    return MyObject;
//		})(java.lang.Object);
//
//		var o = new MyObject();
//		var hashCode = o.hashCode();
//
//		expect(hashCode).toBe(123);
//	});
//});