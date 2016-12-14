var runner = require("./interval-runner-with-timeout.js"),
    exec = require('child_process').exec,
    runOnDeviceOrEmulator = "-e";

if(process.argv[2]) {
	runOnDeviceOrEmulator = process.argv[2];  
}

runner.runFunctionWithIntervalAndTimeout(startApk, 10 * 1000, 60 * 1000);

function startApk() {
    console.log("starting application")
    var res = exec("$ANDROID_HOME/platform-tools/adb " + runOnDeviceOrEmulator + " shell am start -n com.tns.android_runtime_testapp/com.tns.NativeScriptActivity -a android.intent.action.MAIN -c android.intent.category.LAUNCHER", function (error, stdout, stderr) {
            if (error) {
                console.error("Starting application failed: " + error);
                process.exit(-2);
            }
        });
    res.stdout.on("data", function (data) {
        console.log("Started application successfully");
        process.exit(0);
    })
}
