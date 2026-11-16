// Entry for testContentKeyedBindings: the same content as call sites of the main
// runtime, one prebuilt, one generated at runtime, and a named one.
var ext = "ex" + "tend";
var result = {};

function javaToString(instance) {
    return String(instance.getClass().getMethod("toString", []).invoke(instance, []));
}

try {
    var Prebuilt = java.lang.Object.extend({
        toString: function () {
            return "worker prebuilt";
        },
        workerContentMarker: 1
    });
    var prebuilt = new Prebuilt();
    result.prebuiltClass = String(prebuilt.getClass().getName());
    result.prebuiltString = javaToString(prebuilt);

    var Hidden = java.lang.Object[ext]({
        toString: function () {
            return "worker hidden";
        },
        workerHiddenMarker: 1
    });
    var hidden = new Hidden();
    result.hiddenClass = String(hidden.getClass().getName());
    result.hiddenString = javaToString(hidden);
    result.hiddenCreatedString = javaToString(hidden.getClass().newInstance());

    var Owned = java.lang.Object.extend("WorkerOwnedName", {
        toString: function () {
            return "worker owned";
        }
    });
    var owned = new Owned();
    result.ownedClass = String(owned.getClass().getName());
    result.ownedString = javaToString(owned);
    result.ownedCreatedString = javaToString(owned.getClass().newInstance());
} catch (e) {
    result.error = String(e && e.stack || e);
}
postMessage(result);
