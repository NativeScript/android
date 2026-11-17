// Generates a set of named proxies unique to `tag` and has Java call every overridden
// method through each generated class. Returns a description of every check that failed.
//
// extend() is called through a computed member so the static binding generator does not
// precompile these classes: they must be generated at runtime by the calling runtime.
module.exports = function generateAndCallProxies(tag) {
    var failures = [];
    var prefix = "com.tns.tests.proxygen." + tag + ".";

    function check(what, actual, expected) {
        if (actual !== expected) {
            failures.push(tag + " " + what + ": expected " + expected + ", got " + actual);
        }
    }

    function attempt(what, body) {
        try {
            body();
        } catch (e) {
            failures.push(tag + " " + what + " threw: " + e);
        }
    }

    attempt("Comparator", function () {
        var ByLength = java.util.Comparator["extend"](prefix + "ByLength", {
            compare: function (a, b) {
                return a.length - b.length;
            }
        });
        var list = java.util.Arrays.asList(["aa", "a", "aaaa", "aaa"]);
        check("Collections.max(compare)", java.util.Collections.max(list, new ByLength()), "aaaa");
    });

    attempt("Callable", function () {
        var Answer = java.util.concurrent.Callable["extend"](prefix + "Answer", {
            call: function () {
                return "called " + tag;
            }
        });
        var task = new java.util.concurrent.FutureTask(new Answer());
        task.run();
        check("FutureTask.get(call)", task.get(), "called " + tag);
    });

    attempt("Runnable", function () {
        var ran = false;
        var Task = java.lang.Runnable["extend"](prefix + "Task", {
            run: function () {
                ran = true;
            }
        });
        new java.lang.Thread(new Task()).run();
        check("Thread.run(run)", ran, true);
    });

    attempt("InputStream", function () {
        var Stream = java.io.InputStream["extend"](prefix + "Stream", {
            read: function () {
                return 42;
            },
            skip: function (n) {
                return n * 2;
            },
            available: function () {
                return 5;
            }
        });
        var stream = new java.io.PushbackInputStream(new Stream());
        check("read()", stream.read(), 42);
        check("skip(long)", stream.skip(21), 42);
        check("available()", stream.available(), 5);
    });

    attempt("OutputStream", function () {
        var written = [];
        var Sink = java.io.OutputStream["extend"](prefix + "Sink", {
            write: function (b) {
                written.push(b);
            }
        });
        new java.io.DataOutputStream(new Sink()).writeShort(0x0102);
        check("DataOutputStream.writeShort(write)", written.join(","), "1,2");
    });

    attempt("Object", function () {
        var Value = java.lang.Object["extend"](prefix + "Value", {
            toString: function () {
                return "value " + tag;
            },
            hashCode: function () {
                return 1234;
            },
            equals: function (other) {
                return other === "same";
            }
        });
        var value = new Value();
        check("Objects.toString(toString)", java.util.Objects.toString(value), "value " + tag);
        check("Objects.hashCode(hashCode)", java.util.Objects.hashCode(value), 1234);
        check("Objects.equals(equals)", java.util.Objects.equals(value, "same"), true);
    });

    return failures;
};
