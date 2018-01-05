
var checkBootAnimationPropertyExec = require('child_process').exec,
	checkAnimExec = require('child_process').exec,
	isTimeToExit = false,
	processTimeout = 10 * 60 * 1000, // 10 minutes
	searchInterval = 2 * 1000;

var runOnDeviceOrEmulator = process.argv[2];

checkBootAnimationPropertyExec("", check)

function check() {
	closeProcessAfter(processTimeout);
	setInterval(tryToCheckProperty, searchInterval);
}

function tryToCheckProperty() {
	var checkApp = checkAnimExec("adb " + runOnDeviceOrEmulator + " shell getprop init.svc.bootanim", checkCommandOutput);

	checkApp.stdout.pipe(process.stdout, { end: false });
	checkApp.stderr.pipe(process.stderr, { end: false });
}

function checkCommandOutput(err, stdout, stderr) {
	if (stdout) {
		if (stdout.trim() === "stopped") {
			process.exit(0);
		}
	}
	else {
		if (err.toString().indexOf("device offline") == -1) {
			process.exit(1);
			console.log(`${err}`);
		}
	}
	console.log("waiting for device to start up...")
}

function closeProcessAfter(timeout) {
	//this will force process closed even if the the file is not found
	setTimeout(function () { isTimeToExit = true; }, timeout);
}
