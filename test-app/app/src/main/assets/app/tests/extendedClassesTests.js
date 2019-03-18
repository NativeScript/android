describe("Tests extended classes ", function () {

	it("Instance with no extension shouldn't use previously defined implementation object", function () {
		var MyButton = com.tns.tests.Button1.extend({
			toString: function () {
				return "overriden toString method of chronometer instance";
			},
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button";
			}
		});
		var button = new MyButton();
		var labelToString = button.toString();
		var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
		//

		var button1 = new com.tns.tests.Button1();
		var labelToString1 = button1.toString();
		var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();

		expect(labelToString).not.toBe(labelToString1);
		expect(labelgetIMAGE_ID_PROP).not.toBe(labelgetIMAGE_ID_PROP1);
	});

	it("Having a class with static method named 'extend' and extending it in a child class shouldn't crash the app", function (){

        /*  JS below the comment is generated from the following TS code

            class Base{
                static extend(){
                    return "expectedValue";
                }
            }
            class Child extends Base{
            }

            class SecondChild extends Child{
            }


            const superProto = Object.getPrototypeOf(Child.prototype)
            const Super = superProto.constructor;
            //console.log(Super.extend());

            var child = Object.create(Child);
            //console.log(child.extend());

            //console.log(Child.extend());

        */

        var Base = /** @class */ (function () {
            function Base() {
            }
            Base.extend = function () {
                return "expectedValue";
            };
            return Base;
        }());
        var Child = /** @class */ (function (_super) {
            __extends(Child, _super);
            function Child() {
                return _super !== null && _super.apply(this, arguments) || this;
            }
            return Child;
        }(Base));
        var SecondChild = /** @class */ (function (_super) {
            __extends(SecondChild, _super);
            function SecondChild() {
                return _super !== null && _super.apply(this, arguments) || this;
            }
            return SecondChild;
        }(Child));

        var superProto = Object.getPrototypeOf(Child.prototype);
        var Super = superProto.constructor;
        expect(Super.extend()).toBe("expectedValue");

        var child = Object.create(Child);
        expect(child.extend()).toBe("expectedValue");

        expect(Child.extend()).toBe("expectedValue");
	});

	it("Instance with extension shouldn't use previously defined implementation object", function () {

		var MyButton = com.tns.tests.Button1.extend({
			toString: function () {
				return "overriden toString method of button instance";
			},
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button";
			}
		});
		var button = new MyButton();
		var labelToString = button.toString();
		var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
		//

		var MyButton1 = com.tns.tests.Button1.extend({
			toString: function () {
				return "overriden toString method of button1 instance ";
			},
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button1";
			}
		});
		var button1 = new MyButton1();
		var labelToString1 = button1.toString();
		var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();

		expect(labelToString).not.toBe(labelToString1);
		expect(labelgetIMAGE_ID_PROP).not.toBe(labelgetIMAGE_ID_PROP1);
	});

	it("Newly created instances should behave the same and not use previously defined implementation objects", function () {

		var button1 = new com.tns.tests.Button1();
		var labelgetIMAGE_ID_PROP1 = button1.getIMAGE_ID_PROP();

		//
		var MyButton = com.tns.tests.Button1.extend({
			getIMAGE_ID_PROP: function () {
				return "overriden getIMAGE_ID_PROP method on button";
			}
		});
		var button = new MyButton();
		var labelgetIMAGE_ID_PROP = button.getIMAGE_ID_PROP();
		//

		var button2 = new com.tns.tests.Button1();
		var labelgetIMAGE_ID_PROP2 = button2.getIMAGE_ID_PROP();

		expect(labelgetIMAGE_ID_PROP1).toBe(labelgetIMAGE_ID_PROP2);
	});

	it("Should not crash with no exception when incorrectly calling extended class constructor", function () {
	    let MyObj = java.lang.Object.extend({
                        toString: () => { return "It's MyObj" }
                    });

        expect(() => { myObj() }).toThrow();
	});
});