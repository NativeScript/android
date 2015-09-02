describe("Tests ", function () {

	var objectToString = function(o){
	    var str='';

	    for(var p in o){
	        if(typeof o[p] == 'string'){
	            str+= p + ': ' + o[p]+';';
	        }else{
	            str+= p + ': { ' + objectToString(o[p]) + ' } ';
	        }
	    }

	    return str;
	};
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("When_extending_a_class_two_times", function () {
		
		__log("TEST: When_extending_a_class_two_times");
		
		__log("TEST: Creating MyButton");
		var MyButton = com.tns.tests.Button1.extend("MyButton", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		__log("TEST: Calling MyButton ctor");
		var button1 = new MyButton();
		__log("TEST: Calling button1 toString");
		var button1Label = button1.toString();
		button1.setLabel("first button");
		
		__log("TEST: Creating MyButton2 class");
		var MyButton2 = com.tns.tests.Button1.extend("MyButton", {
			toString : function() {
		  		return "button2";	
		}});
		
		var button2 = new MyButton2();
		button2.setLabel("second button");
		var button2Label = button2.toString();
		
		__log("but1=" + button1Label + ", but2=" + button2Label);
		
		var shouldBeTrue = (button1 != button2 && button1Label == "button1" && button2Label == "button2");
		
		expect(shouldBeTrue).toBe(true);
		
		var button1LabelAfterButton2Created = button1.toString();
		shouldBeTrue = (button1 != button2 && button1LabelAfterButton2Created == "button1" && button2Label == "button2");
		
		expect(shouldBeTrue).toBe(true);
	});
	
	it("When_extending_a_class_two_times_with_no_extend_names", function () {
		
		__log("TEST: When_extending_a_class_two_times_with_no_extend_names");
		
		__log("TEST: Creating MyButton");
		var MyButton = com.tns.tests.Button1.extend({
			toString : function() {
			  	return "button1";	
			}
		});
		
		__log("TEST: Calling MyButton ctor");
		var button1 = new MyButton();
		__log("TEST: Calling button1 toString");
		var button1Label = button1.toString();
		button1.setLabel("first button");
		
		__log("TEST: Creating MyButton2 class");
		var MyButton2 = com.tns.tests.Button1.extend({
			toString : function() {
		  		return "button2";	
		}});
		
		var button2 = new MyButton2();
		button2.setLabel("second button");
		var button2Label = button2.toString();
		
		__log("but1=" + button1Label + ", but2=" + button2Label);
		
		var shouldBeTrue = (button1 != button2 && button1Label == "button1" && button2Label == "button2");
		
		expect(shouldBeTrue).toBe(true);
		
		var button1LabelAfterButton2Created = button1.toString();
		shouldBeTrue = (button1 != button2 && button1LabelAfterButton2Created == "button1" && button2Label == "button2");
		
		expect(shouldBeTrue).toBe(true);
	});
	
	
	it("When_extending_a_class_with_method_overloads_by_argument_type", function () {
		
		__log("TEST: Creating MyButton");
		var MyButton = com.tns.tests.Button1.extend({
			method2 : function(arg1) {
			  	return arg1.toString();	
			}
		});
		
		var button1 = new MyButton();
		
		var callWithInt = button1.getClass().getMethod("callMethod2WithInt", []);
		var callWithByte = button1.getClass().getMethod("callMethod2WithByte", []);
		
		
		
		__log("TEST: Calling MyButton method2 with int");
		var intResult = callWithInt.invoke(button1, []);
		expect(intResult).toBe("1");
		//var intResult = button1.callMethod2WithInt();
		__log("TEST: intResult = " + intResult);
		
		__log("TEST: Calling MyButton method2 with byte");
		//var byteResult = button1.callMethod2WithByte();
		var byteResult = callWithByte.invoke(button1, []);
		__log("TEST: byteResult = " + byteResult);
		
		expect(byteResult).toBe("5");
	});
	
	
	it("When_implementing_an_interface_with_new_the_overrides_should_work", function () {
		
		__log("TEST: When_implementing_an_interface_with_new__the_overrides_should_work");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton60", {
			toString : function() {
			  	return "button1";	
			} 
		});
		
		var button1 = new MyButton();
		var buttonClicked = false;
		button1.setOnClickListener(new android.view.View.OnClickListener("MyClickListener", {
			onClick : function() {
				buttonClicked = true;
			}
		}));
		button1.click(null);
		
		expect(buttonClicked).toEqual(true);
	});
	
	it("When_calling_instanceof_on_field_result_it_should_work", function () {
		
		__log("TEST: When_calling_instanceof_on_field_result_it_should_work");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton81", {
			toString : function() {
			  	return "button1";	
			},
		});
		
		var button1 = new MyButton();
		var dummyObject = button1.DummyClassAsObjectField;
		
		var isInstanceOf = dummyObject instanceof com.tns.tests.DummyClass;
		
		expect(isInstanceOf).toEqual(true);
	});
	
	it("When_calling_instanceof_on_method_result_it_should_work", function () {
		
		__log("TEST: When_calling_instanceof_on_method_result_it_should_work");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton98", {
			toString : function() {
			  	return "button1";	
			},
		});
		
		var button1 = new MyButton();
		var dummy = button1.getDummy();
		
		var isInstanceOf = dummy instanceof com.tns.tests.DummyClass;
		
		expect(isInstanceOf).toEqual(true);
	});
	
	it("When_calling_instanceof_on_method_argument_it_should_work", function () {
		
		__log("TEST: When_calling_instanceof_on_method_argument_it_should_work");
		
		var isInstanceOf;
		
		var MyButton = com.tns.tests.Button1.extend("MyButton115", {
			toString : function() {
			  	return "button1";	
			},

			methodDummyClassAsObjectInArgs: function(object) {
				isInstanceOf = object instanceof com.tns.tests.DummyClass;
			}
		});
		
		var button1 = new MyButton();
		button1.callMethodDummyClassAsObjectInArgs();
		
		expect(isInstanceOf).toEqual(true);
	});
	
	//originally wasn't run 
	it("When_calling_instanceof_on_interface_it_should_work", function () {
		
		__log("TEST: When_calling_instanceof_on_interface_it_should_work");
		
		var interfaceInstance = new android.view.View.OnClickListener("ClickListener", {
			onClick : function() {
				buttonClicked = true;
			}
		});
		
		var secondInterfaceInstance = new android.view.View.OnClickListener("ClickListener", {
			onClick : function() {
				buttonClicked = true;
			}
		});
		
		var thirdInterfaceInstance = new android.view.View.OnClickListener("ClickListener", {
			onClick : function() {
				buttonClicked = true;
			}
		});
		
		//__log("Object get PrototypeOf" + Object.getPrototypeOf(interfaceInstance).toString());
		//__log("Object get PrototypeOf" + Object.getPrototypeOf(secondInterfaceInstance).toString());
		
		var isInstanceOfOnClickListener = interfaceInstance instanceof android.view.View.OnClickListener;
		var secondIsInstanceOfOnClickListener = secondInterfaceInstance instanceof android.view.View.OnClickListener;
		var thirdIsInstanceOfOnClickListener = thirdInterfaceInstance instanceof android.view.View.OnClickListener;
		
		expect(isInstanceOfOnClickListener).toEqual(true);
		expect(secondIsInstanceOfOnClickListener).toEqual(true);
		expect(thirdIsInstanceOfOnClickListener).toEqual(true);
	});
	
	it("When_calling_instanceof_it_should_work", function () {
		
		__log("TEST: When_calling_instanceof_it_should_work");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton148", {
			toString : function() {
			  	return "button1";	
			}
		});

		var button1 = new MyButton();
		
		var isInstanceOfMyButton = button1 instanceof MyButton;
		var isInstanceOfButton1 = button1 instanceof com.tns.tests.Button1;
		
		expect(isInstanceOfMyButton).toEqual(true);
		expect(isInstanceOfButton1).toBe(true);
	});
	
	it("When_calling_instance_and_static_member_with_same_name_the_calls_should_succeed", function () {
		
		__log("TEST: When_calling_instance_and_static_member_with_same_name_the_calls_should_succeed");
		var MyButton = com.tns.tests.Button1.extend("MyButton213", {
			toString : function() {
			  	return "button1";	
			}
		});

		var button1 = new MyButton();

		var exceptionCaught = false;
		try {
			MyButton.someMethod(1, "hello");
			button1.someMethod(1, "hello");
			button1.someMethod(1, new java.lang.Object());
		}
		catch(e) {
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(false);
	});
	
	it("When_calling_toString_on_an_java_object_it_should_call_the_java_method", function () {
		
		__log("TEST: When_calling_toString_on_an_java_object_it_should_call_the_java_method");
		var instance = new com.tns.tests.DummyClass();
		var s = instance.toString();
		
		expect(s.indexOf("com.tns.tests.DummyClass")).not.toEqual(-1);
	});
	
	it("When_calling_toString_on_an_java_object_that_has_overriden_toString_in_js_it_should_call_the_js_method", function () {
		
		__log("TEST: When_calling_toString_on_an_java_object_that_has_overriden_toString_in_js_it_should_call_the_js_method");
		var MyButton = com.tns.tests.Button1.extend("MyButton240", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var instance = new MyButton();
		var s = instance.toString();
		
		expect(s).toBe("button1");
	});
	
	it("When_extending_a_class_two_times_without_second_implementation_object", function () {
		
		__log("TEST: When_extending_a_class_two_times_without_second_implementation_object");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton257", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var button1 = new MyButton();
		var button1Label = button1.toString();
		
		var button2 = new com.tns.tests.Button1();
		var button2Label = button2.toString();
		
		__log("button1Label=" + button1Label + ", button2Label=" + button2Label);
		var shouldBeTrue = (button1 !== button2 && button1Label !== button2Label);
		
		expect(shouldBeTrue).toBe(true);
		
		var button1PostButton2CreationLabel = button1.toString();
		
		expect(button1Label).toBe(button1PostButton2CreationLabel);
	});
	
	it("When__calling_super_method_using_the_prototype_property_of_a_function_it_should_call_the_super_method", function () {
		
		__log("TEST: When__calling_super_method_using_the_prototype_property_of_a_function_it_should_call_the_super_method");

		var button1 = new com.tns.tests.Button1();
		var prop = com.tns.tests.Button1.prototype.getIMAGE_ID_PROP.call(button1);
		
		expect(prop).toBe("image id prop");
	});
	
	it("When__calling_super_method_using_the_prototype_property_of_a_extended_function_it_should_call_the_super_method", function () {
		
		__log("TEST: When__calling_super_method_using_the_prototype_property_of_a_extended_function_it_should_call_the_super_method");

		var MyButton = com.tns.tests.Button1.extend("MyButton289", {});
		var button1 = new MyButton();
		var prop = com.tns.tests.Button1.prototype.getIMAGE_ID_PROP.call(button1);
		
		expect(prop).toBe("image id prop");
	});
	
	it("When__calling_super_method_using_the_prototype_property_of_a_extended_function_it_should_call_the_super_method2", function () {
		
		__log("TEST: When__calling_super_method_using_the_prototype_property_of_a_extended_function_it_should_call_the_super_method2");

		var MyButton = com.tns.tests.Button1.extend("MyButton294", {
			getIMAGE_ID_PROP: function() { return ""; }
		});
		var button1 = new MyButton();
		var prop = com.tns.tests.Button1.prototype.getIMAGE_ID_PROP.call(button1);
		
		expect(prop).toBe("image id prop");
	});
	
	it("When_extending_a_class_and_calling_super_toString", function () {
		
		//__log("//TODO: NOT WORKING: super method calls are not working correctly. Tests fails with FAILED: When_extending_a_class_and_calling_super_toString. Actual: com.tns.com.tns.tests.Button1-MyButton305@52854640 Expected: com.tns.tests.Button1@");
		//return;
		
		__log("TEST: When_extending_a_class_and_calling_super_toString");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton", {
			toString : function() {
			  	return this.super.toString() + this.super.echo("success");	
			},
			
			echo : function(s) {
		  		return "fail";
			}
		});
		
		var button1 = new MyButton();
		var button1Label = button1.toString();
		
		expect(button1Label.indexOf("com.tns.tests.Button1_")).not.toEqual(-1);
		expect(button1Label.indexOf("MyButton")).not.toEqual(-1);
		expect(button1Label.indexOf("success")).not.toEqual(-1);
		
	});
	
	it("When_extending_a_class_and_calling_super_method_it_should_work", function () {
		
		__log("TEST: When_extending_a_class_and_calling_super_method_it_should_work");
		var MyButton = com.tns.tests.Button1.extend("MyButton318", {
			toString : function() {
			  	return "toString overriden";	
			},
		
			getIMAGE_ID_PROP : function() {
				return this.super.getIMAGE_ID_PROP() + "!";
			}
		});
		var button1 = new MyButton();
		var button1SuperToString = button1.toString();
		
		expect(button1SuperToString).toBe("toString overriden");
		
		var IMAGE_ID_PROP_Result = button1.getIMAGE_ID_PROP();
		
		expect(IMAGE_ID_PROP_Result).toBe("image id prop!");
	});
	
	it("When_accessing_static_members_on_an_extended_class", function () {
		
		__log("TEST: When_accessing_static_members_on_an_extended_class");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton341", {
			hashCode : function() {
			  	return 5454;	
			}
		});
		
		var MyButton2 = com.tns.tests.Button1.extend("MyButton347", {
			hashCode : function() {
			  	return 1212;	
			}
		});
		
		var setValue = 4;
		MyButton.setMyStaticIntField(setValue);
		var readValue = MyButton2.getMyStaticIntField();
		
		expect(readValue).toEqual(setValue);
		
		var readValue = com.tns.tests.Button1.getMyStaticIntField();
		
		expect(readValue).toEqual(setValue);
	});
	
	it("When_implementing_an_interface_with_new__the_overrides_should_work", function () {
		
		__log("TEST: When_implementing_an_interface_with_new__the_overrides_should_work");
		
		var MyButton = com.tns.tests.Button1.extend({
			toString : function() {
			  	return "button1";	
			} 
		});
		
		var button1 = new MyButton();
		var buttonClicked = false;

		button1.setOnClickListener(new android.view.View.OnClickListener({
			onClick : function() {
				buttonClicked = true;
			}
		}));

		button1.click(null);

		expect(buttonClicked).toEqual(true);
	});
	
	it("When_a_java_method_returns_object_that_needs_js_instance__it_should_create_the_instance", function () {
		
		__log("TEST: When_a_java_method_returns_object_that_needs_js_instance__it_should_create_the_instance");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton381", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var button1 = new MyButton();
		var dummy = button1.getDummy();
		
		var exceptionCaught = false;
		try {
			var res = dummy.dummyMethod(123); //this will fail if button2 is not valid proxy object and properly exposed to js	
		}
		catch (e) {
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(false);
	});
	
	it("When_a_java_method_returns_object_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type", function () {

		__log("TEST: When_a_java_method_returns_object_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type");
		
		var Button = com.tns.tests.Button1.extend("MyButton397", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var button = new Button();
		var object = button.getDummyClassAsObject();
		var name = object.getName();
		
		expect(name).toEqual("dummy");
	});
	
	it("When_a_java_field_returns_object_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type", function () {
		
		__log("TEST: When_a_java_field_returns_object_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type");
		
		var Button = com.tns.tests.Button1.extend("MyButton413", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var button = new Button();
		var object = button.DummyClassAsObjectField;
		var name = object.getName();
		
		expect(name).toEqual("dummy");
	});
	
	it("When_a_java_argument_is_passed_to_js_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type", function () {
		
		__log("TEST: When_a_java_argument_is_passed_to_js_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type");
		
		var name = "";
		var Button = com.tns.tests.Button1.extend("MyButton418", {
			toString : function() {
			  	return "button1";	
			},
			
			methodDummyClassAsObjectInArgs: function(object) {
				name = object.getName();
				__log("The actual name is " + name);
			}
		});
		
		var button = new Button();
		var object = button.callMethodDummyClassAsObjectInArgs();
		
		expect(name).toEqual("dummy");
	});
	
	it("When_a_java_object_is_returned_from_indexer_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type", function () {
		
		__log("TEST: When_a_java_object_is_returned_from_indexer_that_needs_js_instance__it_should_create_the_instance_according_to_the_actual_return_type");
		
		var Button = com.tns.tests.Button1.extend("MyButton450", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var button = new Button();
		var arrayOfObjects = button.getDummyClassAsObjectArray();
		var name = arrayOfObjects[0].getName();
		
		expect(name).toEqual("dummy");
	});
	
	it("When_accessing_a_static_field_on_a_javascript_instance_it_should_work", function () {
		
		__log("TEST: When_accessing_a_static_field_on_a_javascript_instance_it_should_work");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton455", {
			hashCode : function() {
			  	return 5454;	
			},
			
			toString : function() {
			  	return "button1";	
			},
			
			equals : function() {
				return true;
			}
		});
		
		var valueUsingChild = MyButton.STATIC_IMAGE_ID;
		expect(valueUsingChild).toEqual("static image id");
		
		var valueUsingParent = com.tns.tests.Button1.STATIC_IMAGE_ID;
		
		expect(valueUsingParent).toEqual("static image id");
	});
	
	it("TestRequireDirName", function () {
		
		__log("TEST: TestRequireDirName");
		
		var dir = __dirname;
		
		var expectedDirname = "/data/data/com.tns.android_runtime_testapp/files/app/tests";
		
		expect(dir).toBe(expectedDirname);
	});
	
	it("TestRequireFileName", function () {
		
		__log("TEST: TestRequireFileName");
		
		var file = __filename;
		
		var expectedFilename = "/data/data/com.tns.android_runtime_testapp/files/app/tests/tests.js";
			
		expect(file).toBe(expectedFilename);
		
		var file2 = module.filename;

		expect(file).toBe(file2);
	});
	
	it("TestGarbageCollection", function (done) {
		var normalTest = function () { 

			__log("TEST: TestGarbageCollection");
			
			var obj = new com.tns.tests.ClassX();
			
			obj.dummy();
			
			obj = null;
			
			gc();
			java.lang.System.gc();
			gc();
			java.lang.System.gc();
			gc();
			java.lang.System.gc();
			
			new java.lang.Thread(new java.lang.Runnable("ThreadFunc", {
				run: function() {
					var isCollected = com.tns.tests.ClassX.IsCollected;
					__log('----------> isCollected: ' + isCollected);
					expect(isCollected).toBe(true);
					done();
				}
			})).start();
		};
		normalTest();
	});
	
	it("TestWorkingWithJavaArrayDoesNotMakeMemoryLeak", function () {
		

		__log("TEST: TestWorkingWithJavaArrayDoesNotMakeMemoryLeak");
		
		var size = 10 * 1024 * 1024;
		
		for (var i = 0; i < 100; i++) {
		
			var arr = java.lang.reflect.Array.newInstance(java.lang.Byte.class.getField("TYPE").get(null), size);
			
			var length = arr.length;
			
			expect(length).toEqual(size);
			
			arr[0] = 123;
			
			var el = arr[0];
			
			expect(el).toEqual(123);
		
			gc();
			java.lang.System.gc();
		}
	});
	
	it("TestConstructorOverride", function () {
		

		__log("TEST: TestConstructorOverride");
		
		var ctorCalled = false;
		var isConstructor = false;

		var MyButton = com.tns.tests.Button1.extend("MyButton574", {
			init : function() {
				ctorCalled = true;
				isConstructor = arguments[arguments.length - 1];
			},

			toString : function() {
			  	return "button1";	
			}
		});
		
		var btn = new MyButton();
		
		expect(ctorCalled).toEqual(true);
		expect(isConstructor).toEqual(true);
	});
	
	it("TestConstructorOverrideOnTypeWithInitMethod", function () {

		__log("TEST: TestConstructorOverrideOnTypeWithInitMethod");
		
		var isCalled = false;
		var isConstructor = false;

		var MyDummyClassWithInit = com.tns.tests.DummyClassWithInit.extend("MyButton591", {
			init: function() {
				isCalled = true;
				isConstructor = arguments[arguments.length - 1];
			}
		});
		
		__log("TEST: TestConstructorOverrideOnTypeWithInitMethod: calling overriden ctor");
		var dummy = new MyDummyClassWithInit();
		
		expect(isCalled).toEqual(true);
		expect(isConstructor).toEqual(true);
		
		__log("TEST: TestConstructorOverrideOnTypeWithInitMethod: calling ctor as regular method");
		isCalled = undefined;
		isConstructor = undefined;
		dummy.callInit();

		expect(isCalled).toEqual(true);
		expect(isConstructor).toEqual(false);
		
	});
	
	it("TestRequire", function () {
		
		__log("TEST: TestRequire");
		
		var exceptionCaught = false;
		try{
			var myModule = require("../simplemodule");	
		}
		catch(e) {
			exceptionCaught = true;
		}
		
		myModule.myLog("Hello world from NativeScript!");
		
		expect(exceptionCaught).toBe(false);
	});
	
	it("TestArrays", function () {
		
		__log("TEST: TestArrays");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton639", {
			toString : function() {
			  	return "button1";	
			}
		});
		var tester = new MyButton(); 
		var instances = tester.getDummyInstances();
		
		var instanceFound = false;

		for (var i = 0; i < instances.length; i++)
		{
			if (instances[i].getName() == "second");
			{
				instanceFound = true;
			}
		}
		
		expect(instanceFound).toEqual(true);
		
		instances[0] = instances[1];
		
		var instances0name = instances[0].getName();
		var instances1name = instances[1].getName();
		
		expect(instances0name).toEqual(instances1name);
	});
	
	it("TestArrayLengthPropertyIsNumber", function () {
		
		__log("TEST: TestArrayLengthPropertyIsNumber");
		
		var expectedLength = 10;

		function getLength(x)
		{
			var arr = x.getIntArray1(expectedLength);
			
			return arr ? arr.length : 123456;
		}
		
		var MyButton = com.tns.tests.Button1.extend("MyButton680", {
			toString : function() {
			  	return "button1";	
			}
		});
		
		var count = getLength(new MyButton());
		
		expect(count).toBe(expectedLength);
		
	});
	
	it("TestCreationOfLocationListener", function () {
		
		__log("TEST: TestCreationOfLocationListener");
		
		var onLocationChangedCalled = false;
		var onProviderDisabledCalled = false;
		var onProviderEnabledCalled = false;
		
		var listener = new android.location.LocationListener("LocationListener",{
			onLocationChanged: function(location) {
				onLocationChangedCalled = true;
			},
			onProviderDisabled: function(provider) {
				onProviderDisabledCalled = true;
			},
			onProviderEnabled: function(provider) {
				onProviderEnabledCalled = true;
			}
		});
		
		listener.onLocationChanged(null);
		
		expect(onLocationChangedCalled).toEqual(true);
		
		listener.onProviderDisabled("");
		
		expect(onProviderDisabledCalled).toEqual(true);
		
		listener.onProviderEnabled("");
		
		expect(onProviderEnabledCalled).toEqual(true);
	});
	
	it("TestInnerClassCreation", function () {
		
		__log("TEST: TestInnerClassCreation");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton726", {
			toString : function() {
		  		return "button1"	
		}});
		
		var button1 = new MyButton();
		
		var innerButton = new button1.InnerButton();
		
		var s = innerButton.getSomeString();
		
		expect(s.length).toBeGreaterThan(0);
		 
		var innerButton2 = new new button1.InnerButton().InnerClass2(123)
		
		var s1 = innerButton2.getSomeString2();
		
		expect(s1.length).toBeGreaterThan(0);
	});
	
	it("TestNestedClassCreation", function () {
		
		__log("TEST: TestNestedClassCreation");

		var i = 123;
		
		var nested = new com.tns.tests.Button1.InnerStaticClass(i);
		
		var actual_i = nested.getInt();
		
		expect(actual_i).toEqual(i);
	});
	
	it("TestCallMethodOnAnObjectReturnedAsObjectWithoutMetadata", function () {
		
		__log("TEST: TestCallMethodOnAnObjectReturnedAsObjectWithoutMetadata");
		
		var dummy = new com.tns.tests.DummyClass();
		
		var dummy2 = dummy.getDummyClassAsObject();
		
		var name = dummy2.getName();
		
		expect(name).toEqual("dummy");
	});
	
	it("TestGetFieldOnAnObjectReturnedAsObjectWithoutMetadata", function () {
		
		__log("TEST: TestGetFieldOnAnObjectReturnedAsObjectWithoutMetadata");
		
		var dummy = new com.tns.tests.DummyClass();
		
		dummy.setDummyField();
		
		var dummy2 = dummy.dummyField;
		
		var name = dummy2.getName();
		
		expect(name).toEqual("dummy");
	});
	
	it("TestSetFloatInstanceField", function () {
		
		__log("TEST: TestSetFloatInstanceField");
		
		var lParams = new android.widget.LinearLayout.LayoutParams(android.widget.LinearLayout.LayoutParams.MATCH_PARENT, android.widget.LinearLayout.LayoutParams.MATCH_PARENT);
		lParams.weight = 1;
		
		var value = lParams.weight;
		
		expect(value).toEqual(1);
	});
	
	it("TestCallMethodOnAnObjectPassedAsParameterInOverriddenMethodAsAnObjectWithoutMetadata", function () {
		
		__log("TEST: TestCallMethodOnAnObjectPassedAsParameterInOverriddenMethodAsAnObjectWithoutMetadata");
		
		var D = com.tns.tests.DummyClass.DummyDerivedClass.extend("D",{
			dummyMethod: function(dummy) {
				return this.getName();
			}
		})
		
		var d = new D();

		var name = d.executeCallback();
		
		expect(name).toEqual("dummy");
	});
	
	it("TestAccessArrayElementAsObjectWithoutMetadata", function () {
		
		__log("TEST: TestAccessArrayElementAsObjectWithoutMetadata");
		
		var d = new com.tns.tests.DummyClass();
		
		var arr = d.getDummyClassArrayAsObject();
		
		var arrLength = arr.length;
		
		expect(arrLength).toEqual(1);
		
		var dummy = arr[0];
		
		var name = dummy.getName();
		
		expect(name).toBe("dummy");
	});
	
	it("TestCallMethodThatReturnsNull", function () {
		
		__log("TEST: TestCallMethodThatReturnsNull");

		var dummy = new com.tns.tests.DummyClass();
		
		var x = dummy.getNull();
		
		expect(x).toEqual(null);
	});
	
	it("TestCallMethodThatReturnsNullString", function () {

		__log("TEST: TestCallMethodThatReturnsNullString");

		var dummy = new com.tns.tests.DummyClass();
		
		var x = dummy.getNullString();
		
		expect(x).toEqual(null);
	});
	
	it("TestAccessNullField", function () {

		__log("TEST: TestAccessNullField");

		var dummy = new com.tns.tests.DummyClass();
		
		var x = dummy.nullField
		
		expect(x).toEqual(null);
	});
	
	it("TestAccessNullArrayElement", function () {

		__log("TEST: TestAccessNullArrayElement");

		var dummy = new com.tns.tests.DummyClass();
		
		var arr = dummy.getArrayWithNullElement();
		
		__log("arr=" + arr.length)
		
		var x = arr[0];
		
		expect(x).toEqual(null);
	});
	
	it("TEMPLATE", function () {

		__log("TEST: TestCallMethodWithIntVarArg");
		
		var dummy = new com.tns.tests.DummyClass();
		
		var s = dummy.concatIntArrayAsString([1, 2, 3, 4]);
		
		expect(s).toBe("1234");
	});
	
	it("TestCallMethodWithCharVarArg", function () {
		
		__log("TEST: TestCallMethodWithCharVarArg");
		
		var dummy = new com.tns.tests.DummyClass();
		
		var s = dummy.concatCharArrayAsString(['t', 'e', 's', 't']);
		
		expect(s).toBe("test");
	});
	
	it("TestCallMethodWithObjectVarArg", function () {
		
		__log("TEST: TestCallMethodWithObjectVarArg");
		
		var dummy = new com.tns.tests.DummyClass();
		
		var s = dummy.concatObjectArrayAsString([1, "test", false]);

		expect(s).toBe("1, test, false");
	});
	
	it("TestCanInheritFromClassInAndroidSupportLibrary", function () {
		
		__log("TEST: TestCanInheritFromClassInAndroidSupportLibrary");
		
		var MyParcelableCompat = android.support.v4.os.ParcelableCompat.extend("MyParcelableCompat", {
			toString: function() {
				return "MyParcelableCompat";
			}
		});
		
		var compat = new MyParcelableCompat();
		
		var s = compat.toString();
		
		expect(s).toBe("MyParcelableCompat");
	});
	
	it("TestCallMethodWithByteParameter", function () {
		
		__log("TEST: TestCallMethodWithByteParameter");
		
		var b = java.lang.Byte.valueOf(byte(123));
		
		var s = "" + b;
		
		expect(s).toBe("123");
	});
	
	it("TestCallMethodWithFloatParameter", function () {
		
		__log("TEST: TestCallMethodWithFloatParameter");
		
		var d = new com.tns.tests.DummyClass();
		
		var s = d.methodWithoutOverloads(1.23);
		
		expect(s).toBe("float=1.23");
	});
	
	it("TestCanCallStaticMethodThroughBaseClass", function () {
		
		__log("TEST: TestCanCallStaticMethodThroughBaseClass");
		
		var name = com.tns.tests.MyClassDerived.getName();

		expect(name).toBe("com.tns.tests.MyClassBase");
	});
	
	it("TestUseFieldThatIsArray", function () {
		
		__log("TEST: TestUseFieldThatIsArray");
		
		var d = new com.tns.tests.DummyClass();
		
		var arrInt = d.arrIntField;
		
		var arrIntLength = arrInt.length;
		
		expect(arrIntLength).toBe(5);
		
		var intElement = arrInt[2];
		
		expect(intElement).toBe(33);
		
		var arrString = d.arrStringField;
		
		var arrStringLength = arrString.length;
		
		expect(arrIntLength).toBe(5);
		
		var stringElement = arrString[2];
		
		expect(stringElement).toBe("cc");
	});
	
	it("TestCanAssignArrayToField", function () {
		
		__log("TEST: TestCanAssignArrayToField");
		
		var d = new com.tns.tests.DummyClass();
		
		var arr = d.arrIntField2;
		
		expect(arr).toBe(null);
		
		d.arrIntField2 = d.arrIntField;
		
		var arrLength = d.arrIntField2.length;
		
		expect(arrLength).toBe(5);
	});
	
	it("TestCallMethodThatReturnsLong", function () {
		
		__log("TEST: TestCallMethodThatReturnsLong");
		
		var n = java.lang.Long.parseLong("9007199254740991"); // 9007199254740991 = 2^53-1

		expect(n.__proto__.valueOf()).toBe(0);
		expect(n.value).toBe(undefined);
		expect(typeof n).toBe("number");
		expect(n instanceof Number).toBe(false);

		var n = java.lang.Long.parseLong("9007199254740992"); // 9007199254740992 = 2^53
		
		var ctorFuncName = n.__proto__.constructor.name;
		expect(ctorFuncName).toBe("NativeScriptLongNumber");
		expect(isNaN(n.valueOf())).toBe(true);
		
		var javaValue = n.value;
		expect(javaValue).toBe("9007199254740992");
		
		var typeName = typeof n; 
		expect(typeName).toBe("object");
	});
	
	it("TestCallMethodWithLongParameter", function () {

		__log("TEST: TestCallMethodWithLongParameter");
		
		var d = new com.tns.tests.DummyClass();
		
		var n1 = java.lang.Long.parseLong("9007199254740991"); // 9007199254740991 = 2^53-1
		var s1 = d.getLongAsString(n1);
		expect(s1).toBe("9007199254740991");
		
		var n2 = java.lang.Long.parseLong("9007199254740992"); // 9007199254740992 = 2^53
		var s2 = d.getLongAsString(n2);
		expect(s2).toBe("9007199254740992");
		
		var n3 = java.lang.Long.parseLong("9007199254740993"); // 9007199254740992 = 2^53+1
		var s3 = d.getLongAsString(n3);
		expect(s3).toBe("9007199254740993");
	});
	
	it("TestCallMethodWithLongCastArgument", function () {

		__log("TEST: TestCallMethodWithLongCastArgument");
		
		var d = new com.tns.tests.DummyClass();
		
		var s1 = d.getLongAsString(long("9007199254740991")); // 9007199254740991 = 2^53-1
		expect(s1).toBe("9007199254740991");
		
		var s2 = d.getLongAsString(long(9007199254740991)); // 9007199254740991 = 2^53-1
		expect(s2).toBe("9007199254740991");
		
		var s3 = d.getLongAsString(long("9007199254740992")); // 9007199254740992 = 2^53
		expect(s3).toBe("9007199254740992");
		
		var s4 = d.getLongAsString(long("9007199254740993")); // 9007199254740992 = 2^53+1
		expect(s4).toBe("9007199254740993");
	});
	
	it("TestCallToStringOfNativeScriptLongObject", function () {
		
		__log("TEST: TestCallToStringOfNativeScriptLongObject");
		
		var n = java.lang.Long.parseLong("9007199254740992"); // 9007199254740992 = 2^53
		
		var s = n.toString();
		
		expect(s).toBe(n.value);
	});
	
	it("TestCallMethodWithLongParameterWithNumberObject", function () {
		
		__log("TEST: TestCallMethodWithLongParameterWithNumberObject");
		
		var d = new com.tns.tests.DummyClass();
		
		var s = d.getLongAsString(new Number("9007199254740991")); // 9007199254740991 = 2^53-1
		expect(s).toBe("9007199254740991");
	});
	
	it("TestCallMethodWithMinAndMaxLongValues", function () {
		
		__log("TEST: TestCallMethodWithMinAndMaxLongValues");
		
		var d = new com.tns.tests.DummyClass();
		
		var maxLong = d.getMaxLong();
		var sMax = d.getLongAsString(maxLong);
		expect(sMax).toBe("9223372036854775807");
		
		var minLong = d.getMinLong();
		var sMin = d.getLongAsString(minLong);
		expect(sMin).toBe("-9223372036854775808");
	});
	
	it("TestCallMethodWithByteParameter", function () {
		
		__log("TEST: TestCallMethodWithByteParameter");
		
		var d = new com.tns.tests.DummyClass();
		
		var s1 = d.method1(byte(123));
		expect(s1).toBe("byte=123");
		
		var s2 = d.method1(byte(new Number(123)));
		expect(s2).toBe("byte=123");
		
		var s3 = d.method1(byte("123"));
		expect(s3).toBe("byte=123");
		
		var s4 = d.method1(byte(new String("123")));
		expect(s4).toBe("byte=123");
	});
	
	it("TestCallMethodWithShortParameter", function () {
		
		__log("TEST: TestCallMethodWithShortParameter");
		
		var d = new com.tns.tests.DummyClass();
		
		var s1 = d.method1(short(12345));
		expect(s1).toBe("short=12345");
		
		var s2 = d.method1(short(new Number(12345)));
		expect(s2).toBe("short=12345");
		
		var s3 = d.method1(short("12345"));
		expect(s3).toBe("short=12345");
		
		var s4 = d.method1(short(new String("12345")));
		expect(s4).toBe("short=12345");
	});
	
	it("TestCallMethodWithBooleanParameter", function () {
		
		__log("TEST: TestCallMethodWithBooleanParameter");
		
		var d = new com.tns.tests.DummyClass();
		
		var s1 = d.method1(true);
		expect(s1).toBe("boolean=true");
		
		var s2 = d.method1(false);
		expect(s2).toBe("boolean=false");
		
		var s3 = d.method1(new Boolean(true));
		expect(s3).toBe("boolean=true");
		
		var s4 = d.method1(new Boolean(false));
		expect(s4).toBe("boolean=false");
	});
	
	it("TestThrowJavaScriptExceptionWhenCannotResolveJavaMethod", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenCannotResolveJavaMethod");
		
		var exceptionCaught = false;
		
		var d = new com.tns.tests.DummyClass();
		
		try
		{
			var s = d.method1(new java.lang.Object());
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenCannotResolveJavaConstructor", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenCannotResolveJavaConstructor");
		
		var exceptionCaught = false;
		
		try
		{
			var d = new com.tns.tests.DummyClass(new java.lang.Object());
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenSetArrayRefElementWithNakedJavaScriptObject", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenSetArrayRefElementWithNakedJavaScriptObject");
		
		var arr = java.lang.reflect.Array.newInstance(java.lang.Object.class, 10);
		
		var o = new java.lang.Object();
		arr[0] = o;
		
		var exceptionCaught = false;
		
		try
		{
			arr[0] = {};
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
		
		var isOldElement = o.equals(arr[0]);
		
		expect(isOldElement).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenSetArrayRefElementWithJavaScriptPrimitive", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenSetArrayRefElementWithJavaScriptPrimitive");
		
		var arr = java.lang.reflect.Array.newInstance(java.lang.Object.class, 10);
		
		var o = new java.lang.Object();
		arr[0] = o;
		
		var exceptionCaught = false;
		
		try
		{
			arr[0] = 123;
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
		
		var isOldElement = o.equals(arr[0]);
		
		expect(isOldElement).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenCreateJavaObjectWithNakedJavaScriptObject", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenCreateJavaObjectWithNakedJavaScriptObject");
		
		var exceptionCaught = false;

		try
		{
			var d = new com.tns.tests.DummyClass({});
		}
		catch (e)
		{
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenCallJavaMethodWithNakedJavaScriptObject", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenCallJavaMethodWithNakedJavaScriptObject");
		
		var exceptionCaught = false;
		
		var d = new com.tns.tests.DummyClass();

		try
		{
			var s = d.method2({});
		}
		catch (e)
		{
			exceptionCaught = true;
		}
	
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenCallJavaMethodWithJavaScriptPrimitiveWhenJavaRefIsExpected", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenCallJavaMethodWithJavaScriptPrimitiveWhenJavaRefIsExpected");
		
		var exceptionCaught = false;
		
		var d = new com.tns.tests.DummyClass();

		try
		{
			var s = d.method2(123);
		}
		catch (e)
		{
			exceptionCaught = true;
		}

		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenOverideMethodImplementationIsDeleted", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenOverideMethodImplementationIsDeleted");
		
		var exceptionCaught = false;
		
		var impl = {
			echo : function(s) {
		  		return "!!!" + s;
			}
		};
		
		var MyButton = com.tns.tests.Button1.extend("btn1303", impl);
		var btn = new MyButton();
		
		var echo = com.tns.tests.Button1.prototype.echo; 
		delete com.tns.tests.Button1.prototype.echo;
		delete impl.echo;
		
		try
		{
			__log("btn=" + btn.triggerEcho("12345"));
		}
		catch (e)
		{
			exceptionCaught = true;
		}	

		expect(exceptionCaught).toBe(true);
		
		exceptionCaught = false;	

		try
		{
			__log("btn=" + btn.triggerEchoAsObject("123"));
		}
		catch (e)
		{
			exceptionCaught = true;
		}	

		expect(exceptionCaught).toBe(true);
		
		com.tns.tests.Button1.prototype.echo = echo;
	});
	
	it("TestThrowJavaScriptExceptionWhenOverideMethodImplementationIsOverwritten", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenOverideMethodImplementationIsOverwritten");
		
		var exceptionCaught = false;
		
		var impl = {
			echo : function(s) {
		  		return "!!!" + s;
			}
		};
		
		var MyButton = com.tns.tests.Button1.extend("btn1344", impl);
		var btn = new MyButton();
		
		impl.echo = ""
		
		try
		{
			__log("btn=" + btn.triggerEcho("123"));
		}
		catch (e)
		{
			exceptionCaught = true;
		}	

		expect(exceptionCaught).toBe(true);
		
		exceptionCaught = false;	

		try
		{
			__log("btn=" + btn.triggerEchoAsObject("123"));
		}
		catch (e)
		{
			exceptionCaught = true;
		}	

		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenPartiallyImplementedInterfaceIsUsed", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenPartiallyImplementedInterfaceIsUsed");
		
		var methodCalled = false;
		var exceptionCaught = false;
		
		var d = new com.tns.tests.DummyClass();
		
		var impl1 = new com.tns.tests.DummyClass.MyInterface("impl1_1393", {
			echoInt: function(i) { methodCalled = true; return i; }
		});
		
		var i = d.triggerEchoInt(impl1, 123);
		
		expect(methodCalled).toBe(true);
		
		expect(i).toBe(123);
		
		try
		{
			d.triggerDoSomething(impl1);
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
		
		methodCalled = false;
		exceptionCaught = false;
		
		var impl2 = new com.tns.tests.DummyClass.MyInterface("impl2_1417",{
			doSomething: function() { methodCalled = true; }
		});
		
		d.triggerDoSomething(impl2);
		
		expect(methodCalled).toBe(true);
		
		try
		{
			d.triggerEchoInt(impl2, 123);
		}
		catch (e)
		{
			exceptionCaught = true;
		}
			
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenImplementationObjectIsUsedToExtendMoreThanOneClass", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenImplementationObjectIsUsedToExtendMoreThanOneClass");
		
		var implObj = {}
		
		var exceptionCaught = false;
		
		var Button1 = com.tns.tests.Button1.extend("Button1", implObj);
		
	    try
		{
			var D = com.tns.tests.DummyClass.DummyDerivedClass.extend("D1440", implObj);
		}
		catch (e)
		{
			__log("TEST: TestThrowJavaScriptExceptionWhenImplementationObjectIsUsedToExtendMoreThanOneClass exception:" + e);
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenPassBooleanArgumentWhereNotExpected", function () {
		
		__log("TEST: TestThrowJavaScriptExceptionWhenPassBooleanArgumentWhereNotExpected");
		
		var d = new com.tns.tests.DummyClass();
		
		var exceptionCaught = false;
		
		try
		{
			d.setName(false);	
		}
		catch (e)
		{
			__log("e=" + e);
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
		
		exceptionCaught = false;

		try
		{
			d.setName(true);	
		}
		catch (e)
		{
			__log("e=" + e);
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestThrowJavaScriptExceptionWhenPassNumberArgumentWhereNotExpected", function () {

		__log("TEST: TestThrowJavaScriptExceptionWhenPassNumberArgumentWhereNotExpected");
		
		var d = new com.tns.tests.DummyClass();
		
		var exceptionCaught = false;
		
		try
		{
			d.setName(1);	
		}
		catch (e)
		{
			__log("e=" + e);
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestCallProctedMethodDefinedAsAbstractAndThenOverwritten", function () {
		
		__log("TEST: TestCallProctedMethodDefinedAsAbstractAndThenOverwritten");

		var C = com.tns.tests.AbsClassImpl.extend("C1520", {
			echoString: function(s) {
				var echo = this.super.echoString(s);
				return echo + "!";
			}
		});
		var c = new C();
		
		var s = c.echo("test");
		
		expect(s).toBe("test!");
	});
	
	it("TestCharSequenceReturnValueIsTreatedAsStringWhenItIsString", function () {
		
		__log("TEST: TestCharSequenceReturnValueIsTreatedAsStringWhenItIsString");

		var d = new com.tns.tests.DummyClass();
		
		var s = d.getNameAsCharSequence();
		
		expect(s).toBe("dummy");
	});
	
	it("TestObjectReturnValueIsTreatedAsStringWhenItIsString", function () {
		
		__log("TEST: TestObjectReturnValueIsTreatedAsStringWhenItIsString");

		var d = new com.tns.tests.DummyClass();
		
		var s = d.getNameAsCharSequence();
		
		expect(s).toBe("dummy");
		expect(true).toEqual(true);
	});
	
	it("TestCanFindImplementationObjectWhenCreateExtendedObjectFromJava", function () {

		__log("TEST: TestCanFindImplementationObjectWhenCreateExtendedObjectFromJava");

		var O = java.lang.Object.extend("O1560", {});
		
		var ctor = (new O()).getClass().getConstructors()[0];
		
		var o = ctor.newInstance(null);

		expect(o).not.toBe(null);
	});
	
	it("TestCanCallMethodThatReturnsArrayOfInterfaces", function () {

		__log("TEST: TestCanCallMethodThatReturnsArrayOfInterfaces");

		var arr = java.lang.reflect.Array.newInstance(android.view.View.OnClickListener.class, 1);
		
		expect(arr).not.toBe(null);
		
		var listener = new android.view.View.OnClickListener("listener1580", {});
		
		arr[0] = listener;

		expect(arr[0]).not.toBe(null);
	});
	
	it("TestCanParseSignatureWithTypesThatContainsCapitalLettersForPrimitiveTypes", function () {
		
		__log("TEST: TestCanParseSignatureWithTypesThatContainsCapitalLettersForPrimitiveTypes");

		var formats = java.lang.reflect.Array.newInstance(java.text.NumberFormat.class, 2);
		formats[0] = java.text.NumberFormat.getInstance();
		formats[1] = java.text.NumberFormat.getIntegerInstance();
		var mf = new java.text.MessageFormat("{0}, {1}")
		mf.setFormats(formats);
		var arr = mf.parse("123, 4567");
		var len = arr.length;
	
		expect(len).toBe(2);
	});

	it("TestCanCallToStringOnClassProxy", function () {
		
		__log("TEST: TestCanCallToStringOnClassProxy");

		var view = android.view.View;
		var s = view.toString();
			
		expect(s.length).toBeGreaterThan(0);
	});
	
	it("When_accessing_class_property_on_a_extended_class_it_should_return_the_extended_class", function () {
		
		__log("TEST: When_accessing_class_property_on_a_extended_class_it_should_return_the_extended_class");
		
		var MyButton = com.tns.tests.Button1.extend("MyButton1615", {
			toString : function() {
		  		return "button1"	
		}});
		

		var button = new MyButton();
		var clazz1 = button.getClass();
		var name1 = clazz1.getName();
		expect(name1.indexOf("MyButton1615")).not.toEqual(-1);

		var clazz2 = MyButton.class;
		var name2 = clazz2.getName();
		expect(name2.indexOf("MyButton1615")).not.toEqual(-1);
	});
	
	it("When_using_global_in_a_module_global_should_be_defined", function () {
		
		__log("TEST: When_using_global_in_a_module_global_should_be_defined");
		
		var module = require("../modules/module");
		var canAccessGlobalObject = module.accessGlobalObject();
		expect(canAccessGlobalObject).toBe(true);
	});
	
	it("When_using_package_json_should_load_module", function () {
		
		__log("TEST: When_using_package_json_should_load_module");
		
		var module2 = require("../module2");
		var value456 = module2.value456;
		
		expect(value456).toBe(456);
	});
	
	it("When_require_bcl_module_it_should_be_loaded", function () {
		
		__log("TEST: When_require_bcl_module_it_should_be_loaded");
		
		var module = require("bclmodule");
		var moduleName = module.getModuleName();
		var expectedModuleName = "bclModule";
		expect(moduleName).toBe(expectedModuleName);
	});
	
	it("When_require_a_module_it_should_be_loaded", function () {
		
		__log("TEST: When_require_a_module_it_should_be_loaded");
		
		var module = require("./testModules/testmodule");
		var moduleName = module.getModuleName();
		
		expect(moduleName).toEqual("testModule");
	});
	
	it("When_require_a_bcl_module_in_a_dir_it_should_be_loaded", function () {
		
		__log("TEST: When_require_a_bcl_module_in_a_dir_it_should_be_loaded");
		
		var module = require("tests/testModules/testBclModule");
		var moduleName = module.getModuleName();
		
		expect(moduleName).toEqual("testBclModule");
	});
	
	it("When_require_a_module_that_is_a_directory_name_it_should_load_the_index_js_inside_it", function () {
		
		__log("TEST: When_require_a_module_that_is_a_directory_name_it_should_load_the_index_js_inside_it");
		
		var module = require("./testModules/someDirModule");
		var moduleName = module.getModuleName();
		
		expect(moduleName).toEqual("index.js");
	});
	
	it("When_require_a_bcl_module_that_is_a_directory_name_it_should_load_the_index_js_inside_it", function () {
		
		__log("TEST: When_require_a_bcl_module_that_is_a_directory_name_it_should_load_the_index_js_inside_it");
		
		var module = require("tests/testModules/someBclDirModule");
		var moduleName = module.getModuleName();
		
		expect(moduleName).toEqual("bclindex.js");
	});
	
	it("When_require_a_bcl_module_that_is_a_directory_name_it_should_load_the_package_json_inside_it", function () {
		
		__log("TEST: When_require_a_bcl_module_that_is_a_directory_name_it_should_load_the_package_json_inside_it");
		
		var module = require("tests/testModules/someModule");
		var value123 = module.value123;
		
		expect(value123).toBe(123);
	});
});