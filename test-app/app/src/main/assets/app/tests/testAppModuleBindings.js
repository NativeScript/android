describe("Bindings of app-module types", function () {
    // A computed member hides an extend from the static binding generator.
    var ext = "ex" + "tend";

    // Reflection makes Java, not JS, call the implementation.
    function callFromJava(instance, methodName) {
        return String(instance.getClass().getMethod(methodName, []).invoke(instance, []));
    }

    function reportEntriesFor(loadedName) {
        var lines = com.tns.DexFactory.getRuntimeGeneratedClasses();
        var result = [];
        for (var i = 0; i < lines.length; i++) {
            var fields = String(lines[i]).split(" ");
            if (fields[2] === String(loadedName)) {
                result.push({ source: fields[0], kind: fields[1], loaded: fields[2], requested: fields[3] });
            }
        }
        return result;
    }

    describe("App-module types prebuilt by the static binding generator", function () {
        it("prebuilds an extend of an app-module class", function () {
            var Extended = com.tns.tests.Button1.extend({
                toString: function () {
                    return "prebuilt extend";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "toString")).toBe("prebuilt extend");
            expect(instance instanceof com.tns.tests.Button1).toBe(true);
            expect(reportEntriesFor(instance.getClass().getName())).toEqual([]);
        });

        it("prebuilds new of an app-module interface", function () {
            var implemented = new com.tns.tests.AppModuleBindingTarget({
                describe: function () {
                    return "prebuilt new";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("prebuilt new");
            expect(String(implemented.getClass().getName())).toBe("com.tns.gen.com.tns.tests.AppModuleBindingTarget");
            expect(reportEntriesFor(implemented.getClass().getName())).toEqual([]);
        });

        it("prebuilds new of a nested app-module interface", function () {
            var implemented = new com.tns.tests.AppModuleBindingTarget.Nested({
                describe: function () {
                    return "prebuilt nested";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("prebuilt nested");
            expect(String(implemented.getClass().getName())).toBe("com.tns.gen.com.tns.tests.AppModuleBindingTarget_Nested");
            expect(reportEntriesFor(implemented.getClass().getName())).toEqual([]);
        });
    });

    describe("App-module types generated at runtime", function () {
        it("generates a hidden extend of an app-module class", function () {
            var Extended = com.tns.tests.Button1[ext]({
                toString: function () {
                    return "runtime extend";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "toString")).toBe("runtime extend");
            expect(instance instanceof com.tns.tests.Button1).toBe(true);
            var entries = reportEntriesFor(instance.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries[0].kind).toBe("class");
            expect(entries[0].requested).toMatch(/^com\.tns\.gen\.com\.tns\.tests\.Button1_/);
        });

        it("generates a hidden new of an app-module interface", function () {
            var Target = com.tns.tests.AppModuleBindingTarget.RuntimeGenerated;
            var implemented = new Target({
                describe: function () {
                    return "runtime new";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("runtime new");
            expect(String(implemented.getClass().getName())).toBe("com.tns.gen.com.tns.tests.AppModuleBindingTarget_RuntimeGenerated");
            var entries = reportEntriesFor(implemented.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries[0].kind).toBe("interface");
        });

        it("generates a hidden extend of a nested app-module interface", function () {
            var Target = com.tns.tests.AppModuleBindingTarget.ExtendedAtRuntime;
            var Extended = Target[ext]({
                describe: function () {
                    return "runtime interface extend";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("runtime interface extend");
            expect(String(instance.getClass().getName())).toBe("com.tns.gen.com.tns.tests.AppModuleBindingTarget_ExtendedAtRuntime");
            var entries = reportEntriesFor(instance.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries[0].kind).toBe("interface");
        });

        it("generates app-module types in a worker", function (done) {
            var worker = new Worker("./appModuleBindingsWorker.js");
            worker.onmessage = function (msg) {
                worker.terminate();
                expect(msg.data.error).toBeUndefined();
                expect(msg.data.extendedCall).toBe("worker extend");
                expect(msg.data.implementedCall).toBe("worker new");
                expect(msg.data.implementedClass).toBe("com.tns.gen.com.tns.tests.AppModuleBindingTarget_WorkerRuntimeGenerated");
                expect(reportEntriesFor(msg.data.extendedClass).length).toBe(1);
                expect(reportEntriesFor(msg.data.implementedClass).length).toBe(1);
                done();
            };
        });
    });
});
