describe("Tests for runtime binding generator", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("When_generating_a_proxy_of_nested_interface_at_runtime", function () {

		__log("TEST: When_generating_a_proxy_of_nested_interface_at_runtime");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton10", {
			toString : function() {
			  	return "button1";	
			},
		});
		
		var button = new MyButton();
		
		var called = false;
		button.setOnClickListener(new android.view.View.OnClickListener("ClickListener19", {
			onClick:  function() {
		          called = true;
		    }
		}));
		
	    button.click(null);
		
		expect(called).toBe(true);
	});
	
	it("When_generating_a_proxy_of_android_class_at_runtime", function () {
		
		__log("TEST: When_generating_a_proxy_of_android_class_at_runtime");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton36", {
			toString : function() {
			  	return "button1";	
			},
		});
		
		var button1 = new MyButton();
		var dummyObject = button1.DummyClassAsObjectField;
		
		var isInstanceOf = dummyObject instanceof com.tns.tests.DummyClass;
		expect(isInstanceOf).toEqual(true);
	});

	it("When_implementing_interface_and_its_implemented_interfaces", function() {
    	var impl = new com.tns.tests.MyDerivedPublicInterface({
    		methodOfMyDerivedPublicInterface: function(input) {
  			    return "Le java " + input;
    		},
    		methodOfMyPublicInterface: function(input) {
    		    return "Le java " + input;
    		}
    	});

    	try {
    		var C = java.lang.Class.forName("com.tns.gen.com.tns.tests.MyDerivedPublicInterface");
    		expect(C).not.toBe(null);

    		var expected = "Le java test derived method";
    		var actual = impl.methodOfMyPublicInterface("test derived method");
    		expect(actual).toBe(expected);
    	} catch (e) {
    		//fail("class was not found");
    		expect(true).toBe(false);
    	}
    });

	it("When_generating_a_class_that_implements_interfaces_javascript", function() {

	    __log("TEST: When_generating_a_class_that_implements_interfaces_javascript");

	    var MyNormalObject = java.lang.Object.extend({
            greet: function(greetings) {
                return "Hello, " + greetings;
            },
            unpack: function() {

            },
            properties: function() {

            },
            toString: function() {

            },
            formatTo: function() {

            },
            update: function() {

            },
            addPropertyChangeListener: function() {

            },
            pack: function() {

            },
            removePropertyChangeListener: function() {

            },
            interfaces: [java.util.jar.Pack200.Unpacker, java.util.Formattable, java.util.Observer, java.util.jar.Pack200.Packer]
        })

        var myNormalObject = new MyNormalObject();

        var isInstanceOf = myNormalObject instanceof java.lang.Object;

        expect(isInstanceOf).toBe(true);

        var clazz = myNormalObject.getClass();

        var interfaces = clazz.getInterfaces();

        var expectedInterfaces = ["java.util.jar.Pack200$Packer", "java.util.Formattable", "java.util.Observer", "java.util.jar.Pack200$Unpacker", "com.tns.NativeScriptHashCodeProvider"];

        expect(interfaces.length).toBe(5);

        for(var i = 0; i < interfaces.length; i++) {
            var interfaceName = interfaces[i].getName().toString();

            __log(interfaceName);

            if(interfaceName.length > 0) {
                expect(expectedInterfaces.indexOf(interfaceName)).not.toBe(-1);
            }
        }
	});

	it("When_generating_a_class_that_implements_interfaces_typescript", function() {

    	    __log("TEST: When_generating_a_class_that_implements_interfaces_typescript");

    	    var MyObject = (function (_super) {
                __extends(MyObject, _super);
                function MyObject() {
                    _super.apply(this, arguments);
                    return global.__native(this);
                }
                MyObject.prototype.greet = function (greeting) {
                    return "Hello, " + greeting;
                };
                MyObject.prototype.unpack = function () {
                };
                MyObject.prototype.properties = function () {
                };
                MyObject.prototype.toString = function () {
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

            var myObject = new MyObject();

            var isInstanceOf = myObject instanceof java.lang.Object;

            expect(isInstanceOf).toBe(true);

            var clazz = myObject.getClass();

            var interfaces = clazz.getInterfaces();

            var expectedInterfaces = ["java.util.jar.Pack200$Packer", "java.util.Formattable", "java.util.Observer", "java.util.jar.Pack200$Unpacker", "com.tns.NativeScriptHashCodeProvider"];

            expect(interfaces.length).toBe(5);

            for(var i = 0; i < interfaces.length; i++) {
                var interfaceName = interfaces[i].getName().toString();

                __log(interfaceName);

                if(interfaceName.length > 0) {
                    expect(expectedInterfaces.indexOf(interfaceName)).not.toBe(-1);
                }
            }
    	});

    it("Test_correct_behavior_when_two_or_more_classes_implement_the_same_interfaces", function() {

        __log("TEST: Test_correct_behavior_when_two_or_more_classes_implement_the_same_interfaces");

        var resultFromMyObject1UnpackCall = "",
            resultFromMyObject2UnpackCall = "";

        var MyObject1 = (function (_super) {
                        __extends(MyObject1, _super);
                        function MyObject1() {
                            _super.apply(this, arguments);
                            return global.__native(this);
                        }
                        MyObject1.prototype.greet = function (greeting) {
                            return "Hello, " + greeting;
                        };
                        MyObject1.prototype.unpack = function () {
                            resultFromMyObject1UnpackCall = "Object1";
                            __log("From MyObject1");
                        };
                        MyObject1.prototype.properties = function () {
                        };
                        MyObject1.prototype.toString = function () {
                        };
                        MyObject1.prototype.formatTo = function () {
                        };
                        MyObject1.prototype.update = function () {
                        };
                        MyObject1.prototype.addPropertyChangeListener = function () {
                        };
                        MyObject1.prototype.pack = function () {
                        };
                        MyObject1.prototype.removePropertyChangeListener = function () {
                        };
                        MyObject1 = __decorate([
                            Interfaces([java.util.jar.Pack200.Unpacker, java.util.Formattable, java.util.Observer, java.util.jar.Pack200.Packer])
                        ], MyObject1);
                        return MyObject1;
                    }(java.lang.Object));

        var MyObject2 = (function (_super) {
                        __extends(MyObject2, _super);
                        function MyObject2() {
                            _super.apply(this, arguments);
                            return global.__native(this);
                        }
                        MyObject2.prototype.greet = function (greeting) {
                            return "Hello, " + greeting;
                        };
                        MyObject2.prototype.unpack = function () {
                            resultFromMyObject2UnpackCall = "Object2";
                            __log("From MyObject2");
                        };
                        MyObject2.prototype.properties = function () {
                        };
                        MyObject2.prototype.toString = function () {
                        };
                        MyObject2.prototype.formatTo = function () {
                        };
                        MyObject2.prototype.update = function () {
                        };
                        MyObject2.prototype.addPropertyChangeListener = function () {
                        };
                        MyObject2.prototype.pack = function () {
                        };
                        MyObject2.prototype.removePropertyChangeListener = function () {
                        };
                        MyObject2 = __decorate([
                            Interfaces([java.util.jar.Pack200.Unpacker, java.util.Formattable, java.util.Observer, java.util.jar.Pack200.Packer])
                        ], MyObject2);
                        return MyObject2;
                    }(java.lang.Object));


                    var myObj1 = new MyObject1();
                        myObj1.unpack();

                    var myObj2 = new MyObject2();
                        myObj2.unpack();

                    expect(resultFromMyObject1UnpackCall).toBe("Object1");
                    expect(resultFromMyObject2UnpackCall).toBe("Object2");
    });
});