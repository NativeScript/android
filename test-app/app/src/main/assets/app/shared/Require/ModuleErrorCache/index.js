try {
    require("./maybeThrow");
} catch(e) {
    TNSLog(e.message);
    TNSLog(e.stack.indexOf('/maybeThrow.js:') > 0 ? "1" : "0");
}

require("./maybeThrow");
