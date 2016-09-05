var MyClass = (function (_super) {
    __extends(MyClass, _super);
    function MyClass() {
        _super.apply(this, arguments);
    }
    MyClass.prototype.MyClassMethod1 = function () {
        console.log("smth");
    };
    MyClass.prototype.MyClassMethod2 = function () {
        console.log("smth");
    };
    MyClass = __decorate([
        JavaProxy("asd/asd.As")
    ], MyClass);
    return MyClass;
})(android.view.ViewGroup);