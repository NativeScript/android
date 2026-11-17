describe("Test Method Resolution When Nulls are passed", function () {
	var dummyClass = new com.tns.tests.DummyClass();

	var objNull = java.lang.Object.null;
	var dummyNull = com.tns.tests.DummyClass.null;
	var staticInterfaceNull = com.tns.tests.DummyClass.MyInterface.null;
	var publicInterfaceNull = com.tns.tests.MyPublicInterface.null;
	var fileNull = java.io.File.null;
	var stringNull = java.lang.String.null;

	it("When_accessing_.null_of_a_node_the_same_instance_will_be_returned", function () {
		__log("TEST: When_accessing_.null_of_a_node_the_same_instance_will_be_returned");

		var dummyNull = com.tns.tests.DummyClass.null;
		var testIterations = 10;

		for (var i = 0; i < testIterations; i++) {
			var res = dummyNull === com.tns.tests.DummyClass.null;
			expect(res).toBe(true);
		}
	});

	it("When_call_method_valueOf_of_.null_object_should_return_null", function () {
		__log("TEST: When_call_method_valueOf_of_.null_object_should_return_null");

		var res = objNull.valueOf();
		expect(res).toBeNull();
		res = stringNull.valueOf();
		expect(res).toBeNull();
		res = dummyNull.valueOf();
		expect(res).toBeNull();
	});

	it("When_call_method_methodWithOverloadsWithOneArgument_with_java.lang.Object_argument", function () {
		__log("TEST: When_call_method_methodWithOverloadsWithOneArgument_with_java.lang.Object_argument");

		var result = dummyClass.methodWithOverloadsWithOneArgument(objNull);

		expect(result).toContain("Object");
	});

	it("When_call_method_methodWithOverloadsWithOneArgument_with_com.tns.tests.DummyClass_argument", function () {
		__log("TEST: When_call_method_methodWithOverloadsWithOneArgument_with_com.tns.tests.DummyClass_argument");

		var result = dummyClass.methodWithOverloadsWithOneArgument(dummyNull);

		expect(result).toContain("DummyClass");
	});

	it("When_call_method_methodWithOverloadsWithOneArgument_with_java.lang.String_argument", function () {
		__log("TEST: When_call_method_methodWithOverloadsWithOneArgument_with_java.lang.String_argument");

		var result = dummyClass.methodWithOverloadsWithOneArgument(stringNull);

		expect(result).toContain("String");
	});

	it("When_call_method_methodWithOverloadsWithOneArgument_with_java.io.File_argument", function () {
		__log("TEST: When_call_method_methodWithOverloadsWithOneArgument_with_java.io.File_argument");

		var result = dummyClass.methodWithOverloadsWithOneArgument(fileNull);

		expect(result).toContain("File");
	});

	it("When_call_method_methodWithOverloadsWithOneArgument_with_com.tns.tests.DummyClass.MyInterface_argument", function () {
		__log("TEST: When_call_method_methodWithOverloadsWithOneArgument_with_com.tns.tests.DummyClass.MyInterface_argument");

		var result = dummyClass.methodWithOverloadsWithOneArgument(staticInterfaceNull);

		expect(result).toContain("MyInterface");
	});

	it("When_call_method_methodWithOverloadsWithOneArgument_with_com.tns.tests.MyPublicInterface_argument", function () {
		__log("TEST: When_call_method_methodWithOverloadsWithOneArgument_with_com.tns.tests.MyPublicInterface_argument");

		var result = dummyClass.methodWithOverloadsWithOneArgument(publicInterfaceNull);

		expect(result).toContain("MyPublicInterface");
	});

	it("When_call_method_methodWithOverloadsWithThreeArguments_with_Object_String_Object_arguments", function() {
		__log("TEST: When_call_method_methodWithOverloadsWithThreeArguments_with_Object_String_Object_arguments");

		var result = dummyClass.methodWithOverloadsWithThreeArguments(objNull, stringNull, objNull);

		expect(result).toMatch(/\S*.Object and \S*.String and \S*.Object/);
	});

	it("When_call_method_methodWithOverloadsWithThreeArguments_with_Object_String_MyInterface_arguments", function() {
		__log("TEST: When_call_method_methodWithOverloadsWithThreeArguments_with_Object_String_MyInterface_arguments");

		var result = dummyClass.methodWithOverloadsWithThreeArguments(objNull, stringNull, staticInterfaceNull);

		expect(result).toMatch(/\S*.Object and \S*.String and \S*.MyInterface/);
	});

	it("When_call_method_methodWithOverloadsWithThreeArguments_with_Object_String_MyPublicInterface_arguments", function() {
		__log("TEST: When_call_method_methodWithOverloadsWithThreeArguments_with_Object_String_MyPublicInterface_arguments");

		var result = dummyClass.methodWithOverloadsWithThreeArguments(objNull, stringNull, publicInterfaceNull);

		expect(result).toMatch(/\S*.Object and \S*.String and \S*.MyPublicInterface/);
	});

	it("When_call_method_methodWithOverloadsWithThreeArguments_with_Object_Object_Object_arguments", function() {
		__log("TEST: When_call_method_methodWithOverloadsWithThreeArguments_with_Object_Object_Object_arguments");

		var result = dummyClass.methodWithOverloadsWithThreeArguments(objNull, objNull, objNull);

		expect(result).toMatch(/\S*.Object and \S*.Object and \S*.Object/);
	});

	it("When_call_method_methodWithOverloadsWithThreeArguments_with_MyInterface_MyInterface_MyPublicInterface_arguments", function() {
		__log("TEST: When_call_method_methodWithOverloadsWithThreeArguments_with_MyInterface_MyInterface_MyPublicInterface_arguments");

		var result = dummyClass.methodWithOverloadsWithThreeArguments(staticInterfaceNull, staticInterfaceNull, publicInterfaceNull);

		expect(result).toMatch(/\S*.MyInterface and \S*.MyInterface and \S*.MyPublicInterface/);
	});

	it("When_call_method_methodWithOverloadsWithThreeArguments_with_String_Object_Object_arguments", function() {
		__log("TEST: When_call_method_methodWithOverloadsWithThreeArguments_with_String_Object_Object_arguments");

		var result = dummyClass.methodWithOverloadsWithThreeArguments(stringNull, objNull, objNull);

		expect(result).toMatch(/\S*.String and \S*.Object and \S*.Object/);
	});
});

