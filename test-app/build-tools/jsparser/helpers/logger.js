var fs = require('fs'),
    path = require('path'),
    os = require('os'),
    fileHelpers = require("./file_helpers")();

module.exports = function (setting) {
    var logDirectory = path.dirname(setting.logPath);

    // TODO: Logging to file disabled temporarily, console output is default
    
    // if (!fs.existsSync(logDirectory)) {
    //     console.error("couldn't find logDirectory so it will be created in place:" + setting.logPath);
    //     fileHelpers.ensureDirectories(setting.logPath);
    // }
    // if (os.type().indexOf('Windows') === -1) {
    //     var appLogStat = fs.statSync(logDirectory);
    //     if (canWrite(process.uid === appLogStat.uid, process.gid === appLogStat.gid, appLogStat.mode)) {
    //         console.error("ERROR WRITING TO LOG FILE DIRECTORY : " + logDirectory);
    //         process.exit(-1);
    //     }
    // }

    var appLog = createLog(setting.APP_NAME, logDirectory, setting);

    if (setting.disable) {
        for (var prop in appLog) {
            appLog[prop] = function () { };
        }
    }

    return appLog;
};

function canWrite(owner, inGroup, mode) {
    return owner && (mode & 00200) || // User is owner and owner can write.
        inGroup && (mode & 00020) || // User is in group and group can write.
        (mode & 00002); // Anyone can write.
}

function createLog(appName, logDirectory, setting) {

    var appLog = {};

    function getRequestId() {
        return (process.domain && process.domain.id) || "";
    }

    function getLogDate() {
        var today = new Date();

        var fullYear = today.getFullYear();
        var month = today.getMonth();
        var day = today.getDate();
        var hours = today.getHours();
        var minutes = today.getMinutes();
        var seconds = today.getSeconds();

        var logDate = fullYear + "-" + month + "-" + day + "/" + hours + ":" + minutes + ":" + seconds;
        return logDate;
    }

    function setMessageWithFormat(message) {
        var res = getLogDate() + "\t" + message;
        return res;
    }

    function appendToLogFile(line) {
        var logFile = logDirectory + path.sep + appName + ".log";
        var augmentedLine = getLogDate() + "\t" + line + os.EOL
        fs.appendFile(logFile, augmentedLine, function (err) {
            if (err) {
                throw "couldn't write to " + logFile;
            }
        });
    }

    appLog.log = function (input) {
        console.log(setMessageWithFormat(input));
    }

    appLog.info = appLog.log;

    appLog.warn = function (input) {
        console.warn(setMessageWithFormat(input));
    }

    appLog.error = function (input) {
        console.error(setMessageWithFormat(input));
    }

    return appLog;
}
