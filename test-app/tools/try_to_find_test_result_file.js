var execFindFile = require('child_process').exec,
	runner = require("./interval-runner-with-timeout.js"),
	exec = require('child_process').exec,
	fs = require('fs'),
	pullfile,
	processTimeout = 20 * 60 * 1000, // 20 minutes timeout (empirical constant :)) 
	searchInterval = 10 * 1000;

var runOnDeviceOrEmulator = "-e";
if(process.argv[2]) {
	runOnDeviceOrEmulator = process.argv[2];
}

runner.runFunctionWithIntervalAndTimeout(checkIfAppIsRunning, searchInterval, processTimeout);
runner.runFunctionWithIntervalAndTimeout(tryToGetFile, searchInterval, processTimeout);

function checkIfAppIsRunning() {
	exec("adb " + runOnDeviceOrEmulator + " shell \"ps | grep com.tns.android_runtime_testapp\"", checkIfProcessIsRunning);
}

function checkIfProcessIsRunning(err, stdout, stderr) {
	if(stdout) {
		console.log("com.tns.android_runtime_testapp process is running")
	}
	else {
		console.log('com.tns.android_runtime_testapp process died!');
		process.exit(1);
	}
}

function tryToGetFile() {
	pullfile = execFindFile("adb " + runOnDeviceOrEmulator + " pull /sdcard/android_unit_test_results.xml", checkIfFileExists);
	pullfile.stdout.pipe(process.stdout, { end: false });
	pullfile.stderr.pipe(process.stderr, { end: false });
}

function checkIfFileExists(err, stout, stderr) {

	//if you find file in sdcard exit process
	if (!err) {
		console.log('Tests results file found file!');
		process.exit(0);
	}
	else {
		//if the time to get the file is out exit process
		console.log('Tests results file not found!');
	}
}
