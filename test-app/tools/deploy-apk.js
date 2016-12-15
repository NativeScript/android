// This node app deploys apk file on device
// Parameters:
//  - Path to APK to deploy
//  - runOnDevice flag (optional)

var apk = process.argv[2],
    exec = require('child_process').exec,
    deployTimeout = 3 * 60 * 1000, // 3 minutes to deploy and launch.
    defaultPort = require("./config.json")["emulator-port"],
    emulatorName = "emulator-" + defaultPort + " ",
    adb = process.env.ANDROID_HOME + "/platform-tools/adb ",
    runner = require("./interval-runner-with-timeout.js");

if (process.argv.length < 2) {
    console.error('Expect path to apk file to install');
    process.exit(1);
}

runner.runFunctionWithIntervalAndTimeout(checkIfEmulatorFullyStarted, 10 * 1000, 60 * 10 * 1000)

function checkIfEmulatorFullyStarted() {
    var res = exec(adb + "-s " + emulatorName + " shell getprop init.svc.bootanim");
    res.stdout.on("data", function (data) {
        if(data.indexOf("stopped") !== -1) {
            console.log("emulator annimation has stopped");
            installApk()
        }
    })
}

function timeoutFunction(msg) {
    console.error(msg);
    testrun.kill();
    process.exit(-1);
};


function installApk() {
    console.log("Executing adb install");
    var testrun = exec(adb + "-s " + emulatorName + " install -r " + apk, function (error, stdout, stderr) {
            if (error) {
                console.error("Deply apk failed: " + error);
                process.exit(-2);
            }
            process.exit(0);
        });
    testrun.stdout.pipe(process.stdout, { end: false });
    testrun.stderr.pipe(process.stderr, { end: false });
}