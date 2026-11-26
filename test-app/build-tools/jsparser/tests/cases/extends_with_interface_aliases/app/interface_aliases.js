var __extends = (this && this.__extends) || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
};
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
const Observer = java.util.Observer;
var util = java.util;
var Formattable = util.Formattable;
const Runnable = java.lang.Runnable;
var _imported__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(0);

var AliasedObject = (function (_super) {
    __extends(AliasedObject, _super);
    function AliasedObject() {
        var _this = _super.call(this) || this;
        return global.__native(_this);
    }
    AliasedObject_1 = AliasedObject;
    AliasedObject.prototype.update = function () {
    };
    AliasedObject.prototype.formatTo = function () {
    };
    var AliasedObject_1;
    AliasedObject = AliasedObject_1 = __decorate([
        Interfaces([Observer, Formattable])
    ], AliasedObject);
    return AliasedObject;
}(java.lang.Object));

var ImportedInterfaceObject = (function (_super) {
    __extends(ImportedInterfaceObject, _super);
    function ImportedInterfaceObject() {
        var _this = _super.call(this) || this;
        return global.__native(_this);
    }
    ImportedInterfaceObject.prototype.update = function () {
    };
    ImportedInterfaceObject = __decorate([
        Interfaces([_imported__WEBPACK_IMPORTED_MODULE_0__.Observer])
    ], ImportedInterfaceObject);
    return ImportedInterfaceObject;
}(java.lang.Object));

var AliasedExtend = java.lang.Object.extend({
    run: function () {
    },
    interfaces: [Runnable]
});

var NamedImportedInterfaceObject = (function (_super) {
    __extends(NamedImportedInterfaceObject, _super);
    function NamedImportedInterfaceObject() {
        var _this = _super.call(this) || this;
        return global.__native(_this);
    }
    NamedImportedInterfaceObject.prototype.update = function () {
    };
    NamedImportedInterfaceObject = __decorate([
        JavaProxy("org.nativescript.aliases.NamedImported"),
        Interfaces([_imported__WEBPACK_IMPORTED_MODULE_0__.Observer])
    ], NamedImportedInterfaceObject);
    return NamedImportedInterfaceObject;
}(java.lang.Object));

var NamedCalledExtend = java.lang.Object.extend("org.nativescript.aliases.NamedCalled", {
    run: function () {
    },
    interfaces: getInterfaces()
});

var mixin = { hashCode: function () { return 1; } };
var SpreadExtend = java.lang.Object.extend({
    ...mixin,
    toString: function () {
    }
});

var memberName = "equals";
var ComputedExtend = java.lang.Object.extend({
    [memberName]: function () {
    },
    finalize: function () {
    }
});

var ComputedMember = (function (_super) {
    __extends(ComputedMember, _super);
    function ComputedMember() {
        var _this = _super.call(this) || this;
        return global.__native(_this);
    }
    ComputedMember.prototype.update = function () {
    };
    ComputedMember.prototype[memberName] = function () {
    };
    return ComputedMember;
}(java.lang.Object));

const { Closeable } = java.io;
const { Observer: ObserverMember } = java.util;
const { Formattable: DefaultedFormattable = null } = java.util;

var DestructuredObject = (function (_super) {
    __extends(DestructuredObject, _super);
    function DestructuredObject() {
        var _this = _super.call(this) || this;
        return global.__native(_this);
    }
    DestructuredObject.prototype.close = function () {
    };
    DestructuredObject = __decorate([
        Interfaces([Closeable, ObserverMember])
    ], DestructuredObject);
    return DestructuredObject;
}(java.lang.Object));

var DefaultedDestructuredObject = (function (_super) {
    __extends(DefaultedDestructuredObject, _super);
    function DefaultedDestructuredObject() {
        var _this = _super.call(this) || this;
        return global.__native(_this);
    }
    DefaultedDestructuredObject.prototype.formatTo = function () {
    };
    DefaultedDestructuredObject = __decorate([
        Interfaces([DefaultedFormattable])
    ], DefaultedDestructuredObject);
    return DefaultedDestructuredObject;
}(java.lang.Object));
