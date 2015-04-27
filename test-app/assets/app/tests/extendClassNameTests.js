var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

//the class name valid symbols are [a-z , A-Z , 0-9, _]
var When_naming_extension_class_user_should_give_valid_name = function() {
	
	var exceptionCaught = false;
	try
	{
		var O = java.lang.Object.extend("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789", {});
	}
	catch(e)
	{
		exceptionCaught = true;
		__log("Validation is wrong");
	}
	
	Assert(exceptionCaught === false, "FAILED: When_naming_extension_class_user_should_give_valid_name.");
}

var When_naming_extension_contains_invalid_symbols_should_throw_exception = function() {
	
	var exceptionCaught = false;
	try
	{
		var O = java.lang.Object.extend("1235!", {}); //[!] is invalid symbol
	}
	catch(e)
	{
		__log('message: ' + e.message);
		exceptionCaught = true;
	}
	
	Assert(exceptionCaught === true, "FAILED: When_naming_extension_doesnt_have_valid_name_should_throw_exception.");
}

When_naming_extension_class_user_should_give_valid_name();
When_naming_extension_contains_invalid_symbols_should_throw_exception();