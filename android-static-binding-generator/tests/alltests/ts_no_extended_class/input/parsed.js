var MyClass = (function () {
    function MyClass() {
    }
    MyClass.prototype.MyClassMethod1 = function () {
        console.log("smth");
    };
    MyClass.prototype.MyClassMethod2 = function () {
        console.log("smth");
    };
    MyClass = __decorate([
        JavaProxy("asd.asd.As")
    ], MyClass);
    return MyClass;
})();