// Entry for testInterfaceDefaultMethodOverrides: implements interfaces through aliases, so
// this worker's runtime uses classes generated on the device.
var F = com.tns.tests.DefaultMethodFixtures;
var It = java.util.Iterator;
var Split = java.util.Spliterator;
var result = {};

try {
    var removes = 0;
    var withRemove = new It({
        hasNext: function () { return false; },
        next: function () { return null; },
        remove: function () { removes++; }
    });
    result.newClassName = String(F.className(withRemove));
    result.remove = String(F.remove(withRemove));
    result.removes = removes;

    var withoutRemove = new It({
        hasNext: function () { return false; },
        next: function () { return null; },
        forEachRemaining: function () {}
    });
    result.missingDefault = String(F.remove(withoutRemove));

    var Sized = Split.extend({
        tryAdvance: function () { return false; },
        trySplit: function () { return null; },
        estimateSize: function () { return 7; },
        characteristics: function () { return Split.SIZED; },
        getExactSizeIfKnown: function () { return 7; }
    });
    var sized = new Sized();
    result.extendClassName = String(F.className(sized));
    result.spliterator = String(F.spliteratorDefaults(sized));
} catch (e) {
    result.error = String(e && e.message || e);
}
postMessage(result);
