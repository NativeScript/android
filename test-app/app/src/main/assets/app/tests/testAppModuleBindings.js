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

    // These work whether or not the build showed the static binding generator the app's
    // own classes (-PskipSbgAppClasses); with it, they are prebuilt only where noted.
    describe("App-module types the static binding generator cannot prebuild as they are", function () {
        var Shapes = com.tns.tests.AppModuleShapes;
        var sdk = android.os.Build.VERSION.SDK_INT;

        if (false) {
            new com.tns.tests.AppModuleShapes.StepProbe({
                describe: function () {
                    return "never runs";
                }
            });
        }
        var appClassesShown = (function () {
            try {
                java.lang.Class.forName("com.tns.gen.com.tns.tests.AppModuleShapes_StepProbe", false, Shapes.class.getClassLoader());
                return true;
            } catch (e) {
                return false;
            }
        })();

        function expectGeneratedAtRuntime(instance, kind) {
            var entries = reportEntriesFor(instance.getClass().getName());
            expect(entries.length).toBe(1);
            expect(entries.length > 0 && entries[0].kind).toBe(kind);
        }

        function expectPrebuiltWhenAppClassesShown(instance) {
            if (appClassesShown) {
                expect(reportEntriesFor(instance.getClass().getName())).toEqual([]);
            } else {
                expectGeneratedAtRuntime(instance, "class");
            }
        }

        it("prebuild an extend of a Kotlin class with a default constructor argument", function () {
            var Extended = com.tns.tests.AppModuleKotlinDefaultArgument.extend({
                describe: function () {
                    return "js default argument";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("js default argument");
            expect(String(instance.getLabel())).toBe("kotlin default");
            expectPrebuiltWhenAppClassesShown(instance);
        });

        it("prebuild an extend of a Kotlin class with JvmOverloads constructors", function () {
            var Extended = com.tns.tests.AppModuleKotlinOverloads.extend({
                describe: function () {
                    return "js overloads";
                }
            });
            var instance = new Extended("from js");

            expect(callFromJava(instance, "describe")).toBe("js overloads");
            expect(String(instance.getLabel())).toBe("from js");
            expect(instance.getCount()).toBe(1);
            expectPrebuiltWhenAppClassesShown(instance);
        });

        it("generate an extend of a class with a self-referencing type parameter", function () {
            var Extended = com.tns.tests.AppModuleShapes.SelfTyped.extend({
                self: function () {
                    return this;
                },
                describe: function () {
                    return "js self typed";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("js self typed");
            expect(instance.getClass().getMethod("self", []).invoke(instance, [])).toBe(instance);
            expectGeneratedAtRuntime(instance, "class");
        });

        it("generate an extend of a Kotlin class with a self-referencing type parameter", function () {
            var Extended = com.tns.tests.AppModuleKotlinSelfTyped.extend({
                self: function () {
                    return this;
                },
                describe: function () {
                    return "js kotlin self typed";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("js kotlin self typed");
            expectGeneratedAtRuntime(instance, "class");
        });

        it("generate new of an interface with a type parameter bounded by itself", function () {
            var implemented = new com.tns.tests.AppModuleShapes.ComparableSource({
                next: function () {
                    return null;
                },
                describe: function () {
                    return "js comparable source";
                }
            });

            expect(callFromJava(implemented, "describe")).toBe("js comparable source");
            expectRuntimeInterfaceClass(java.lang.reflect.Proxy.isProxyClass(implemented.getClass()), String(implemented.getClass().getName()), "com.tns.gen.com.tns.tests.AppModuleShapes_ComparableSource");
            expectGeneratedAtRuntime(implemented, "interface");
        });

        it("generate an extend of a class with a package-private abstract method", function () {
            var Extended = com.tns.tests.AppModuleShapes.PackagePrivateAbstract.extend({
                describe: function () {
                    return "js package-private abstract";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("js package-private abstract");
            expectGeneratedAtRuntime(instance, "class");
        });

        it("generate an extend of a class with an abstract method returning a package-private type", function () {
            var Extended = com.tns.tests.AppModuleShapes.HiddenReturn.extend({
                describe: function () {
                    return "js hidden return";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("js hidden return");
            expectGeneratedAtRuntime(instance, "class");
        });

        it("generate an extend of a class with a public constructor taking a package-private type", function () {
            var Extended = com.tns.tests.AppModuleShapes.HiddenConstructorArgument.extend({
                describe: function () {
                    return "js hidden constructor argument";
                }
            });
            var instance = new Extended();

            expect(callFromJava(instance, "describe")).toBe("js hidden constructor argument");
            expectGeneratedAtRuntime(instance, "class");
        });

        it("generate an extend of a class with an abstract method taking a package-private type", function () {
            var calls = 0;
            var Extended = com.tns.tests.AppModuleShapes.HiddenParameter.extend({
                take: function () {
                    calls++;
                    return "js take";
                }
            });
            var instance = new Extended();

            expect(String(com.tns.tests.AppModuleShapes.HiddenParameter.callTake(instance))).toBe("js take");
            expect(calls).toBe(1);
            expectGeneratedAtRuntime(instance, "class");
        });

        it("generate an extend of a non-static inner class", function () {
            var Extended = com.tns.tests.AppModuleShapes.Inner.extend({
                describe: function () {
                    return "js inner";
                }
            });
            var instance = new Extended(new Shapes());

            expect(callFromJava(instance, "describe")).toBe("js inner");
            expectGeneratedAtRuntime(instance, "class");
        });

        function expectTwoDefaultsCallSites(first, second, caller, javaPrefix, interfaceName) {
            var interfaceClass = java.lang.Class.forName(interfaceName, false, Shapes.class.getClassLoader());
            var desugared = sdk < 24 || !interfaceClass.getMethod("first", []).isDefault();

            expect(String(caller.first(first))).toBe("js first");
            expect(String(caller.second(second))).toBe("js second");
            if (desugared) {
                // Below minSdk 24 D8 turns the defaults into abstract methods: each reaches
                // JS, and one the JS object lacks has no Java body to fall back to.
                expect(String(caller.second(first))).toBe("com.tns.NativeScriptException");
                expect(String(caller.first(second))).toBe("com.tns.NativeScriptException");
            } else {
                expect(String(caller.second(first))).toBe(javaPrefix + " second");
                expect(String(caller.first(second))).toBe(javaPrefix + " first");
                if (!proxies) {
                    expect(String(first.getClass().getName())).toMatch(/-defaults/);
                    expect(String(second.getClass().getName())).toMatch(/-defaults/);
                }
            }
            expect(reportEntriesFor(first.getClass().getName()).length).toBeGreaterThan(0);
            expect(reportEntriesFor(second.getClass().getName()).length).toBeGreaterThan(0);
        }

        it("generate the classes of call sites that override different default methods of an interface", function () {
            var first = new com.tns.tests.AppModuleShapes.TwoDefaults({
                name: function () {
                    return "first";
                },
                first: function () {
                    return "js first";
                }
            });
            var second = new com.tns.tests.AppModuleShapes.TwoDefaults({
                name: function () {
                    return "second";
                },
                second: function () {
                    return "js second";
                }
            });

            expectTwoDefaultsCallSites(first, second, Shapes, "java", "com.tns.tests.AppModuleShapes$TwoDefaults");
        });

        it("generate the classes of call sites that override different default methods of a Kotlin interface", function () {
            var first = new com.tns.tests.AppModuleKotlinTwoDefaults({
                name: function () {
                    return "first";
                },
                first: function () {
                    return "js first";
                }
            });
            var second = new com.tns.tests.AppModuleKotlinTwoDefaults({
                name: function () {
                    return "second";
                },
                second: function () {
                    return "js second";
                }
            });

            expectTwoDefaultsCallSites(first, second, com.tns.tests.AppModuleKotlinShapesCaller, "kotlin", "com.tns.tests.AppModuleKotlinTwoDefaults");
        });
    });
});
