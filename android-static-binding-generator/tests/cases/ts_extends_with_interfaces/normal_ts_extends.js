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
var MyObject = (function (_super) {
    __extends(MyObject, _super);
    function MyObject(message) {
        _super.call(this);
        this.greeting = message;
    }
    MyObject.prototype.greet = function () {
        return "Hello, " + this.greeting;
    };
    // interfaces method implementations
    MyObject.prototype.unpack = function (fileIn, out) {
        console.log("It's the unpack thingie!");
    };
    MyObject.prototype.properties = function () {
    };
    MyObject.prototype.toString = function () {
        this.unpack(null, null);
        return "MyObject";
    };
    MyObject.prototype.formatTo = function () {
    };
    MyObject.prototype.update = function () {
    };
    MyObject.prototype.addPropertyChangeListener = function () {
    };
    MyObject.prototype.pack = function () {
    };
    MyObject.prototype.removePropertyChangeListener = function () {
    };
    MyObject = __decorate([
        Interfaces([java.util.jar.Pack200.Unpacker, java.util.Formattable, java.util.Observer, java.util.jar.Pack200.Packer])
    ], MyObject);
    return MyObject;
}(java.lang.Object));
var myObj = new MyObject("world");
