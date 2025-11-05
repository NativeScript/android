//debugger;

global.__onUncaughtError = function(error){
	if(error.nativeException){
		__log("err.message: " + error.message);
		__log("err.stackTrace: " + error.stackTrace);

		//stop execution
		return false;
	}
	
	//continue execution
	return true;
}

require('./Infrastructure/timers');

require('./Infrastructure/Jasmine/jasmine-2.0.1/boot'); //runs jasmine, attaches the junitOutputter


require("./mainpage");