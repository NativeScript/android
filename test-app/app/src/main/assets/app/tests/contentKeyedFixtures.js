// Native classes in the shapes bundlers emit for `@NativeClass` TypeScript and for
// plain JS. The static binding generator must prebuild each of them under the
// class name the runtime asks for.

var getterCalls = 0;

function profile(target, key, descriptor) {
    return descriptor;
}

var DecoratedFixture = /** @class */ (function (_super) {
    __extends(DecoratedFixture, _super);
    function DecoratedFixture() {
        var _this = _super.call(this) || this;
        _this.backgroundBitmap = null;
        return global.__native(_this);
    }
    DecoratedFixture.newInstance = function () {
        return new DecoratedFixture();
    };
    DecoratedFixture.prototype.toString = function () {
        return "decorated fixture";
    };
    DecoratedFixture.prototype.run = function () {
        this.ran = true;
    };
    Object.defineProperty(DecoratedFixture.prototype, "items", {
        get: function () {
            getterCalls++;
            return [];
        },
        enumerable: true,
        configurable: true
    });
    __decorate([profile], DecoratedFixture.prototype, "toString", null);
    DecoratedFixture = __decorate([Interfaces([java.lang.Runnable])], DecoratedFixture);
    return DecoratedFixture;
}(java.lang.Object));

var ApplyFixture = function (_super) {
    __extends(ApplyFixture, _super);
    function ApplyFixture() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this.count = 0;
        return _this;
    }
    ApplyFixture.prototype.toString = function () {
        return "apply fixture";
    };
    ApplyFixture.prototype.applyFixtureMarker = 1;
    return ApplyFixture;
}(java.lang.Object);

var NoConstructorFixture = /** @class */ (function (_super) {
    __extends(NoConstructorFixture, _super);
    function NoConstructorFixture() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    NoConstructorFixture.prototype.toString = function () {
        return "no constructor fixture";
    };
    NoConstructorFixture.prototype.noConstructorFixtureMarker = 1;
    return NoConstructorFixture;
}(java.lang.Object));

var QuotedKeysFixture = java.lang.Object.extend({
    "toString": function () {
        return "quoted keys fixture";
    },
    "quotedKeysFixtureMarker": 1,
    get ignoredGetter() {
        getterCalls++;
        return 1;
    }
});

var NestedBaseFixture = com.tns.tests.DummyClass.DummyDerivedClass.extend({
    toString: function () {
        return "nested base fixture";
    },
    nestedBaseFixtureMarker: 1
});

exports.fixtures = {
    DecoratedFixture: { ctor: DecoratedFixture, toString: "decorated fixture" },
    ApplyFixture: { ctor: ApplyFixture, toString: "apply fixture" },
    NoConstructorFixture: { ctor: NoConstructorFixture, toString: "no constructor fixture" },
    QuotedKeysFixture: { ctor: QuotedKeysFixture, toString: "quoted keys fixture" },
    NestedBaseFixture: { ctor: NestedBaseFixture, toString: "nested base fixture", appModuleBase: "com.tns.tests.DummyClass_DummyDerivedClass" }
};
exports.getterCalls = function () {
    return getterCalls;
};
