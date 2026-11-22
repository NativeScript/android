// Entry for testWrapperLifetimeFieldWrites: the same fields written from another runtime.
var Fixtures = com.tns.tests.CanaryFieldFixtures;
var result = { values: [] };
try {
    var leaf = new Fixtures.Leaf();
    leaf.f = 1;
    leaf.f = 2;
    result.values.push(leaf.getF());
    var Ext = Fixtures.Base.extend({});
    var ext = new Ext();
    ext.f = 2;
    ext.f = 3;
    result.values.push(ext.getF());
    leaf.f = 4;
    result.values.push(leaf.getF());
    ext.f = 5;
    result.values.push(ext.getF());
} catch (e) {
    result.error = String(e && e.message || e);
}
postMessage(result);
