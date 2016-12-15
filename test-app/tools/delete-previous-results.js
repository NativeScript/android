var runner = require("./interval-runner-with-timeout.js"),
    exec = require('child_process').exec,
    adb = process.env.ANDROID_HOME + "/platform-tools/adb ",
    defaultPort = require("./config.json")["emulator-port"],
    emulatorName = "emulator-" + defaultPort + " ",
    xmlName = "android_unit_test_results.xml";


runner.runFunctionWithIntervalAndTimeout(removePreviousXml, 1000, 10 * 1000);

function removePreviousXml() {
    var out = exec(adb + "-s " + emulatorName + " shell rm -rf /sdcard/" + xmlName, function (err, stdout, stderr) {
        if(stderr) {
            console.log("Couldn't delete :" + xmlName);
            if(stderr.indexOf(xmlName + "' does not exist")) {
                process.exit(0);
            }
            process.exit(-1);
        }
        process.exit(0);
    })
}