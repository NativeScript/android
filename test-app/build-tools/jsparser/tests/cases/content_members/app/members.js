var other = { spreadMember: function () {} };
var computedName = "computedMember";

var LiteralMembers = java.lang.Object.extend("LiteralMembers", {
    identifierMember: function () {},
    "quotedMember": function () {},
    "quoted-invalid": function () {},
    shorthandMember() {},
    dataMember: 1,
    get getterMember() { return 1; },
    set setterMember(value) {},
    [computedName]: function () {},
    ...other,
    42: function () {},
    __proto__: null,
    identifierMember: function () {},
    interfaces: [java.lang.Runnable],
    run: function () {}
});

var AccessorMembers = java.lang.Object.extend("AccessorMembers", {
    get getOnly() { return 1; },
    set setOnly(value) {},
    get pair() { return 1; },
    set pair(value) {},
    get "quotedAccessor"() { return 1; },
    get accessorThenData() { return 1; },
    accessorThenData: function () {},
    dataThenAccessor: function () {},
    get dataThenAccessor() { return 1; },
    get toString() { return function () { return "accessor"; }; }
});

var AccessorInterface = java.lang.Runnable.extend("AccessorInterface", {
    get run() { return function () {}; }
});

var PrototypeMembers = /** @class */ (function (_super) {
    __extends(PrototypeMembers, _super);
    function PrototypeMembers() {
        var _this = _super.call(this) || this;
        _this.instanceField = null;
        return global.__native(_this);
    }
    PrototypeMembers.newInstance = function () { return new PrototypeMembers(); };
    PrototypeMembers.staticField = 1;
    PrototypeMembers.prototype.toString = function () { return "members"; };
    PrototypeMembers.prototype["quotedPrototypeMember"] = function () {};
    PrototypeMembers.prototype.chainedA = PrototypeMembers.prototype.chainedB = function () {};
    PrototypeMembers.prototype.sequenceA = function () {}, PrototypeMembers.prototype.sequenceB = 1;
    PrototypeMembers.prototype.toString = function () { return "redefined"; };
    Object.defineProperty(PrototypeMembers.prototype, "accessorMember", {
        get: function () { return 1; },
        enumerable: true,
        configurable: true
    });
    __decorate([someDecorator], PrototypeMembers.prototype, "toString", null);
    PrototypeMembers = __decorate([Interfaces([java.lang.Runnable])], PrototypeMembers);
    return PrototypeMembers;
}(java.lang.Object));

var ApplyInDeclaration = function (_super) {
    __extends(ApplyInDeclaration, _super);
    function ApplyInDeclaration() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this.count = 0;
        return _this;
    }
    ApplyInDeclaration.prototype.toString = function () { return "apply"; };
    return ApplyInDeclaration;
}(java.lang.Object);
