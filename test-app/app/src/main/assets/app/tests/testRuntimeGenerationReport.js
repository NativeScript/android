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
        // A computed member hides the extend from the static binding generator.
        var ext = "ex" + "tend";
        var ReportedObject = java.lang.Object[ext]({
            toString: function () {
                return "reported";
            }
        });
        var instance = new ReportedObject();
        expect(instance.toString()).toBe("reported");

        var entries = reportEntriesFor(instance.getClass().getName());
        expect(entries.length).toBe(1);
        expect(["generated", "cached"]).toContain(entries[0].source);
        expect(entries[0].kind).toBe("class");
        expect(entries[0].requested).toMatch(/^com\.tns\.gen\.java\.lang\.Object_/);
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
