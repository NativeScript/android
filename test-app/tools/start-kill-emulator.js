var runner = require("./interval-runner-with-timeout.js"),
    execSync = require('child_process').execSync,
    exec = require('child_process').exec,
    adb = process.env.ANDROID_HOME + "/platform-tools/adb ";

if(process.argv.length <= 2) {
    console.log("You need to pass 'kill' or 'start'");
    process.exit(1);
}
var action = process.argv[2];
if(action === "kill") {
    runner.runFunctionWithIntervalAndTimeout(stopEmulatorsAndExit, 1000, 10 * 1000);
}
else if(action === "start") {
    //try starting emulator every 10 seconds, but try maximum of 60 seconds
    startEmulator();
}
else {
    console.log("You need to pass an action. Possible actions are 'start' and 'kill'");
    process.exit(1);
}

function startEmulator() {
    console.log("Stopping previous emulators if any")
    stopEmulators();

    wrapedExec(adb + " start-server");
    var emulator = "Emulator-Api19-Default"
    if(!process.argv[3]) {
        console.log("You didn't specify emulator so we'll start the default one: " + emulator)
    }
    else {
        emulator = process.argv[3].trim();
    }
    console.log("Starting emulator " + emulator)
    var out = exec("$ANDROID_HOME/tools/emulator -avd " + emulator + " -wipe-data -gpu on -no-window &")
    out.stdout.on("data", function (data) {
        console.log(data)
        runner.runFunctionWithIntervalAndTimeout(checkAvailable, 1000, 10 * 1000);
    })
    if(out.stderr) {
        out.stderr.on("data", function (data) {
            console.log("Error while launching emulator: " + data);
            process.exit(-1)
        })
    }
}

function stopEmulatorsAndExit() {
    stopEmulators();
    process.exit(0);
}

function stopEmulators() {
    var out = wrapedExec(adb + " devices | grep emulator | cut -f1 | while read line; do " + adb + " -s $line emu kill; done");
    if(!out.stderr) {
        console.log("Killed emulators!")
    }
}

function checkAvailable() {
    var out = exec(adb + " devices")
    out.stdout.on("data", function (data) {
        if(data.indexOf("offline") === -1) {
            console.log("Emulator is online")
            process.exit(0);
        }
        console.log(data)
    })
}

function wrapedExec(command) {
    try {
        return execSync(command);
    } catch(e) {
        console.log("err: " + e);
    }
}