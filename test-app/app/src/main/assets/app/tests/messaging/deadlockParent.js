// Leaves a message that carries a port on this worker's own loop, undrained,
// at the moment the parent terminates it: the port's sibling is port1, owned
// by this worker. Spinning inside a timer callback keeps the loop from
// draining while still letting terminate() interrupt the JS.
var channel = new MessageChannel();
var child = new Worker("./deadlockChild.js");
var flag = new Int32Array(new SharedArrayBuffer(4));
child.postMessage({ port: channel.port2, flag: flag }, [channel.port2]);
setTimeout(function () {
    while (Atomics.load(flag, 0) === 0) {}
    postMessage("ready");
    for (;;) {}
}, 0);
