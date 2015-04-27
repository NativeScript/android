global.__onUncaughtError = function(error){
	if(error.nativeException){
		__log("err.message: " + error.message);
		__log("err.stackTrace: " + error.stackTrace);
		// false == do not continue execution
		return false;
	}
	
	// true == continue execution
	return true;
}

require("./mainpage");