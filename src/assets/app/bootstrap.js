// A sample code how to catch uncaught errors
global.__onUncaughtError = function(error){
	if(error.nativeException){
		// false == do not continue execution
		return false;
	}
	
	// true == continue execution
	return true;
}

require("./mainpage");