// Java->JS calls look the method up on the instance on every call, so an own
// property of the instance or a method replaced after extend() is what runs. Debug
// builds keep calling it but log a warning once per class and method, since the
// iOS runtime binds the class's methods at extend time instead. Every extend() spec
// uses a class of its own: the warning names the class, and each is logged only once.
// Interfaces are named in full so the static binding generator implements them.
describe("Java->JS calls through a function other than the class's own", function () {
    var LogcatUtil = com.tns.tests.LogcatUtil;

    function runFromJava(runnable) {
        new java.util.concurrent.FutureTask(runnable, null).run();
    }

    function warnings(className) {
        return LogcatUtil.countJsWarnings(className, 'Java called "run" on ');
    }

    it("calls the class's own method without a warning", function () {
        var runs = 0;
        var C = java.lang.Object.extend("DispatchWarnOwnMethod", {
            interfaces: [java.lang.Runnable],
            run: function () { runs++; }
        });
        var instance = new C();

        runFromJava(instance);
        runFromJava(new C());

        expect(runs).toBe(2);
        expect(warnings("DispatchWarnOwnMethod")).toBe(0);
    });

    it("calls an instance's own property and warns", function () {
        var classRuns = 0;
        var ownRuns = 0;
        var C = java.lang.Object.extend("DispatchWarnInstanceProperty", {
            interfaces: [java.lang.Runnable],
            run: function () { classRuns++; }
        });
        var instance = new C();
        instance.run = function () { ownRuns++; };

        runFromJava(instance);

        expect(ownRuns).toBe(1);
        expect(classRuns).toBe(0);
        expect(warnings("DispatchWarnInstanceProperty")).toBe(1);
    });

    it("calls a TypeScript class field and warns", function () {
        var fieldRuns = 0;
        var FieldClass = (function (_super) {
            __extends(DispatchWarnClassField, _super);
            function DispatchWarnClassField() {
                var _this = _super.call(this) || this;
                _this.run = function () { fieldRuns++; };
                return _this;
            }
            DispatchWarnClassField = __decorate([
                Interfaces([java.lang.Runnable])
            ], DispatchWarnClassField);
            return DispatchWarnClassField;
        }(java.lang.Object));
        var instance = new FieldClass();

        runFromJava(instance);

        expect(fieldRuns).toBe(1);
        expect(warnings("DispatchWarnClassField")).toBe(1);
    });

    it("calls a method replaced after extend and warns", function () {
        var originalRuns = 0;
        var replacedRuns = 0;
        var implementation = {
            interfaces: [java.lang.Runnable],
            run: function () { originalRuns++; }
        };
        var C = java.lang.Object.extend("DispatchWarnReplacedMethod", implementation);
        var instance = new C();
        implementation.run = function () { replacedRuns++; };

        runFromJava(instance);

        expect(replacedRuns).toBe(1);
        expect(originalRuns).toBe(0);
        expect(warnings("DispatchWarnReplacedMethod")).toBe(1);
    });

    it("calls a method added to the class's prototype after extend and warns", function () {
        var originalRuns = 0;
        var addedRuns = 0;
        var C = java.lang.Object.extend("DispatchWarnPrototypeMethod", {
            interfaces: [java.lang.Runnable],
            run: function () { originalRuns++; }
        });
        C.prototype.run = function () { addedRuns++; };

        runFromJava(new C());

        expect(addedRuns).toBe(1);
        expect(originalRuns).toBe(0);
        expect(warnings("DispatchWarnPrototypeMethod")).toBe(1);
    });

    it("calls an instance's function for a member the class defined as null and warns", function () {
        var ownRuns = 0;
        var C = java.lang.Object.extend("DispatchWarnNullMember", {
            interfaces: [java.lang.Runnable],
            run: null
        });
        var instance = new C();
        instance.run = function () { ownRuns++; };

        runFromJava(instance);
        runFromJava(instance);

        expect(ownRuns).toBe(2);
        expect(warnings("DispatchWarnNullMember")).toBe(1);
    });

    it("warns once however often and on however many instances the call repeats", function () {
        var ownRuns = 0;
        var C = java.lang.Object.extend("DispatchWarnRepeated", {
            interfaces: [java.lang.Runnable],
            run: function () {}
        });
        for (var i = 0; i < 3; i++) {
            var instance = new C();
            instance.run = function () { ownRuns++; };
            runFromJava(instance);
            runFromJava(instance);
        }

        expect(ownRuns).toBe(6);
        expect(warnings("DispatchWarnRepeated")).toBe(1);
    });

    // Every `new Runnable({...})` shares one Java class, so the spec without an override
    // runs first and both count only the warnings their own calls add.
    it("calls an interface implementation's method without a warning", function () {
        var runs = 0;
        var runnable = new java.lang.Runnable({ run: function () { runs++; } });
        var className = String(runnable.getClass().getName());
        var before = warnings(className);

        runFromJava(runnable);

        expect(runs).toBe(1);
        expect(warnings(className)).toBe(before);
    });

    it("calls an own property of an interface implementation and warns once", function () {
        var implementationRuns = 0;
        var ownRuns = 0;
        var runnable = new java.lang.Runnable({ run: function () { implementationRuns++; } });
        var className = String(runnable.getClass().getName());
        var before = warnings(className);
        runnable.run = function () { ownRuns++; };

        runFromJava(runnable);
        runFromJava(runnable);

        expect(ownRuns).toBe(2);
        expect(implementationRuns).toBe(0);
        expect(warnings(className)).toBe(before + 1);
    });

    it("calls an instance's own property on an instance Java created and warns", function () {
        var ownRuns = 0;
        var C = java.lang.Object.extend("DispatchWarnJavaCreated", {
            interfaces: [java.lang.Runnable],
            run: function () {}
        });
        var instance = com.tns.tests.JavaCreated.newInstance(new C().getClass());
        instance.run = function () { ownRuns++; };

        runFromJava(instance);

        expect(ownRuns).toBe(1);
        expect(warnings("DispatchWarnJavaCreated")).toBe(1);
    });

    // The getter on Object.prototype.value is installed only around extend(): while it
    // is there, Object.defineProperty reads every accessor descriptor as having a value.
    it("runs no inherited getter while extend() captures an accessor", function () {
        var inheritedReads = 0;
        var implementation = {
            interfaces: [java.lang.Runnable],
            run: function () {}
        };
        Object.defineProperty(implementation, "state", {
            get: function () { return 1; },
            enumerable: true,
            configurable: true
        });

        Object.defineProperty(Object.prototype, "value", {
            get: function () { inheritedReads++; return undefined; },
            configurable: true
        });
        try {
            java.lang.Object.extend("DispatchWarnAccessorCapture", implementation);
        } finally {
            delete Object.prototype.value;
        }

        expect(inheritedReads).toBe(0);
    });
});
