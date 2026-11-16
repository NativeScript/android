// Entry for testAppModuleBindings: every call is hidden from the static binding
// generator, so this worker's runtime generates the classes.
var ext = "ex" + "tend";
var Target = com.tns.tests.AppModuleBindingTarget.WorkerRuntimeGenerated;
var result = {};

function callFromJava(instance, methodName) {
    return String(instance.getClass().getMethod(methodName, []).invoke(instance, []));
}

try {
    var Extended = com.tns.tests.Button1[ext]({
        toString: function () {
            return "worker extend";
        },
        workerExtendMarker: true
    });
    var extended = new Extended();
    result.extendedClass = String(extended.getClass().getName());
    result.extendedCall = callFromJava(extended, "toString");

    var implemented = new Target({
        describe: function () {
            return "worker new";
        }
    });
    result.implementedClass = String(implemented.getClass().getName());
    result.implementedCall = callFromJava(implemented, "describe");
} catch (e) {
    result.error = String(e && e.message || e);
}
postMessage(result);
