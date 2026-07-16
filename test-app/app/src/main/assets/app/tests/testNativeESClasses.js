describe("Tests native ES class extensions (class X extends NativeType)", function () {

    var appClassLoader = com.tns.Runtime.class.getClassLoader();

    // DummyClass.method2(Object) returns "obj=" + obj.toString(), forcing a Java-side virtual
    // toString dispatch through the generated proxy. (java.lang.String.valueOf is unusable for
    // this: accessing `java.lang.String.null` in other suites permanently replaces `valueOf` on
    // the ctor function with the runtime's null-returning valueOf.)
    function javaToString(obj) {
        return new com.tns.tests.DummyClass().method2(obj);
    }

    it("When_extending_a_class_with_es_class_syntax_instances_should_construct_and_dispatch_overrides", function () {
        class EsButton extends com.tns.tests.Button1 {
            getIMAGE_ID_PROP() {
                return "es class override";
            }
        }

        var button = new EsButton();

        expect(button instanceof EsButton).toBe(true);
        expect(button instanceof com.tns.tests.Button1).toBe(true);
        expect(button.getIMAGE_ID_PROP()).toBe("es class override");
        // non-overridden base methods still work
        expect(button.echo("hello")).toBe("hello");
    });

    it("When_java_calls_a_virtual_method_it_should_dispatch_to_the_es_class_override", function () {
        class EchoButton extends com.tns.tests.Button1 {
            echo(s) {
                return "es:" + s;
            }
        }

        var button = new EchoButton();

        // triggerEcho calls this.echo(s) on the Java side - it must route
        // through the generated proxy back into the ES class method
        expect(button.triggerEcho("x")).toBe("es:x");
    });

    it("When_calling_super_method_from_an_es_class_override_it_should_invoke_the_java_implementation", function () {
        class SuperEchoButton extends com.tns.tests.Button1 {
            echo(s) {
                return "es:" + super.echo(s);
            }
        }

        var button = new SuperEchoButton();

        expect(button.echo("y")).toBe("es:y");
        // round trip: Java triggerEcho -> proxy echo -> JS override -> super.echo -> Java echo
        expect(button.triggerEcho("z")).toBe("es:z");
    });

    it("When_the_es_class_constructor_passes_arguments_to_super_the_matching_java_constructor_should_be_used", function () {
        class CtorButton extends com.tns.tests.Button1 {
            constructor(value) {
                super(value); // Button1(int) overload
                this.value = value;
            }
        }

        var button = new CtorButton(5);

        expect(button.value).toBe(5);
        expect(button instanceof com.tns.tests.Button1).toBe(true);
    });

    it("When_accessing_the_class_property_before_any_instance_exists_the_class_should_be_registered_lazily", function () {
        class LazyTouch extends java.lang.Object {
            toString() {
                return "lazy touch";
            }
        }

        // no `new` has happened - static touch must register the proxy class
        var clazz = LazyTouch.class;

        expect(clazz).not.toBe(null);
        expect(clazz.getName()).toContain("LazyTouch");

        // the class is fully functional before any JS-side construction:
        // Java instantiates it through reflection and dispatches to the JS override
        var created = clazz.newInstance();
        expect(javaToString(created)).toBe("obj=lazy touch");
    });

    it("When_passing_the_es_class_to_a_java_method_expecting_a_class_it_should_marshal_to_java_lang_Class", function () {
        class MarshalledClass extends com.tns.tests.DummyClass {
        }

        // Class.isAssignableFrom(Class) - MarshalledClass is registered lazily during marshalling
        expect(com.tns.tests.DummyClass.class.isAssignableFrom(MarshalledClass)).toBe(true);
        expect(java.lang.Object.class.isAssignableFrom(MarshalledClass)).toBe(true);
    });

    it("When_passing_the_es_class_where_java_lang_Object_is_expected_it_should_marshal_to_its_java_lang_Class", function () {
        class ObjectMarshalledClass extends java.lang.Object {
        }

        var list = new java.util.ArrayList();
        list.add(ObjectMarshalledClass);

        var stored = list.get(0);
        expect(stored.getName()).toBe(ObjectMarshalledClass.class.getName());
    });

    it("When_extending_an_es_class_that_extends_a_native_class_each_level_should_get_its_own_proxy", function () {
        class LevelOne extends com.tns.tests.Button1 {
            getIMAGE_ID_PROP() {
                return "level one";
            }
            echo(s) {
                return "L1:" + s;
            }
        }

        class LevelTwo extends LevelOne {
            getIMAGE_ID_PROP() {
                return "level two + " + super.getIMAGE_ID_PROP();
            }
        }

        var two = new LevelTwo();
        expect(two instanceof LevelTwo).toBe(true);
        expect(two instanceof LevelOne).toBe(true);
        expect(two instanceof com.tns.tests.Button1).toBe(true);
        expect(two.getIMAGE_ID_PROP()).toBe("level two + level one");
        // method defined only on the intermediate level must be part of the proxy overrides
        expect(two.triggerEcho("q")).toBe("L1:q");

        var one = new LevelOne();
        expect(one.getIMAGE_ID_PROP()).toBe("level one");
        expect(one instanceof LevelTwo).toBe(false);

        expect(one.getClass().getName()).not.toBe(two.getClass().getName());
    });

    it("When_constructing_the_es_class_multiple_times_all_instances_should_share_one_proxy_class", function () {
        class SharedProxyButton extends com.tns.tests.Button1 {
        }

        var first = new SharedProxyButton();
        var second = new SharedProxyButton();

        expect(first.getClass().equals(second.getClass())).toBe(true);
        expect(first.getClass().equals(SharedProxyButton.class)).toBe(true);
    });

    it("When_reading_base_class_statics_through_the_es_class_they_should_resolve_to_the_java_members", function () {
        class StaticsButton extends com.tns.tests.Button1 {
            static jsStatic = 42;
            static jsStaticMethod() {
                return "js static";
            }
        }

        // Java statics are reachable through the constructor prototype chain
        expect(StaticsButton.STATIC_IMAGE_ID).toBe("static image id");
        expect(StaticsButton.SGetStaticImageId()).toBe("static image id");

        // plain JS statics live on the JS constructor and are untouched
        expect(StaticsButton.jsStatic).toBe(42);
        expect(StaticsButton.jsStaticMethod()).toBe("js static");
    });

    it("When_the_es_class_is_registered_its_proxy_should_be_discoverable_through_the_app_class_loader", function () {
        class DiscoverableEsClass extends java.lang.Object {
            toString() {
                return "discoverable es class";
            }
        }

        var instance = new DiscoverableEsClass();
        var className = instance.getClass().getName();

        var found = java.lang.Class.forName(className, false, appClassLoader);

        expect(found.getName()).toBe(className);
        expect(found.equals(instance.getClass())).toBe(true);
        expect(found.equals(DiscoverableEsClass.class)).toBe(true);
    });

    it("When_implementing_an_interface_with_es_class_syntax_java_should_dispatch_to_the_js_methods", function () {
        var runCount = 0;

        class EsRunnable extends java.lang.Runnable {
            run() {
                runCount++;
            }
        }

        var runnable = new EsRunnable();
        expect(runnable instanceof java.lang.Runnable).toBe(true);

        // Thread.run() (not started) invokes target.run() synchronously on the current thread
        var thread = new java.lang.Thread(runnable);
        thread.run();

        expect(runCount).toBe(1);
    });

    it("When_declaring_static_interfaces_the_proxy_should_implement_them", function () {
        var ran = { value: false };

        class WithInterfaces extends java.lang.Object {
            static interfaces = [java.lang.Runnable];

            run() {
                ran.value = true;
            }
        }

        var instance = new WithInterfaces();

        expect(instance instanceof java.lang.Runnable).toBe(true);

        var thread = new java.lang.Thread(instance);
        thread.run();

        expect(ran.value).toBe(true);
    });

    it("When_getting_the_class_name_it_should_be_stable_and_descriptive", function () {
        class StableNameClass extends java.lang.Object {
        }

        var name = StableNameClass.class.getName();

        // runtime generated proxies are named <base>_es<hash>_<jsClassName>, mirroring the
        // legacy <base>_<file>_<line>_<column>_<name> scheme (the com.tns.gen prefix is only
        // present on build-time pre-generated bindings)
        expect(name).toContain("java.lang.Object_es");
        expect(name).toContain("StableNameClass");
        // repeated access resolves to the very same class
        expect(StableNameClass.class.getName()).toBe(name);
    });

    it("When_passing_an_es_class_instance_to_java_and_reading_it_back_it_should_be_the_same_object", function () {
        class RoundTripObject extends java.lang.Object {
            toString() {
                return "round trip";
            }
        }

        var instance = new RoundTripObject();
        var list = new java.util.ArrayList();
        list.add(instance);

        var stored = list.get(0);
        expect(stored.equals(instance)).toBe(true);
        expect(javaToString(stored)).toBe("obj=round trip");
    });

    it("When_calling_extend_on_an_es_class_it_should_throw_a_descriptive_error", function () {
        class NotExtendable extends com.tns.tests.Button1 {
        }

        expect(function () {
            NotExtendable.extend({
                toString: function () {
                    return "should not work";
                }
            });
        }).toThrow();
    });

    it("When_extending_a_class_created_with_legacy_extend_the_old_behavior_should_be_preserved", function () {
        var LegacyButton = com.tns.tests.Button1.extend({
            getIMAGE_ID_PROP: function () {
                return "legacy override";
            }
        });

        class EsOnLegacy extends LegacyButton {
        }

        // the legacy proxy is used - ES levels above `.extend()`-created classes get no proxy of their own
        var instance = new EsOnLegacy();
        expect(instance instanceof com.tns.tests.Button1).toBe(true);
        expect(instance.getIMAGE_ID_PROP()).toBe("legacy override");
    });

    it("When_a_plain_js_class_hierarchy_is_used_the_runtime_should_not_interfere", function () {
        class PlainBase {
            value() {
                return 1;
            }
        }

        class PlainDerived extends PlainBase {
            value() {
                return super.value() + 1;
            }
        }

        var plain = new PlainDerived();
        expect(plain.value()).toBe(2);
        expect(function () {
            return plain instanceof java.lang.Object;
        }).not.toThrow();
    });

    it("When_the_NativeClass_decorator_is_applied_it_should_be_a_noop", function () {
        expect(typeof global.NativeClass).toBe("function");

        const DecoratedButton = global.NativeClass(class DecoratedButton extends com.tns.tests.Button1 {
            getIMAGE_ID_PROP() {
                return "decorated";
            }
        });

        var button = new DecoratedButton();
        expect(button.getIMAGE_ID_PROP()).toBe("decorated");
    });

    it("When_anonymous_es_classes_extend_native_types_each_should_get_a_distinct_proxy", function () {
        var First = class extends java.lang.Object {
            toString() {
                return "first anonymous";
            }
        };
        var Second = class extends java.lang.Object {
            toString() {
                return "second anonymous";
            }
        };

        var firstInstance = new First();
        var secondInstance = new Second();

        expect(javaToString(firstInstance)).toBe("obj=first anonymous");
        expect(javaToString(secondInstance)).toBe("obj=second anonymous");
        expect(firstInstance.getClass().equals(secondInstance.getClass())).toBe(false);
    });
});
