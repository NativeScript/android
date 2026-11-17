// Entry for testInterfaceExtraInterfaces: an interface extend with an extra interface, made and
// constructed in a worker's runtime.
var X = com.tns.tests.interfaces.extras;
var F = X.ExtrasFixtures;
var result = {};

try {
    var B = X.WorkerBase;
    var C = B.extend({
        interfaces: [X.Extra],
        base: function () { return "worker base"; },
        extra: function () { return "worker extra"; }
    });
    var instance = new C();
    result.className = String(F.className(instance));
    result.isExtra = F.isInstance(instance, "com.tns.tests.interfaces.extras.Extra");
    result.extra = String(F.call(instance, "com.tns.tests.interfaces.extras.Extra", "extra"));
} catch (e) {
    result.error = String(e && e.message || e);
}
postMessage(result);
