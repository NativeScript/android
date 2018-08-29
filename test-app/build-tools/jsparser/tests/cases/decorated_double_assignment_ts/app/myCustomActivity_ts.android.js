"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var TestActivity = (function (_super) {
    __extends(TestActivity, _super);
    function TestActivity() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    TestActivity_1 = TestActivity;
    TestActivity.prototype.onCreate = function (savedInstanceState) {
        _super.prototype.onCreate.call(this, savedInstanceState);
        console.log(TestActivity_1.TEST1);
    };
    var TestActivity_1;
    TestActivity.TEST1 = "my_test";
    TestActivity = TestActivity_1 = __decorate([
        JavaProxy('org.nativescript.MyCustomActivity')
    ], TestActivity);
    return TestActivity;
}(android.app.Activity));
exports.TestActivity = TestActivity;