// A sample code how to catch uncaught errors
global.__onUncaughtError = function(error){
	if(error.nativeException){
		// false == do not continue executionW
		Log("ERROR.MESSAGE:---- " + error.message);
		Log("ERROR.STACK_TRACE:---- " + error.stackTrace);
		
		return false;
	}
	
	// true == continue execution
	return true;
}

require("./mainpage");