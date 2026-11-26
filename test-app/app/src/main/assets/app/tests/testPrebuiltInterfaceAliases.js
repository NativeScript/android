describe("Interfaces of prebuilt classes", function () {
    var RunnableAlias = java.lang.Runnable;

    function runtimeGeneratedEntries(loadedName) {
        var lines = com.tns.DexFactory.getRuntimeGeneratedClasses();
        var result = [];
        for (var i = 0; i < lines.length; i++) {
            var fields = String(lines[i]).split(" ");
            if (fields[2] === loadedName) {
                result.push({ source: fields[0], loaded: fields[2], requested: fields[3] });
            }
        }
        return result;
    }

    function runFromJava(instance) {
        java.util.concurrent.Executors.callable(instance).call();
    }

    it("loads a class whose interface is listed through a local alias from the APK", function () {
        var runs = 0;
        var Aliased = (function (_super) {
            __extends(Aliased, _super);
            function Aliased() {
                var _this = _super.call(this) || this;
                return global.__native(_this);
            }
            Aliased.prototype.run = function () {
                runs++;
            };
            Aliased = __decorate([
                Interfaces([RunnableAlias])
            ], Aliased);
            return Aliased;
        }(java.lang.Object));
        var instance = new Aliased();

        expect(java.lang.Runnable.class.isAssignableFrom(instance.getClass())).toBe(true);
        runFromJava(instance);
        expect(runs).toBe(1);
        expect(runtimeGeneratedEntries(instance.getClass().getName())).toEqual([]);
    });

    it("generates a class whose interface the static binding generator cannot name at runtime", function () {
        function runnableType() {
            return java.lang.Runnable;
        }
        var runs = 0;
        var Unnamed = (function (_super) {
            __extends(Unnamed, _super);
            function Unnamed() {
                var _this = _super.call(this) || this;
                return global.__native(_this);
            }
            Unnamed.prototype.run = function () {
                runs++;
            };
            Unnamed = __decorate([
                Interfaces([runnableType()])
            ], Unnamed);
            return Unnamed;
        }(java.lang.Object));
        var instance = new Unnamed();

        expect(java.lang.Runnable.class.isAssignableFrom(instance.getClass())).toBe(true);
        runFromJava(instance);
        expect(runs).toBe(1);
        expect(runtimeGeneratedEntries(instance.getClass().getName()).length).toBe(1);
    });

    it("generates a class in place of a prebuilt one built without a method added after the class body, and warns", function () {
        var LateMember = (function (_super) {
            __extends(LateMember, _super);
            function LateMember() {
                var _this = _super.call(this) || this;
                return global.__native(_this);
            }
            LateMember.prototype.run = function () {
            };
            return LateMember;
        }(java.lang.Object));
        LateMember.prototype.toString = function () {
            return "late member";
        };
        var instance = new LateMember();

        var name = String(instance.getClass().getName());
        expect(name).toMatch(/_LateMember_runtime$/);
        expect(String(java.lang.String.valueOf(instance))).toBe("late member");
        var prebuiltName = "com.tns.gen." + name.replace(/_runtime$/, "");
        expect(com.tns.tests.LogcatUtil.countJsWarnings(prebuiltName, "was built without toString")).toBe(1);
    });

    it("generates a class in place of a prebuilt one that lacks a listed interface, and warns", function () {
        var runs = 0;
        var LateInterface = (function (_super) {
            __extends(LateInterface, _super);
            function LateInterface() {
                var _this = _super.call(this) || this;
                return global.__native(_this);
            }
            LateInterface.prototype.run = function () {
                runs++;
            };
            return LateInterface;
        }(java.lang.Object));
        // The generator builds the class from the class body, without the interface.
        LateInterface.prototype.interfaces = [java.lang.Runnable];
        var instance = new LateInterface();

        var name = String(instance.getClass().getName());
        expect(name).toMatch(/_LateInterface_runtime$/);
        expect(java.lang.Runnable.class.isAssignableFrom(instance.getClass())).toBe(true);
        runFromJava(instance);
        expect(runs).toBe(1);

        // Java creates an instance by the generated class alone.
        runFromJava(com.tns.tests.JavaCreated.newInstance(instance.getClass()));
        expect(runs).toBe(2);

        var prebuiltName = "com.tns.gen." + name.replace(/_runtime$/, "");
        expect(com.tns.tests.LogcatUtil.countJsWarnings(prebuiltName, "does not implement java.lang.Runnable")).toBe(1);
    });
});
