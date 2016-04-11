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