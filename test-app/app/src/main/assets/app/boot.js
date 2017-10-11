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

global.__JUnitSaveResults = function (unitTestResults) {
	var pathToApp = '/data/data/com.tns.testapplication';
	var unitTestFileName = 'android_unit_test_results.xml';
	try {
        var javaFile = new java.io.File(pathToApp, unitTestFileName);
        var stream = new java.io.FileOutputStream(javaFile);
        var actualEncoding = 'UTF-8';
        var writer = new java.io.OutputStreamWriter(stream, actualEncoding);
        writer.write(unitTestResults);
        writer.close();
    }
    catch (exception) {
        android.util.Log.d("TEST RESULTS", 'failed writing to files dir: ' + exception)
    }
};

require('./Infrastructure/Jasmine/jasmine-2.0.1/boot'); //runs jasmine, attaches the junitOutputter


require("./mainpage");