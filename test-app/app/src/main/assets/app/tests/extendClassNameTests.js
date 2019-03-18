//describe("Tests extend class name ", function () {
//
//	var myCustomEquality = function(first, second) {
//		return first == second;
//	};
//
//	beforeEach(function() {
//		jasmine.addCustomEqualityTester(myCustomEquality);
//	});
//
//	//the class name valid symbols are [a-z , A-Z , 0-9, _]
//	it("When_naming_extension_class_user_should_give_valid_name", function () {
//
//		var exceptionCaught = false;
//		try
//		{
//			var O = java.lang.Object.extend("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789", {});
//		}
//		catch(e)
//		{
//			exceptionCaught = true;
//			__log("Validation is wrong");
//		}
//
//		expect(exceptionCaught).toBe(false);
//	});
//
//	it("When_naming_extension_contains_invalid_symbols_should_throw_exception", function () {
//
//		var exceptionCaught = false;
//		try
//		{
//			var O = java.lang.Object.extend("1235!", {}); //[!] is invalid symbol
//		}
//		catch(e)
//		{
//			__log('message: ' + e.message);
//			exceptionCaught = true;
//		}
//
//		expect(exceptionCaught).toBe(true);
//	});
//});