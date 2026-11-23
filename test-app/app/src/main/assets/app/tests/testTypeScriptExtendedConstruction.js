describe("TypeScript-extended native class construction", function () {
    var Holder = com.tns.tests.WrapperHolder;
    var Button1 = com.tns.tests.Button1;

    function collectBoth() {
        for (var k = 0; k < 2; k++) {
            gc();
            java.lang.System.nanoTime();
            for (var i = 0; i < 3; i++) {
                java.lang.Runtime.getRuntime().gc();
                java.lang.System.runFinalization();
            }
            gc();
            java.lang.System.nanoTime();
        }
    }

    afterEach(function () {
        Holder.held = null;
    });

    // TypeScript's ES5 output for
    //   class EchoButton extends com.tns.tests.Button1 {
    //     label = "L" + value; count = 0;
    //     constructor(value) { super(value); }
    //     echo(s) { ... super.echo(s) ... } describe() { ... } static kind = "echo";
    //   }
    var EchoButton = (function (_super) {
        __extends(EchoButton, _super);
        function EchoButton(value) {
            var _this = _super.call(this, value) || this;
            _this.label = "L" + value;
            _this.count = 0;
            return global.__native(_this);
        }
        EchoButton.prototype.echo = function (s) {
            this.count++;
            this.lastEcho = s;
            return this.label + ":" + _super.prototype.echo.call(this, s);
        };
        EchoButton.prototype.describe = function () {
            return this.label + "/" + this.count;
        };
        EchoButton.prototype.superToString = function () {
            return this.super.toString();
        };
        EchoButton.kind = "echo";
        return EchoButton;
    })(com.tns.tests.Button1);

    it("is an instance of its class and of every Java class above it", function () {
        var button = new EchoButton(1);
        expect(button instanceof EchoButton).toBe(true);
        expect(button instanceof Button1).toBe(true);
        expect(button instanceof com.tns.tests.ButtonBase).toBe(true);
        expect(button instanceof java.lang.Object).toBe(true);
        expect(new Button1() instanceof EchoButton).toBe(false);
    });

    it("shares one prototype chain between instances, with the class prototype on it", function () {
        var a = new EchoButton(1);
        var b = new EchoButton(2);
        var proto = Object.getPrototypeOf(a);
        expect(Object.getPrototypeOf(b)).toBe(proto);
        expect(Object.getPrototypeOf(proto)).toBe(EchoButton.prototype);
        expect(Object.getPrototypeOf(EchoButton.prototype)).toBe(Button1.prototype);
        expect(a.constructor.prototype).toBe(proto);
        expect(a instanceof a.constructor).toBe(true);
    });

    it("keeps the constructor's field initializers as own properties of the instance", function () {
        var button = new EchoButton(3);
        expect(Object.keys(button).sort()).toEqual(["count", "label"]);
        expect(button.label).toBe("L3");
        expect(button.count).toBe(0);
    });

    it("keeps the state of each instance apart", function () {
        var a = new EchoButton(1);
        var b = new EchoButton(2);
        a.extra = "a";
        a.triggerEcho("x");
        expect(b.extra).toBeUndefined();
        expect(a.count).toBe(1);
        expect(b.count).toBe(0);
        expect(b.describe()).toBe("L2/0");
    });

    it("runs the JS override when Java calls the method, with the instance as this", function () {
        var button = new EchoButton(4);
        expect(button.triggerEcho("hi")).toBe("L4:hi");
        expect(button.count).toBe(1);
        expect(button.lastEcho).toBe("hi");
        expect(button.describe()).toBe("L4/1");
    });

    it("calls the Java implementation through super", function () {
        var button = new EchoButton(5);
        expect(button.echo("s")).toBe("L5:s");
        var label = button.superToString();
        expect(label.indexOf("com.tns.tests.Button1_")).not.toBe(-1);
    });

    it("calls inherited Java methods and passes the instance to Java and back", function () {
        var button = new EchoButton(6);
        expect(button.getIMAGE_ID_PROP()).toBe("image id prop");
        var list = new java.util.ArrayList();
        list.add(button);
        expect(list.get(0)).toBe(button);
    });

    it("keeps the class's static members and inherits the Java ones", function () {
        expect(EchoButton.kind).toBe("echo");
        expect(EchoButton.SGetStaticImageId()).toBe("static image id");
    });

    it("keeps the prototype chain and the expandos of a Java-held instance after a collection", function () {
        var proto = (function () {
            var button = new EchoButton(7);
            button.expando = "kept";
            Holder.held = button;
            return Object.getPrototypeOf(button);
        })();
        collectBoth();

        var button = Holder.held;
        expect(button.triggerEcho("r")).toBe("L7:r");
        expect(Object.getPrototypeOf(button)).toBe(proto);
        expect(button instanceof EchoButton).toBe(true);
        expect(button.expando).toBe("kept");
        expect(button.describe()).toBe("L7/1");
    });

    describe("when the constructor forwards its arguments with apply", function () {
        // TypeScript's output for a class without a constructor, and for `super(...args)`.
        var DefaultButton = (function (_super) {
            __extends(DefaultButton, _super);
            function DefaultButton() {
                return _super !== null && _super.apply(this, arguments) || this;
            }
            DefaultButton.prototype.echo = function (s) {
                return "default:" + s;
            };
            return DefaultButton;
        })(com.tns.tests.Button1);
        var SpreadButton = (function (_super) {
            __extends(SpreadButton, _super);
            function SpreadButton(value) {
                var _this = _super.apply(this, [value]) || this;
                _this.value = value;
                return _this;
            }
            SpreadButton.prototype.echo = function (s) {
                return this.value + ":" + s;
            };
            return SpreadButton;
        })(com.tns.tests.Button1);

        it("constructs the instance with the forwarded arguments", function () {
            var plain = new DefaultButton();
            expect(plain instanceof DefaultButton).toBe(true);
            expect(plain.triggerEcho("a")).toBe("default:a");

            var spread = new SpreadButton(3);
            expect(spread instanceof SpreadButton).toBe(true);
            expect(Object.keys(spread)).toEqual(["value"]);
            expect(spread.triggerEcho("b")).toBe("3:b");
        });
    });
});