describe("Test Constructor Resolution When Nulls are passed", function () {
	var objNull = java.lang.Object.null;
	var dummyNull = com.tns.tests.DummyClass.null;
	var staticInterfaceNull = com.tns.tests.DummyClass.MyInterface.null;
	var publicInterfaceNull = com.tns.tests.MyPublicInterface.null;
	var fileNull = java.io.File.null;
	var stringNull = java.lang.String.null;

	it("When_call_DummyClass_ctor_with_Object_argument", function () {
		__log("TEST: When_call_DummyClass_ctor_with_Object_argument");

		var result = new com.tns.tests.DummyClass(objNull).arbitraryString;
		
		expect(result).toContain("Object");
	});
	
	it("When_call_DummyClass_ctor_with_DummyClass_argument", function () {
		__log("TEST: When_call_DummyClass_ctor_with_DummyClass_argument");

		var result = new com.tns.tests.DummyClass(dummyNull).arbitraryString;
		
		expect(result).toContain("DummyClass");
	});
	
	it("When_call_DummyClass_ctor_with_com.tns.tests.DummyClass.MyInterface_argument", function () {
		__log("TEST: When_call_DummyClass_ctor_with_com.tns.tests.DummyClass.MyInterface_argument");

		var result = new com.tns.tests.DummyClass(staticInterfaceNull).arbitraryString;
		
		expect(result).toContain("MyInterface");
	});
	
	it("When_call_DummyClass_ctor_with_com.tns.tests.DummyClass_and_Object_arguments", function () {
		__log("TEST: When_call_DummyClass_ctor_with_com.tns.tests.DummyClass_and_Object_arguments");

		var result = new com.tns.tests.DummyClass(dummyNull, objNull).arbitraryString;

		expect(result).toMatch(/\S*.DummyClass and \S*.Object/);
	});
});

// Each spec reads .null itself: the suites above read it at require time, and
// these must hold whether or not anything else in this runtime has read it.
describe("Typed null literals leave their class untouched", function () {
	var dummyClass = new com.tns.tests.DummyClass();

	it("keeps String.valueOf working after String.null is read", function () {
		var Described = java.lang.Object.extend({
			toString: function () {
				return "described in JS";
			}
		});
		void java.lang.String.null;

		expect(java.lang.String.valueOf(new Described())).toBe("described in JS");
		expect(java.lang.String.valueOf(new java.io.File("/typed-null"))).toBe("/typed-null");
		expect(java.lang.String.valueOf(42)).toBe("42");
		expect(java.lang.String.valueOf(true)).toBe("true");
	});

	it("keeps Integer.valueOf and an enum's valueOf working after their .null is read", function () {
		void java.lang.Integer.null;
		void java.util.concurrent.TimeUnit.null;

		expect(java.lang.Integer.valueOf(5).intValue()).toBe(5);
		expect(java.util.concurrent.TimeUnit.valueOf("SECONDS").name()).toBe("SECONDS");
	});

	it("does not accept the class itself as a typed null once its .null is read", function () {
		void java.io.File.null;

		expect(function () {
			java.util.Objects.toString(java.io.File, "fallback");
		}).toThrowError(/Cannot convert object/);
	});

	it("returns a cached object, distinct from the class, whose valueOf is null", function () {
		var fileNull = java.io.File.null;

		expect(fileNull).not.toBe(java.io.File);
		expect(typeof fileNull).toBe("object");
		expect(java.io.File.null).toBe(fileNull);
		expect(fileNull.valueOf()).toBeNull();
		expect(Object.keys(fileNull)).toEqual([]);
	});

	it("converts a typed null to a Java null", function () {
		expect(java.util.Objects.isNull(java.io.File.null)).toBe(true);
		expect(java.util.Objects.toString(java.lang.Object.null, "fallback")).toBe("fallback");
		expect(java.lang.String.valueOf(java.lang.String.null)).toBe("null");
	});

	it("types a JS-extended class's .null as its Java base class", function () {
		var Extended = com.tns.tests.DummyClass.extend({});
		var extendedNull = Extended.null;

		expect(extendedNull).not.toBe(com.tns.tests.DummyClass.null);
		expect(Extended.null).toBe(extendedNull);
		expect(extendedNull.valueOf()).toBeNull();
		expect(dummyClass.methodWithOverloadsWithOneArgument(extendedNull)).toContain("DummyClass");
	});

	it("gives a worker typed nulls of its own that leave its classes untouched", function (done) {
		var worker = new Worker("./typedNullWorker.js");
		worker.onmessage = function (msg) {
			expect(msg.data).toEqual({
				valueOf: "7",
				nullValueOf: null,
				distinct: true,
				overload: "java.io.File",
				isNull: true
			});
			worker.terminate();
			done();
		};
		worker.onerror = function (error) {
			fail("worker failed: " + error.message);
			worker.terminate();
			done();
		};
		worker.postMessage("go");
	});
});