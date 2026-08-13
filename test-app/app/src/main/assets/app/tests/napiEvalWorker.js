onmessage = function (msg) {
    eval(msg.data.eval || "");
};
