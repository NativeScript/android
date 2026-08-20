// Worker realms carry only the native __ns__-prefixed timers (the friendly
// setTimeout global is installed by userland, e.g. @nativescript/core).
__ns__setTimeout(function () {
    onmessage = function (msg) {
        postMessage("late:" + msg.data);
    };
    postMessage("ready");
}, 200);
