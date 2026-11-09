// JNI bridge micro-benchmarks. Not part of the Jasmine suite: boot.js runs this instead of the
// suite when `adb shell setprop debug.ns.jnibench 1` is set, and every result is logged as a
// `JNIBENCH {json}` line (read with `adb logcat -s JS:I | grep JNIBENCH`).
//
// Each benchmark warms up (so V8 tiers the caller up), then runs ROUNDS timed rounds and reports
// the median and minimum ns/op. Java -> JS benchmarks are timed in Java with System.nanoTime.

var JniBench = com.tns.tests.JniBench;
var ROUNDS = 7;
var FILTER = String(JniBench.getFilter());
var SCALE = JniBench.getScale();

function median(values) {
    var sorted = values.slice().sort(function (a, b) { return a - b; });
    return sorted[Math.floor(sorted.length / 2)];
}

function report(name, nsPerOp) {
    var result = {
        name: name,
        median: Math.round(median(nsPerOp) * 10) / 10,
        min: Math.round(Math.min.apply(null, nsPerOp) * 10) / 10
    };
    console.log("JNIBENCH " + JSON.stringify(result));
    return result;
}

// body(n) runs the operation n times; returning a value keeps V8 from dropping the loop.
function measureJs(name, iterations, body) {
    if (FILTER && name.indexOf(FILTER) < 0) return null;
    iterations *= SCALE;
    var sink = body(Math.max(1000, iterations >> 2));
    var samples = [];
    for (var r = 0; r < ROUNDS; r++) {
        var start = performance.now();
        sink = body(iterations);
        var elapsed = performance.now() - start;
        samples.push((elapsed * 1e6) / iterations);
    }
    globalThis.__jniBenchSink = sink;
    return report(name, samples);
}

function measureCallback(name, cb, kind, iterations) {
    if (FILTER && name.indexOf(FILTER) < 0) return null;
    iterations *= SCALE;
    JniBench.runCallbacks(cb, kind, Math.max(1000, iterations >> 2));
    var samples = [];
    for (var r = 0; r < ROUNDS; r++) {
        var nanos = JniBench.runCallbacks(cb, kind, iterations);
        samples.push(Number(nanos) / iterations);
    }
    return report(name, samples);
}

function buildBenchmarks() {
    var bench = new JniBench();
    var other = bench.getOther();
    var jsString = "hello world";
    var longString = new Array(129).join("abcdefgh");
    var Extended = java.lang.Object.extend({
        toString: function () { return "extended"; }
    });
    var buffer = new ArrayBuffer(64);
    var intArray = [1, 2, 3, 4, 5, 6, 7, 8];
    var callback = new JniBench.Callback({
        noArgs: function () { },
        twoInts: function (a, b) { },
        oneString: function (s) { },
        intReturn: function (a) { return a + 1; },
        oneObject: function (o) { }
    });

    return [
        function () {
            return measureJs("js.emptyFunctionCall", 200000, function (n) {
                var f = function (x) { return x + 1; };
                var s = 0;
                for (var i = 0; i < n; i++) s = f(s);
                return s;
            });
        },
        function () {
            return measureJs("static.noop()", 50000, function (n) {
                for (var i = 0; i < n; i++) JniBench.noop();
                return n;
            });
        },
        function () {
            return measureJs("static.add(int,int)", 50000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = JniBench.add(i, 1);
                return s;
            });
        },
        function () {
            return measureJs("static.addDouble(double,double)", 50000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = JniBench.addDouble(i, 0.5);
                return s;
            });
        },
        function () {
            return measureJs("static.addLong(long,long)", 50000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = JniBench.addLong(i, 1);
                return s;
            });
        },
        function () {
            return measureJs("instance.noop()", 50000, function (n) {
                for (var i = 0; i < n; i++) bench.instanceNoop();
                return n;
            });
        },
        function () {
            return measureJs("instance.getInt()", 50000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = bench.getInt();
                return s;
            });
        },
        function () {
            return measureJs("instance.getBool()", 50000, function (n) {
                var s = false;
                for (var i = 0; i < n; i++) s = bench.getBool();
                return s;
            });
        },
        function () {
            return measureJs("instance.getChar()", 50000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.getChar();
                return s;
            });
        },
        function () {
            return measureJs("instance.echo(String)", 50000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.echo(jsString);
                return s;
            });
        },
        function () {
            return measureJs("instance.echo(String 1KB)", 20000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.echo(longString);
                return s;
            });
        },
        function () {
            return measureJs("instance.getString()", 50000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.getString();
                return s;
            });
        },
        function () {
            return measureJs("instance.self() [wrapped return]", 50000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.self();
                return s;
            });
        },
        function () {
            return measureJs("instance.getOther() [wrapped return]", 50000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.getOther();
                return s;
            });
        },
        function () {
            return measureJs("instance.takeObject(Object)", 50000, function (n) {
                for (var i = 0; i < n; i++) bench.takeObject(other);
                return n;
            });
        },
        function () {
            return measureJs("instance.takeObjects(Object x3)", 50000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = bench.takeObjects(other, bench, other);
                return s;
            });
        },
        function () {
            return measureJs("static.bufferCapacity(ArrayBuffer)", 20000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = JniBench.bufferCapacity(buffer);
                return s;
            });
        },
        function () {
            return measureJs("static.sumInts(int[8] from JS array)", 20000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = JniBench.sumInts(intArray);
                return s;
            });
        },
        function () {
            return measureJs("static.newObjectInJava() [new wrapper]", 20000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = JniBench.newObjectInJava();
                return s;
            });
        },
        function () {
            return measureJs("new java.lang.Object()", 20000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = new java.lang.Object();
                return s;
            });
        },
        function () {
            return measureJs("new Object.extend({})()", 20000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = new Extended();
                return s;
            });
        },
        function () {
            return measureJs("field.get int", 50000, function (n) {
                var s = 0;
                for (var i = 0; i < n; i++) s = bench.intField;
                return s;
            });
        },
        function () {
            return measureJs("field.set int", 50000, function (n) {
                for (var i = 0; i < n; i++) bench.intField = i;
                return n;
            });
        },
        function () {
            return measureJs("field.get Object", 50000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.objField;
                return s;
            });
        },
        function () {
            return measureJs("field.get String", 50000, function (n) {
                var s;
                for (var i = 0; i < n; i++) s = bench.strField;
                return s;
            });
        },
        function () {
            return measureCallback("callback.noArgs()", callback, 0, 20000);
        },
        function () {
            return measureCallback("callback.twoInts(int,int)", callback, 1, 20000);
        },
        function () {
            return measureCallback("callback.oneString(String)", callback, 2, 20000);
        },
        function () {
            return measureCallback("callback.intReturn(int) -> int", callback, 3, 20000);
        },
        function () {
            return measureCallback("callback.oneObject(Object)", callback, 4, 20000);
        }
    ];
}

exports.run = function () {
    var benchmarks = buildBenchmarks();
    var results = [];
    var index = 0;
    // Yield to the looper between benchmarks so GC and queued work run outside timed rounds.
    function next() {
        if (index >= benchmarks.length) {
            console.log("JNIBENCH_DONE " + JSON.stringify(results));
            return;
        }
        gc();
        var result = benchmarks[index++]();
        if (result) results.push(result);
        setTimeout(next, 50);
    }
    setTimeout(next, 500);
};
