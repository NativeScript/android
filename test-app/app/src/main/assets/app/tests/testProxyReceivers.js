// Reactive frameworks (Vue's reactive(), MobX observables) hand Java wrappers
// around behind a JS Proxy, so the Proxy reaches the runtime both as `this`
// and as a method argument.
describe("Proxy receivers and arguments", function () {

    function passThrough() {
        return {
            get: function (target, key, receiver) {
                return Reflect.get(target, key, receiver);
            },
            set: function (target, key, value, receiver) {
                return Reflect.set(target, key, value, receiver);
            }
        };
    }

    it("calls an instance method on the proxied Java object", function () {
        var sb = new java.lang.StringBuilder();
        var proxy = new Proxy(sb, {});

        proxy.append("abc");

        expect(sb.toString()).toBe("abc");
        expect(proxy.length()).toBe(3);
    });

    it("calls an instance method through a Proxy with traps", function () {
        var sb = new java.lang.StringBuilder();
        var proxy = new Proxy(sb, passThrough());

        proxy.append("x").append("y");

        expect(sb.toString()).toBe("xy");
    });

    it("calls an instance method through nested Proxies", function () {
        var sb = new java.lang.StringBuilder("a");
        var proxy = new Proxy(new Proxy(sb, {}), passThrough());

        proxy.append("b");

        expect(sb.toString()).toBe("ab");
    });

    it("calls a static method through a Proxy of the class", function () {
        var IntegerProxy = new Proxy(java.lang.Integer, {});
        var StringProxy = new Proxy(java.lang.String, passThrough());

        expect(IntegerProxy.parseInt("5")).toBe(5);
        expect(StringProxy.format("%d", [java.lang.Integer.valueOf(3)])).toBe("3");
    });

    it("reads and writes a public Java field through a Proxy", function () {
        var point = new android.graphics.Point(1, 2);
        var proxy = new Proxy(point, passThrough());

        expect(proxy.x).toBe(1);
        expect(proxy.y).toBe(2);

        proxy.y = 7;

        expect(point.y).toBe(7);
        expect(proxy.y).toBe(7);
    });

    it("converts a proxied Java object to the Java toString()", function () {
        var sb = new java.lang.StringBuilder("hello");
        var proxy = new Proxy(sb, {});

        expect(proxy.toString()).toBe("hello");
        expect(String(proxy)).toBe("hello");
        expect("" + proxy).toBe("hello");
        expect(`${proxy}`).toBe("hello");
    });

    it("passes a proxied Java object as a method argument", function () {
        var sb = new java.lang.StringBuilder("item");
        var list = new java.util.ArrayList();

        list.add(new Proxy(sb, {}));

        expect(list.size()).toBe(1);
        expect(list.get(0).equals(sb)).toBe(true);
        expect(list.contains(new Proxy(new Proxy(sb, {}), {}))).toBe(true);
    });

    it("assigns a proxied Java object to a Java array element", function () {
        var arr = Array.create(java.lang.Object, 1);
        var sb = new java.lang.StringBuilder("el");

        arr[0] = new Proxy(sb, {});

        expect(arr[0].equals(sb)).toBe(true);
    });

    it("passes a proxied JS array of Java objects as a Java array", function () {
        var a = new java.io.File("/a");
        var b = new java.io.File("/b");

        var plain = java.util.Arrays.asList([a, b]);
        var proxied = java.util.Arrays.asList(new Proxy([a, b], passThrough()));

        expect(plain.size()).toBe(2);
        expect(proxied.size()).toBe(2);
        expect(proxied.get(0).getPath()).toBe("/a");
        expect(proxied.get(1).equals(b)).toBe(true);
    });

    it("passes proxied elements of a JS array as Java objects", function () {
        var a = new java.io.File("/a");
        var b = new java.io.File("/b");

        var list = java.util.Arrays.asList(new Proxy([new Proxy(a, {}), b], {}));

        expect(list.size()).toBe(2);
        expect(list.get(0).equals(a)).toBe(true);
    });

    function echoIntArray(arr) {
        var echoed = new com.tns.tests.JniReferenceLeakTest(null).echoIntArray(arr);
        var result = [];
        for (var i = 0; i < echoed.length; i++) {
            result.push(echoed[i]);
        }
        return result;
    }

    it("reads a proxied JS array through its get trap when converting to a Java array", function () {
        var proxy = new Proxy([], {
            get: function (target, key, receiver) {
                if (key === "length") {
                    return 3;
                }
                if (key === "0" || key === "1" || key === "2") {
                    return Number(key) * 10;
                }
                return Reflect.get(target, key, receiver);
            }
        });

        expect(echoIntArray(proxy)).toEqual([0, 10, 20]);
    });

    it("passes the values a reactive-style get trap unwraps from refs", function () {
        function ref(value) {
            return { __v_isRef: true, value: value };
        }
        var proxy = new Proxy([ref(1), 2, ref(3)], {
            get: function (target, key, receiver) {
                var v = Reflect.get(target, key, receiver);
                return v !== null && typeof v === "object" && v.__v_isRef ? v.value : v;
            }
        });

        expect(echoIntArray(proxy)).toEqual([1, 2, 3]);
    });

    it("throws the get trap's error when converting a proxied JS array", function () {
        var proxy = new Proxy([1, 2], {
            get: function (target, key, receiver) {
                if (key === "1") {
                    throw new Error("boom");
                }
                return Reflect.get(target, key, receiver);
            }
        });

        expect(function () { echoIntArray(proxy); }).toThrowError(/boom/);
    });

    it("converts a proxied Java object or typed array from its target, bypassing traps", function () {
        var lying = {
            get: function (target, key, receiver) {
                if (key === "length") {
                    return 0;
                }
                if (key === "0") {
                    return 99;
                }
                var v = Reflect.get(target, key, target);
                return typeof v === "function" ? v.bind(target) : v;
            }
        };
        var source = new java.util.ArrayList();
        source.add("a");
        source.add("b");
        var bytes = new Uint8Array([7, 8, 9]);

        var copy = new java.util.ArrayList(new Proxy(source, lying));
        var holder = new java.util.ArrayList();
        holder.add(new Proxy(bytes, lying));
        var buffer = holder.toString();

        expect(copy.size()).toBe(2);
        expect(copy.get(1)).toBe("b");
        expect(buffer).toMatch(/ByteBuffer.*cap=3/);
    });

    it("throws when the receiver is a revoked Proxy", function () {
        var sb = new java.lang.StringBuilder();
        var revocable = Proxy.revocable(sb, {});
        var append = revocable.proxy.append;
        var xGetter = Object.getOwnPropertyDescriptor(android.graphics.Point.prototype, "x").get;
        var pointRevocable = Proxy.revocable(new android.graphics.Point(1, 2), {});

        revocable.revoke();
        pointRevocable.revoke();

        expect(function () { append.call(revocable.proxy, "x"); }).toThrowError(TypeError, /revoked Proxy/);
        expect(function () { xGetter.call(pointRevocable.proxy); }).toThrowError(TypeError, /revoked Proxy/);
        expect(sb.length()).toBe(0);
    });

    it("throws when a revoked Proxy is passed as an argument", function () {
        var revocable = Proxy.revocable(new java.lang.StringBuilder(), {});
        var list = new java.util.ArrayList();

        revocable.revoke();

        expect(function () { list.add(revocable.proxy); }).toThrow();
        expect(list.size()).toBe(0);
    });

    it("throws when the Proxy target is not a Java object", function () {
        var sb = new java.lang.StringBuilder();
        var append = sb.append;
        var toString = sb.toString;

        expect(function () { append.call(new Proxy({}, {}), "x"); }).toThrowError(TypeError, /not a Java object/);
        expect(function () { toString.call(new Proxy([], {})); }).toThrowError(TypeError, /not a Java object/);
        expect(sb.length()).toBe(0);
    });

    it("throws instead of crashing when the receiver is a plain object", function () {
        var append = new java.lang.StringBuilder().append;

        expect(function () { append.call({}, "x"); }).toThrow();
    });

    it("supports instanceof on a proxied Java object", function () {
        var sb = new java.lang.StringBuilder();
        var proxy = new Proxy(sb, {});
        var revocable = Proxy.revocable(new java.lang.StringBuilder(), {});
        revocable.revoke();

        // Class checks walk the Proxy's prototype chain (the target's);
        // interface checks go through the runtime's Symbol.hasInstance.
        expect(proxy instanceof java.lang.StringBuilder).toBe(true);
        expect(proxy instanceof java.lang.CharSequence).toBe(true);
        expect(proxy instanceof java.util.List).toBe(false);
        expect(revocable.proxy instanceof java.lang.CharSequence).toBe(false);
        expect(function () { return revocable.proxy instanceof java.lang.StringBuilder; }).toThrowError(TypeError);
    });

    it("calls overridden and super methods of an extended class instance through a Proxy", function () {
        var MyObject = java.lang.Object.extend({
            toString: function () {
                return "custom:" + (this.super.hashCode() === this.hashCode());
            }
        });
        var instance = new MyObject();
        var proxy = new Proxy(instance, passThrough());

        expect(proxy.toString()).toBe("custom:true");
        expect(String(proxy)).toBe("custom:true");
        expect(proxy.hashCode()).toBe(instance.hashCode());
        expect(proxy.super.hashCode()).toBe(instance.hashCode());
        expect(proxy.equals(instance)).toBe(true);
    });

    it("releases the native counterpart of the Proxy target", function () {
        var target = new java.lang.Object();
        var proxy = new Proxy(target, passThrough());
        var revocable = Proxy.revocable(new java.lang.Object(), {});
        revocable.revoke();

        global.__releaseNativeCounterpart(proxy);

        expect(function () { global.__releaseNativeCounterpart(target); }).toThrowError(/non native object/);
        expect(function () { global.__releaseNativeCounterpart(proxy); }).toThrowError(/non native object/);
        expect(function () { target.toString(); }).toThrowError(/no longer has available Java instance counterpart/);
        expect(function () { global.__releaseNativeCounterpart(revocable.proxy); }).toThrowError(/non native object/);
    });
});
