// Entry for testInterfaceProxies: implements an interface the static binding generator
// never sees, and leaves the instance for the main runtime in RuntimeBoundHolder.
var F = com.tns.tests.InterfaceProxyFixtures;
var Target = F["Worker" + "Target"];
var result = {};

try {
    var target = new Target({
        describe: function () {
            return "worker:" + java.lang.Thread.currentThread().getName();
        }
    });
    result.isProxy = java.lang.reflect.Proxy.isProxyClass(target.getClass());
    result.thread = String(java.lang.Thread.currentThread().getName());
    result.fromJava = String(F.describeWorkerTarget(target));
    com.tns.tests.RuntimeBoundHolder.put("interfaceProxyWorkerTarget", target);
} catch (e) {
    result.error = String(e && e.message || e);
}
postMessage(result);
