// A sample code how to catch uncaught errors
asglobal.__onUncaughtError = function(error){
	if(error.nativeException){
		Log("err.message: " + error.message);
		Log("err.stackTrace: " + error.stackTrace);
		// false == do not continue execution
		return false;
	}
	
	// true == continue execution
	return true;
}

require("./mainpage");