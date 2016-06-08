// This node app deploys apk file on device
// Parameters:
//  - Path to APK to deploy

if (process.argv.length < 2) {
    console.error('Expect path to apk file to install');
    process.exit(1);
}

var apk = process.argv[2];

var proc = require('child_process');

var deployTimeout = 180000; // 3 minutes to deploy and launch.

var cmd = 'adb install -r ' + apk;

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