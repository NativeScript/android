describe("Primitive return values of JS callbacks", function () {
    var current;
    var impl = new com.tns.tests.PrimitiveReturnTest.Returns({
        z: function () { return current(); },
        b: function () { return current(); },
        c: function () { return current(); },
        s: function () { return current(); },
        i: function () { return current(); },
        j: function () { return current(); },
        f: function () { return current(); },
        d: function () { return current(); }
    });

    // Numbers at or beyond +-2^63 are left out: the boxed conversion casts them
    // to int64, which is undefined and differs between arm64 and x86_64.
    var values = {
        "1": function () { return 1; },
        "-1": function () { return -1; },
        "0": function () { return 0; },
        "-0": function () { return -0; },
        "2^31": function () { return 2147483648; },
        "-2^31-1": function () { return -2147483649; },
        "2^40": function () { return 1099511627776; },
        "2^60": function () { return 1152921504606846976; },
        "1.5": function () { return 1.5; },
        "-1.5": function () { return -1.5; },
        "1e-40": function () { return 1e-40; },
        "3.5e38": function () { return 3.5e38; },
        "NaN": function () { return NaN; },
        "Infinity": function () { return Infinity; },
        "-Infinity": function () { return -Infinity; },
        "-2.5": function () { return -2.5; },
        "true": function () { return true; },
        "false": function () { return false; },
        "'1'": function () { return "1"; },
        "'a'": function () { return "a"; },
        "null": function () { return null; },
        "undefined": function () { return undefined; },
        "Number(5)": function () { return new Number(5); },
        "Number(2.5)": function () { return new Number(2.5); },
        "Boolean(true)": function () { return new Boolean(true); },
        "Boolean(false)": function () { return new Boolean(false); },
        "long(5)": function () { return long(5); },
        "byte(2)": function () { return byte(2); },
        "short(3)": function () { return short(3); },
        "char('a')": function () { return char("a"); },
        "float(1.5)": function () { return float(1.5); },
        "double(2)": function () { return double(2); },
        "{}": function () { return {}; }
    };

    // Captured from the boxed conversion; columns are z b c s i j f d.
    var expected = {
        "0": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:0 ok:0 ex:ClassCastException ex:ClassCastException",
        "1": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:1 ok:1 ex:ClassCastException ex:ClassCastException",
        "-1": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:-1 ok:-1 ex:ClassCastException ex:ClassCastException",
        "-0": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:0 ok:0 ex:ClassCastException ex:ClassCastException",
        "2^31": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:2147483648 ex:ClassCastException ex:ClassCastException",
        "-2^31-1": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:-2147483649 ex:ClassCastException ex:ClassCastException",
        "2^40": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:1099511627776 ex:ClassCastException ex:ClassCastException",
        "2^60": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:1152921504606846976 ex:ClassCastException ex:ClassCastException",
        "1.5": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:1.5 ok:1.5",
        "-1.5": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:-1.5",
        "1e-40": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:1.0E-40",
        "3.5e38": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:3.5E38",
        "NaN": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:NaN",
        "Infinity": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:Infinity",
        "-Infinity": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:-Infinity",
        "-2.5": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:-2.5",
        "true": "ok:true ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "false": "ok:false ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "'1'": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "'a'": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "null": "ok:false ok:0 ok:0 ok:0 ok:0 ok:0 ok:0.0 ok:0.0",
        "undefined": "ok:false ok:0 ok:0 ok:0 ok:0 ok:0 ok:0.0 ok:0.0",
        "Number(5)": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:5 ok:5 ex:ClassCastException ex:ClassCastException",
        "Number(2.5)": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:2.5 ok:2.5",
        "Boolean(true)": "ok:true ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "Boolean(false)": "ok:true ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "long(5)": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:5 ex:ClassCastException ex:ClassCastException",
        "byte(2)": "ex:ClassCastException ok:2 ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "short(3)": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:3 ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "char('a')": "ex:ClassCastException ex:ClassCastException ok:97 ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException",
        "float(1.5)": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:1.5 ex:ClassCastException",
        "double(2)": "ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ex:ClassCastException ok:2.0",
        "{}": "ok:false ok:0 ok:0 ok:0 ok:0 ok:0 ok:0.0 ok:0.0"
    };

    var kinds = ["z", "b", "c", "s", "i", "j", "f", "d"];

    function matrix() {
        var result = {};
        Object.keys(values).forEach(function (name) {
            current = values[name];
            result[name] = kinds.map(function (kind) {
                return com.tns.tests.PrimitiveReturnTest.call(impl, kind);
            }).join(" ");
        });
        return result;
    }

    it("converts every JS value the same way for each primitive return type", function () {
        var actual = matrix();
        Object.keys(values).forEach(function (name) {
            expect(name + " | " + actual[name]).toBe(name + " | " + expected[name]);
        });
    });
});
