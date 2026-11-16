// Entry for testInterfaceExtendClasses: extends an interface the static binding
// generator never sees, so this worker's runtime generates its class.
var ext = "ex" + "tend";
var Action = java.security.PrivilegedExceptionAction;
var result = {};

// Reflection makes Java, not JS, call the implementation.
function callFromJava(instance, methodName) {
    return instance.getClass().getMethod(methodName, []).invoke(instance, []);
}
try {
    var Extended = Action[ext]({
        run: function () {
            return "worker extend";
        }
    });
    var extended = new Extended();
    result.extendedClass = String(extended.getClass().getName());
    result.extendedRun = String(callFromJava(extended, "run"));

    var implemented = new Action({
        run: function () {
            return "worker new";
        }
    });
    result.implementedClass = String(implemented.getClass().getName());
    result.implementedRun = String(callFromJava(implemented, "run"));
} catch (e) {
    result.error = String(e && e.message || e);
}
postMessage(result);
