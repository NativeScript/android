// A builtin module is a singleton per realm, so the worker builds its own
// ns:util rather than reaching the main context's instance.
onmessage = function () {
    var util = require("ns:util");
    postMessage({
        formatted: util.format("%d apples", 3),
        singleton: require("ns:util") === util,
        frozen: Object.isFrozen(util),
    });
};
