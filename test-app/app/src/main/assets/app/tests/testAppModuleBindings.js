describe("Bindings of app-module types", function () {
    // A computed member hides an extend from the static binding generator.
    var ext = "ex" + "tend";
    // With this option an interface without a prebuilt class gets a java.lang.reflect.Proxy class.
    var proxies = !!require("../package.json").android.interfaceProxies;

    function expectRuntimeInterfaceClass(isProxyClass, className, expectedGeneratedName) {
        expect(isProxyClass).toBe(proxies);
        if (!proxies) {
            expect(className).toBe(expectedGeneratedName);
        }
    }

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

    describe("App-module Kotlin types", function () {
        function expectPrebuiltInterfaceClass(implemented, expectedName) {
            expect(java.lang.reflect.Proxy.isProxyClass(implemented.getClass())).toBe(false);
            expect(String(implemented.getClass().getName())).toBe(expectedName);
            expect(reportEntriesFor(implemented.getClass().getName())).toEqual([]);
        }

        it("prebuilds an extend of a Kotlin class", function () {
            var Extended = com.tns.tests.AppModuleKotlinBase.extend({
                describe: function () {
                    return "prebuilt kotlin extend";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("prebuilt kotlin extend");
            expect(instance instanceof com.tns.tests.AppModuleKotlinBase).toBe(true);
            expect(reportEntriesFor(instance.getClass().getName())).toEqual([]);
        });

        it("prebuilds new of a Kotlin interface", function () {
            var implemented = new com.tns.tests.AppModuleKotlinBindingTarget({
                describe: function () {
                    return "prebuilt kotlin new";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("prebuilt kotlin new");
            expectPrebuiltInterfaceClass(implemented, "com.tns.gen.com.tns.tests.AppModuleKotlinBindingTarget");
        });

        it("prebuilds new of a nested Kotlin interface", function () {
            var implemented = new com.tns.tests.AppModuleKotlinBindingTarget.Nested({
                describe: function () {
                    return "prebuilt kotlin nested";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("prebuilt kotlin nested");
            expectPrebuiltInterfaceClass(implemented, "com.tns.gen.com.tns.tests.AppModuleKotlinBindingTarget_Nested");
        });

        it("prebuilds new of a Kotlin interface built on a Java interface", function () {
            var implemented = new com.tns.tests.AppModuleKotlinOnJava({
                describe: function () {
                    return "prebuilt kotlin on java";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("prebuilt kotlin on java");
            expect(implemented instanceof com.tns.tests.AppModuleBindingTarget).toBe(true);
            expectPrebuiltInterfaceClass(implemented, "com.tns.gen.com.tns.tests.AppModuleKotlinOnJava");
        });

        it("prebuilds new of a Java interface built on a Kotlin interface", function () {
            var implemented = new com.tns.tests.AppModuleJavaOnKotlin({
                describe: function () {
                    return "prebuilt java on kotlin";
                },
                extra: function () {
                    return "extra";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("prebuilt java on kotlin");
            expect(callFromJava(implemented, "extra")).toBe("extra");
            expectPrebuiltInterfaceClass(implemented, "com.tns.gen.com.tns.tests.AppModuleJavaOnKotlin");
        });

        it("prebuilds an extend of a Kotlin class with an open member Java cannot name", function () {
            var Extended = com.tns.tests.AppModuleKotlinValueOpen.extend({
                describe: function () {
                    return "prebuilt kotlin value open";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("prebuilt kotlin value open");
            expect(reportEntriesFor(instance.getClass().getName())).toEqual([]);
            var declared = instance.getClass().getDeclaredMethods();
            for (var i = 0; i < declared.length; i++) {
                expect(String(declared[i].getName())).not.toContain("-");
            }
        });

        it("generates an extend of a Kotlin class with an internal abstract member", function () {
            var Extended = com.tns.tests.AppModuleKotlinAbstract.extend({
                describe: function () {
                    return "runtime kotlin abstract";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("runtime kotlin abstract");
            var entries = reportEntriesFor(instance.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries[0].kind).toBe("class");
        });

        it("generates an extend of a Kotlin class with an abstract member Java cannot name", function () {
            var Extended = com.tns.tests.AppModuleKotlinValueAbstract.extend({
                describe: function () {
                    return "runtime kotlin value abstract";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("runtime kotlin value abstract");
            var entries = reportEntriesFor(instance.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries[0].kind).toBe("class");
        });

        it("generates new of a Kotlin interface with a member Java cannot name", function () {
            var implemented = new com.tns.tests.AppModuleKotlinValueTarget({
                describe: function () {
                    return "runtime kotlin value new";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("runtime kotlin value new");
            expectRuntimeInterfaceClass(java.lang.reflect.Proxy.isProxyClass(implemented.getClass()), String(implemented.getClass().getName()), "com.tns.gen.com.tns.tests.AppModuleKotlinValueTarget");
            var entries = reportEntriesFor(implemented.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries[0].kind).toBe("interface");
        });

        it("generates an extend of a Kotlin class that only compiles with the rest of the build", function () {
            var Extended = com.tns.tests.AppModuleKotlinNeedsBuild.extend({
                describe: function () {
                    return "runtime kotlin extend";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("runtime kotlin extend");
            var entries = reportEntriesFor(instance.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries[0].kind).toBe("class");
        });
    });

    describe("App-module types generated at runtime", function () {
        it("generates a hidden extend of an app-module class", function () {
            // A member no other extend has keeps a content-keyed name from matching
            // the class prebuilt for the extend above.
            var Extended = com.tns.tests.Button1[ext]({
                toString: function () {
                    return "runtime extend";
                },
                runtimeExtendMarker: true
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
            expectRuntimeInterfaceClass(java.lang.reflect.Proxy.isProxyClass(implemented.getClass()), String(implemented.getClass().getName()), "com.tns.gen.com.tns.tests.AppModuleBindingTarget_RuntimeGenerated");
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
            expectRuntimeInterfaceClass(java.lang.reflect.Proxy.isProxyClass(instance.getClass()), String(instance.getClass().getName()), "com.tns.gen.com.tns.tests.AppModuleBindingTarget_ExtendedAtRuntime");
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
                expectRuntimeInterfaceClass(msg.data.implementedIsProxy, msg.data.implementedClass, "com.tns.gen.com.tns.tests.AppModuleBindingTarget_WorkerRuntimeGenerated");
                expect(reportEntriesFor(msg.data.extendedClass).length).toBe(1);
                expect(reportEntriesFor(msg.data.implementedClass).length).toBe(1);
                done();
            };
        });
    });
});
