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

// Release APKs are not debuggable, so `run-as` cannot read the app's data
// directory; the external files dir is readable with a plain `adb pull`.
global.__nsPgoDump = function (tag) {
	if (typeof __pgoDump !== "function") {
		return;
	}
	var dir = com.tns.NativeScriptApplication.getInstance().getExternalFilesDir(null).getAbsolutePath();
	var rc = __pgoDump(dir + "/ns-" + tag + ".profraw");
	console.log("PGO_DUMP " + tag + " rc=" + rc + " " + dir);
};

global.__JUnitSaveResults = function (unitTestResults) {
	global.__nsPgoDump("suite");
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

if (com.tns.tests.JniBench.isEnabled()) {
	require("./tests/benchmarks/jniBenchmarks").run();
} else {
	require('./Infrastructure/Jasmine/boot');

	require("./mainpage");
}