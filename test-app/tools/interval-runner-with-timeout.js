var exec = require("child_process").exec;

var runner = (function () {
    var isTimeToExit = false;
    var defaultInterval =  1000; //1sec
    var defaultTimeout =  10 * 1000; //10sec

    function closeProcessAfter(timeout) {
        setTimeout(function () { isTimeToExit = true; }, timeout);
    }
    function runWithInterval(commandCallback, interval) {
        if(isTimeToExit) {
            process.exit(1);
        }
        commandCallback();
        setInterval(commandCallback, interval);
    }

    function runFunctionWithIntervalAndTimeout(commandCallback, interval, timeout) {
        if(!interval) {
            console.log("Setting default interval: " + defaultInterval);
            interval = defaultInterval;
        }
        if(!timeout) {
            console.log("Setting default timeout: " + defaultTimeout);
            timeout = defaultTimeout;
        }
        closeProcessAfter(timeout);
        runWithInterval(commandCallback, interval);
    }
    return {
        runFunctionWithIntervalAndTimeout: runFunctionWithIntervalAndTimeout
    }
})();
module.exports = runner;