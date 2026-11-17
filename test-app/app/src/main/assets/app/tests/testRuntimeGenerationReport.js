describe("Runtime class generation report", function () {

    function reportEntriesFor(loadedName) {
        var lines = com.tns.DexFactory.getRuntimeGeneratedClasses();
        var result = [];
        for (var i = 0; i < lines.length; i++) {
            var fields = String(lines[i]).split(" ");
            if (fields[2] === loadedName) {
                result.push({ source: fields[0], kind: fields[1], loaded: fields[2], requested: fields[3] });
            }
        }
        return result;
    }

    it("lists a class generated at runtime", function () {
        // A computed member hides the extend from the static binding generator, and
        // a member no other extend has keeps a content-keyed name from matching a
        // prebuilt class.
        var ext = "ex" + "tend";
        var ReportedObject = java.lang.Object[ext]({
            toString: function () {
                return "reported";
            },
            reportedObjectMarker: true
        });
        var instance = new ReportedObject();
        expect(instance.toString()).toBe("reported");

        var entries = reportEntriesFor(instance.getClass().getName());
        expect(entries.length).toBe(1);
        expect(["generated", "cached"]).toContain(entries[0].source);
        expect(entries[0].kind).toBe("class");
        expect(entries[0].requested).toMatch(/^com\.tns\.gen\.java\.lang\.Object_/);
    });

    it("lists the class of an interface whose JS implementation overrides a default method", function () {
        if (android.os.Build.VERSION.SDK_INT < 24) {
            return;
        }
        // Reached through an alias, so no class of the interface is prebuilt, and implemented
        // by no other spec.
        var LongConsumer = java.util.function["Long" + "Consumer"];
        var thens = 0;
        var instance = new LongConsumer({
            accept: function () {},
            andThen: function (after) {
                thens++;
                return after;
            }
        });
        // Called from Java: the class dispatches the default method to JS.
        LongConsumer.class.getMethod("andThen", [LongConsumer.class]).invoke(instance, [instance]);
        expect(thens).toBe(1);

        var loaded = String(instance.getClass().getName());
        var entries = reportEntriesFor(loaded).filter(function (entry) {
            return entry.requested === "com.tns.gen.java.util.function.LongConsumer-defaults";
        });
        expect(entries.length).toBe(1);
        expect(entries[0].kind).toBe("interface");
        if (java.lang.reflect.Proxy.isProxyClass(instance.getClass())) {
            expect(entries[0].source).toBe("proxy");
        } else {
            expect(["generated", "cached"]).toContain(entries[0].source);
            expect(loaded).toMatch(/^com\.tns\.gen\.java\.util\.function\.LongConsumer-defaults(-jni)?$/);
        }
    });

    it("does not list a class loaded from the APK", function () {
        var UnreportedObject = java.lang.Object.extend({
            toString: function () {
                return "unreported";
            }
        });
        var instance = new UnreportedObject();
        expect(instance.toString()).toBe("unreported");

        expect(reportEntriesFor(instance.getClass().getName())).toEqual([]);
    });
});
