describe("Proxy registration GC guard", function () {
    var Guard = com.tns.ProxyRegistrationGc;

    function maps(names) {
        var text = names.map(function (name, i) {
            return "7f0000" + i + "000-7f0000" + i + "fff rw-p 00000000 00:00 0 " + name;
        }).join("\n");
        return new java.io.BufferedReader(new java.io.StringReader(text));
    }

    var markCompact = [
        "[anon:dalvik-main space]",
        "[anon:dalvik-Concurrent mark-compact compaction buffers]",
        "[anon:dalvik-Concurrent Mark Compact live words bitmap]",
        "/apex/com.android.art/lib64/libart.so"
    ];
    var concurrentCopying = [
        "[anon:dalvik-main space (region space)]",
        "[anon:dalvik-concurrent copying gc mark stack]",
        "[anon:dalvik-region space live bitmap]",
        "/apex/com.android.art/lib64/libart.so"
    ];

    it("collects under the mark-compact collector", function () {
        expect(Guard.isRequired(36, maps(markCompact))).toBe(true);
    });

    it("skips the collection under the concurrent-copying collector", function () {
        expect(Guard.isRequired(33, maps(concurrentCopying))).toBe(false);
    });

    it("never collects before ART became updatable", function () {
        expect(Guard.isRequired(30, maps(markCompact))).toBe(false);
        expect(Guard.isRequired(21, maps([]))).toBe(false);
    });

    it("collects when the heap mappings are unnamed", function () {
        expect(Guard.isRequired(33, maps(["/apex/com.android.art/lib64/libart.so", "[stack]"]))).toBe(true);
    });

    it("collects when no known collector is recognized", function () {
        expect(Guard.isRequired(36, maps(["[anon:dalvik-main space]", "[anon:dalvik-card table]"]))).toBe(true);
    });

    it("collects when mark-compact and region space names both appear", function () {
        expect(Guard.isRequired(36, maps(concurrentCopying.concat(markCompact)))).toBe(true);
    });

    it("matches the collector this process is running", function () {
        var reader = new java.io.BufferedReader(new java.io.FileReader("/proc/self/maps"));
        var sawMarkCompact = false;
        var sawConcurrentCopying = false;
        try {
            var line;
            while ((line = reader.readLine()) !== null) {
                var lower = String(line).toLowerCase();
                if (lower.indexOf("[anon:dalvik-") < 0) {
                    continue;
                }
                if (lower.indexOf("mark-compact") >= 0 || lower.indexOf("mark compact") >= 0) {
                    sawMarkCompact = true;
                }
                if (lower.indexOf("concurrent copying") >= 0 || lower.indexOf("(region space)") >= 0) {
                    sawConcurrentCopying = true;
                }
            }
        } finally {
            reader.close();
        }

        var sdk = android.os.Build.VERSION.SDK_INT;
        if (sdk < 31) {
            expect(Guard.isRequired()).toBe(false);
        } else if (sawMarkCompact) {
            expect(Guard.isRequired()).toBe(true);
        } else if (sawConcurrentCopying) {
            expect(Guard.isRequired()).toBe(false);
        } else {
            expect(Guard.isRequired()).toBe(true);
        }
    });
});
