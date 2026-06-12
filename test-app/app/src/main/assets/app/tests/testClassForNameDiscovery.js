describe("Tests Class.forName discovery of runtime generated classes", function () {

    // Android framework components (e.g. FragmentFactory) resolve classes with
    // Class.forName(className, false, context.getClassLoader()). Runtime generated
    // proxies must be discoverable through the app's class loader, otherwise
    // framework lookups crash with ClassNotFoundException (see issue #1962 / PR #1951).
    //
    // The extend calls below are built dynamically so the static binding generator
    // cannot pre-generate the proxies and DexFactory.resolveClass takes the runtime
    // generation + parent class loader injection path.
    var ext = "ex" + "tend";

    // the app's PathClassLoader — the same loader the framework uses,
    // e.g. in FragmentFactory.loadFragmentClass via context.getClassLoader()
    var appClassLoader = com.tns.Runtime.class.getClassLoader();

    it("When_extending_a_class_at_runtime_it_should_be_discoverable_through_the_app_class_loader", function () {
        var MyObject = java.lang.Object[ext]("ClassForNameDiscoveryObject", {
            toString: function () {
                return "discoverable";
            }
        });

        var instance = new MyObject();
        var className = instance.getClass().getName();

        var found = java.lang.Class.forName(className, false, appClassLoader);

        expect(found.getName()).toBe(className);
        expect(found.equals(instance.getClass())).toBe(true);
    });

    it("When_implementing_an_interface_at_runtime_it_should_be_discoverable_through_the_app_class_loader", function () {
        var MyRunnable = java.lang.Runnable[ext]("ClassForNameDiscoveryRunnable", {
            run: function () {
            }
        });

        var instance = new MyRunnable();
        var className = instance.getClass().getName();

        var found = java.lang.Class.forName(className, false, appClassLoader);

        expect(found.getName()).toBe(className);
        expect(found.equals(instance.getClass())).toBe(true);
    });

    it("When_a_runtime_generated_class_is_instantiated_through_reflection_it_should_dispatch_to_the_JS_implementation", function () {
        var MyObject = java.lang.Object[ext]("ClassForNameDiscoveryInstantiated", {
            toString: function () {
                return "created via reflection";
            }
        });

        // make sure the implementation object is registered before Java constructs an instance
        var instance = new MyObject();
        var className = instance.getClass().getName();

        // FragmentFactory resolves the class by name and instantiates it through
        // reflection. Class.newInstance() invokes the no-arg constructor without
        // the varargs marshalling getDeclaredConstructor() would need.
        var found = java.lang.Class.forName(className, false, appClassLoader);
        var created = found.newInstance();

        expect(created.toString()).toBe("created via reflection");
    });
});
