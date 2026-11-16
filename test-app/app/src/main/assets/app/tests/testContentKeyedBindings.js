// The test app is built and run both with and without -PuseContentBindings; these
// specs pass in both, and differ only where the class name format does.
describe("Content-keyed binding names", function () {

    var contentKeyed = (function () {
        try {
            java.lang.Class.forName("com.tns.gen.ContentKeyedBindings", false, com.tns.Runtime.class.getClassLoader());
            return true;
        } catch (e) {
            return false;
        }
    }());
    console.log("Binding class names are " + (contentKeyed ? "content-keyed" : "call-site-keyed") + " in this build");

    // A computed member hides an extend call from the static binding generator.
    var ext = "ex" + "tend";
    var tsExtends = global["__" + "extends"];

    function runtimeGenerated(instance) {
        var loadedName = String(instance.getClass().getName());
        var lines = com.tns.DexFactory.getRuntimeGeneratedClasses();
        for (var i = 0; i < lines.length; i++) {
            if (String(lines[i]).split(" ")[2] === loadedName) {
                return true;
            }
        }
        return false;
    }

    function className(instance) {
        return String(instance.getClass().getName());
    }

    // Runs the class's own toString override through Java.
    function javaToString(instance) {
        return String(instance.getClass().getMethod("toString", []).invoke(instance, []));
    }

    function createFromJava(instance) {
        return instance.getClass().newInstance();
    }

    // Defined when this file loads, in this order: the first of two call sites
    // sharing a class is the one Java gets when it creates that class by name.
    var SharedFirst = java.lang.Object.extend({
        toString: function () {
            return "shared first";
        },
        sharedContentMarker: 1
    });
    var SharedSecond = java.lang.Object.extend({
        toString: function () {
            return "shared second";
        },
        sharedContentMarker: 2
    });
    var SharedEdited = java.lang.Object.extend({
        toString: function () {
            return "shared edited";
        },
        sharedContentMarker: 3,
        editedMember: function () {
        }
    });

    var CollidingFirst = java.lang.Object.extend("CollidingName", {
        toString: function () {
            return "colliding first";
        }
    });
    var CollidingSecond = java.lang.Object.extend("CollidingName", {
        toString: function () {
            return "colliding second";
        }
    });

    var OwnedVisible = java.lang.Object.extend("OwnedName", {
        toString: function () {
            return "owned visible";
        },
        ownedNameMarker: 1
    });
    var OwnedHidden = java.lang.Object[ext]("OwnedName", {
        toString: function () {
            return "owned hidden";
        },
        ownedNameMarker: 1
    });

    it("names a class after its call site or after its content", function () {
        var Keyed = java.lang.Object.extend({
            toString: function () {
                return "keyed";
            },
            namingFormatMarker: 1
        });
        var instance = new Keyed();

        expect(javaToString(instance)).toBe("keyed");
        expect(runtimeGenerated(instance)).toBe(false);
        if (contentKeyed) {
            expect(className(instance)).toMatch(/^com\.tns\.gen\.java\.lang\.Object_h[0-9a-f]{16}$/);
        } else {
            expect(className(instance)).toMatch(/^com\.tns\.gen\.java\.lang\.Object_tests_testContentKeyedBindings_[0-9a-f]{6}_\d+_\d+_$/);
        }
    });

    describe("agrees with the static binding generator's ContentKeyTest", function () {

        // The class of each call site below is generated on the device, so its name
        // is computed by the runtime alone.
        function expectRuntimeName(instance, suffix) {
            expect(runtimeGenerated(instance)).toBe(true);
            if (contentKeyed) {
                expect(className(instance)).toBe(suffix);
            }
        }

        it("for a plain base", function () {
            var Plain = java.lang.Object[ext]({
                toString: function () {
                    return "pairing plain";
                },
                contentKeyPairingPlain: true
            });
            var instance = new Plain();
            expect(javaToString(instance)).toBe("pairing plain");
            expectRuntimeName(instance, "java.lang.Object_he8b99c521d575fca");
        });

        it("for a nested base", function () {
            var Nested = com.tns.tests.DummyClass.DummyDerivedClass[ext]({
                toString: function () {
                    return "pairing nested";
                },
                contentKeyPairingNested: true
            });
            var instance = new Nested();
            expect(javaToString(instance)).toBe("pairing nested");
            expectRuntimeName(instance, "com.tns.tests.DummyClass_DummyDerivedClass_h68c7e45e43205e66");
        });

        it("for implemented interfaces", function () {
            var calls = [];
            var WithInterfaces = java.lang.Object[ext]({
                interfaces: [java.lang.Runnable, com.tns.tests.DummyClass.MyInterface],
                run: function () {
                    calls.push("run");
                },
                echoInt: function (i) {
                    return i;
                },
                doSomething: function () {
                },
                contentKeyPairingInterfaces: true
            });
            var instance = new WithInterfaces();
            new java.lang.Thread(instance).run();
            expect(calls).toEqual(["run"]);
            expectRuntimeName(instance, "java.lang.Object_ha59a9a5b07d44d2b");
        });

        it("for a named extend", function () {
            var Named = java.lang.Object[ext]("PairingNamed", {
                toString: function () {
                    return "pairing named";
                },
                contentKeyPairingNamed: true
            });
            var instance = new Named();
            expect(javaToString(instance)).toBe("pairing named");
            expectRuntimeName(instance, "java.lang.Object_PairingNamed_h63c03cfe49192d3d");
        });

        it("for a TypeScript class", function () {
            var PairingTsClass = (function (_super) {
                tsExtends(PairingTsClass, _super);
                function PairingTsClass() {
                    var _this = _super.call(this) || this;
                    return global.__native(_this);
                }
                PairingTsClass.create = function () {
                    return new PairingTsClass();
                };
                PairingTsClass.prototype.toString = function () {
                    return "pairing ts";
                };
                PairingTsClass.prototype.contentKeyPairingTs = true;
                return PairingTsClass;
            }(java.lang.Object));
            var instance = PairingTsClass.create();
            expect(javaToString(instance)).toBe("pairing ts");
            expectRuntimeName(instance, "java.lang.Object_PairingTsClass_hc5ac23ca0a98a3c1");
        });
    });

    describe("named extends whose implementation the generator cannot read", function () {

        it("load their own class, not the generator's class without members", function () {
            var implementation = {
                toString: function () {
                    return "named variable";
                }
            };
            var NamedVariable = java.lang.Object.extend("NamedVariable", implementation);
            var instance = new NamedVariable();

            expect(javaToString(instance)).toBe("named variable");
            expect(runtimeGenerated(instance)).toBe(true);
        });

        it("load their own class even when they have no members", function () {
            var emptyImplementation = {};
            var NamedEmptyVariable = java.lang.Object.extend("NamedEmptyVariable", emptyImplementation);
            var instance = new NamedEmptyVariable();

            expect(runtimeGenerated(instance)).toBe(true);
            if (contentKeyed) {
                expect(className(instance)).toBe("java.lang.Object_NamedEmptyVariable_h86a36c7f58a561ea");
            }
        });

        it("leave the class without members to anonymous extends without members", function () {
            var Empty = java.lang.Object.extend({});
            var instance = new Empty();

            expect(runtimeGenerated(instance)).toBe(false);
            if (contentKeyed) {
                expect(className(instance)).toBe("com.tns.gen.java.lang.Object_h86a36c7f58a561ea");
            }
        });
    });

    describe("prebuilds the shapes bundlers emit", function () {
        var fixtures = require("./contentKeyedFixtures");

        Object.keys(fixtures.fixtures).forEach(function (name) {
            it("for " + name, function () {
                var fixture = fixtures.fixtures[name];
                var instance = new fixture.ctor();

                expect(javaToString(instance)).toBe(fixture.toString);
                expect(runtimeGenerated(instance)).toBe(false);
                if (contentKeyed) {
                    expect(className(instance)).toMatch(/_h[0-9a-f]{16}$/);
                }
            });
        });

        it("without running getters of the implementation object", function () {
            Object.keys(fixtures.fixtures).forEach(function (name) {
                new fixtures.fixtures[name].ctor();
            });
            if (contentKeyed) {
                expect(fixtures.getterCalls()).toBe(0);
            }
        });

        it("with the interfaces from @Interfaces", function () {
            var instance = new fixtures.fixtures.DecoratedFixture.ctor();
            new java.lang.Thread(instance).run();
            expect(instance.ran).toBe(true);
        });
    });

    describe("shared classes", function () {

        it("give call sites with the same content one class and their own constructors", function () {
            var first = new SharedFirst();
            var second = new SharedSecond();

            expect(SharedFirst).not.toBe(SharedSecond);
            expect(javaToString(first)).toBe("shared first");
            expect(javaToString(second)).toBe("shared second");
            expect(runtimeGenerated(first)).toBe(false);
            expect(runtimeGenerated(second)).toBe(false);
            if (contentKeyed) {
                expect(className(second)).toBe(className(first));
            } else {
                expect(className(second)).not.toBe(className(first));
            }
        });

        it("give a call site that runs again its first constructor", function () {
            function extendAgain() {
                return java.lang.Object.extend({
                    toString: function () {
                        return "extended again";
                    },
                    sharedContentMarker: 4
                });
            }
            var first = extendAgain();
            expect(extendAgain()).toBe(first);
            expect(javaToString(new first())).toBe("extended again");
        });

        it("give a call site with an extra member another class", function () {
            var edited = new SharedEdited();

            expect(javaToString(edited)).toBe("shared edited");
            expect(className(edited)).not.toBe(className(new SharedFirst()));
            expect(runtimeGenerated(edited)).toBe(false);
        });

        it("are created from Java with the implementation of the first call site", function () {
            var created = createFromJava(new SharedSecond());

            expect(javaToString(created)).toBe(contentKeyed ? "shared first" : "shared second");
        });
    });

    describe("named extends", function () {

        it("with the same name and content keep their call-site classes", function () {
            var first = new CollidingFirst();
            var second = new CollidingSecond();

            expect(className(first)).not.toBe(className(second));
            expect(className(first)).toMatch(/_CollidingName$/);
            expect(className(second)).toMatch(/_CollidingName$/);
            expect(runtimeGenerated(first)).toBe(false);
            expect(runtimeGenerated(second)).toBe(false);
            expect(javaToString(createFromJava(first))).toBe("colliding first");
            expect(javaToString(createFromJava(second))).toBe("colliding second");

            if (contentKeyed) {
                var collisions = com.tns.gen.ContentKeyedBindings.COLLISIONS;
                var listed = [];
                for (var i = 0; i < collisions.length; i++) {
                    listed.push(String(collisions[i]));
                }
                expect(listed.filter(function (name) {
                    return /^com\.tns\.gen\.java\.lang\.Object_CollidingName_h[0-9a-f]{16}$/.test(name);
                }).length).toBe(1);
            }
        });

        it("with the content of a name another call site took first get their call-site class", function () {
            var visible = new OwnedVisible();
            var hidden = new OwnedHidden();

            expect(className(visible)).not.toBe(className(hidden));
            expect(runtimeGenerated(visible)).toBe(false);
            expect(runtimeGenerated(hidden)).toBe(true);
            expect(javaToString(createFromJava(visible))).toBe("owned visible");
            expect(javaToString(createFromJava(hidden))).toBe("owned hidden");
            if (contentKeyed) {
                expect(className(visible)).toMatch(/^com\.tns\.gen\.java\.lang\.Object_OwnedName_h[0-9a-f]{16}$/);
                expect(className(hidden)).toMatch(/_tests_testContentKeyedBindings_[0-9a-f]{6}_\d+_\d+_OwnedName$/);
            }
        });
    });

    it("resolves and constructs content-keyed classes in a worker", function (done) {
        // Same content as the worker's call sites, so the main runtime has the
        // classes before the worker asks for them.
        var MainPrebuilt = java.lang.Object.extend({
            toString: function () {
                return "main prebuilt";
            },
            workerContentMarker: 1
        });
        var MainHidden = java.lang.Object[ext]({
            toString: function () {
                return "main hidden";
            },
            workerHiddenMarker: 1
        });
        var MainOwned = java.lang.Object[ext]("WorkerOwnedName", {
            toString: function () {
                return "main owned";
            }
        });
        var mainPrebuilt = new MainPrebuilt();
        var mainHidden = new MainHidden();
        var mainOwned = new MainOwned();

        var worker = new Worker("./contentKeyedBindingsWorker.js");
        worker.onmessage = function (msg) {
            worker.terminate();
            var result = msg.data;
            expect(result.error).toBeUndefined();
            expect(result.prebuiltString).toBe("worker prebuilt");
            expect(result.hiddenString).toBe("worker hidden");
            expect(result.hiddenCreatedString).toBe("worker hidden");
            expect(result.ownedString).toBe("worker owned");
            expect(result.ownedCreatedString).toBe("worker owned");
            expect(javaToString(createFromJava(mainOwned))).toBe("main owned");
            expect(runtimeGenerated(mainPrebuilt)).toBe(false);
            expect(runtimeGenerated(mainHidden)).toBe(true);
            if (contentKeyed) {
                expect(result.prebuiltClass).toBe(className(mainPrebuilt));
                expect(result.hiddenClass).toBe(className(mainHidden));
                // Each runtime's first call site keeps a named content-keyed class.
                expect(result.ownedClass).toBe(className(mainOwned));
                expect(className(mainOwned)).toMatch(/^com\.tns\.gen\.java\.lang\.Object_WorkerOwnedName_h[0-9a-f]{16}$/);
            }
            done();
        };
    });
});
