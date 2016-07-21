// This node app deploys apk file on device
// Parameters:
//  - Path to APK to deploy
var fs = require('fs');
var path = require('path');

function printHelp() {
    console.error('Usage: deploy-apk.js path-to-apk/myapp.apk <device target>');
    console.error('                                           -e: target the single available emulator');
    console.error('                                           -d: target the single available device');
    console.error('                                           -s device: target a device by its identifier');
}

if (process.argv.length < 2) {
    console.error('Invalid argument no apk file specified.');
    printHelp();
    process.exit(-3);
}

var apk = process.argv[2];
if (!fs.existsSync(apk)) {
    console.error("Apk file not found at path:" + apk)
    var parentDir = path.dirname(apk);
    var parentDirFiles = fs.readdirSync(parentDir);
    console.error("Apk search directory contains" + parentDirFiles);
    
    console.error("Installation failed");
    process.exit(-4);
}

var runOnDeviceOrEmulator = process.argv[3];

var proc = require('child_process');

var deployTimeout = 3 * 60 * 1000; // 3 minutes to deploy and launch.

var cmd = 'adb '+ runOnDeviceOrEmulator  +' install -r ' + apk;

function timeoutFunction(msg) {
    console.error(msg);
    testrun.kill();
    process.exit(-1);
};

var timeout = setTimeout(function () { timeoutFunction("ERROR: Deploy timeout!"); }, deployTimeout);

console.log("Executing adb install: " + cmd);
var testrun = proc.exec(cmd, function (error, stdout, stderr) {
    // If the process exits prematurely kill the timer anyway...
    clearTimeout(timeout);

    if (error) {
        console.error("Deply apk failed: " + error);
        process.exit(-2);
    }
});
testrun.stdout.pipe(process.stdout, { end: false });
testrun.stderr.pipe(process.stderr, { end: false });
