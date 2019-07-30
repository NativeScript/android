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

//	it("When_implementing_interface_and_its_implemented_interfaces", function() {
//    	var impl = new com.tns.tests.MyDerivedPublicInterface({
//    		methodOfMyDerivedPublicInterface: function(input) {
//  			    return "Le java " + input;
//    		},
//    		methodOfMyPublicInterface: function(input) {
//    		    return "Le java " + input;
//    		}
//    	});
//
//    	try {
//    		var C = java.lang.Class.forName("com.tns.gen.com.tns.tests.MyDerivedPublicInterface");
//    		expect(C).not.toBe(null);
//
//    		var expected = "Le java test derived method";
//    		var actual = impl.methodOfMyPublicInterface("test derived method");
//    		expect(actual).toBe(expected);
//    	} catch (e) {
//    		//fail("class was not found");
//    		expect(true).toBe(false);
//    	}
//    });

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

    	    var MyTSTranspiledObjectThatExtendsInterfaces = (function (_super) {
                __extends(MyTSTranspiledObjectThatExtendsInterfaces, _super);
                function MyTSTranspiledObjectThatExtendsInterfaces() {
                    return _super.apply(this, arguments) || this;
                }
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.greet = function (greeting) {
                    return "Hello, " + greeting;
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.unpack = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.properties = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.toString = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.formatTo = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.update = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.addPropertyChangeListener = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.pack = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces.prototype.removePropertyChangeListener = function () {
                };
                MyTSTranspiledObjectThatExtendsInterfaces = __decorate([
                    Interfaces([java.util.jar.Pack200.Unpacker, java.util.Formattable, java.util.Observer, java.util.jar.Pack200.Packer])
                ], MyTSTranspiledObjectThatExtendsInterfaces);
                return MyTSTranspiledObjectThatExtendsInterfaces;
            }(java.lang.Object));

            var myObject = new MyTSTranspiledObjectThatExtendsInterfaces();

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
});