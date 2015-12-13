describe("Tests exception handling ", function () {
	
	var myCustomEquality = function(first, second) {
		return first == second;
	};
	
	beforeEach(function() {
		jasmine.addCustomEqualityTester(myCustomEquality);
	});
	
	it("TestThrowJSExceptionThroughJavaAndCatchInJS", function () {
		
		__log("TEST: TestThrowJSExceptionThroughJavaAndCatchInJS");
		
		var exceptionThrown = false;
		var exceptionCaught = false;
		var sameExObject = false;
		
		var ex = { myProp: "SomeValue" };
		
		var EH = com.tns.tests.ExceptionHandlingTest.extend("ExceptionHandlingTest", {
			onEvent1: function(s) {
				exceptionThrown = true;
				throw ex;
			}
		});
		
		var eh = new EH();
		
		try
		{
			eh.triggerEvent1("test");
		}
		catch (e)
		{
			exceptionCaught = true;
			sameExObject = e === ex;
			__log("e=" + e);
		}

		expect(exceptionThrown).toBe(true);
		expect(exceptionCaught).toBe(true);
		expect(sameExObject).toBe(true);
	});
	
	it("TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS", function () {

		__log("TEST: TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS");
		
		var exceptionThrown = false;
		var exceptionCaught = false;
		var nativeExceptionFound = false;
		var exMsg = "";
		
		var ex = new java.lang.Exception("This exception is thrown from JavaScript!");
		
		var EH = com.tns.tests.ExceptionHandlingTest.extend("ExceptionHandlingTest53", {
			onEvent1: function(s) {
				exceptionThrown = true;
				throw ex;
			}
		});
		
		var eh = new EH();
		
		try
		{
			eh.triggerEvent1("test");
		}
		catch (e)
		{
			exceptionCaught = true;
			nativeExceptionFound = e.nativeException !== undefined;
			if (nativeExceptionFound)
			{
				exMsg = e.nativeException.getMessage();
			}
		}

		expect(exceptionThrown).toBe(true);
		expect(exceptionCaught).toBe(true);
		expect(nativeExceptionFound).toBe(true);
		expect(exMsg).toBe("This exception is thrown from JavaScript!");
	});
	
	it("TestThrowJSExceptionAndCatchInJava", function () {
		
		__log("TEST: TestThrowJSExceptionAndCatchInJava");
		
		var exceptionThrown = false;
		var exceptionCaught = true;
		
		var EH = com.tns.tests.ExceptionHandlingTest.extend("ExceptionHandlingTest89", {
			onEvent1: function(s) {
				exceptionThrown = true;
				throw "My Exception String";
			}
		});
		
		var eh = new EH();
		
		try
		{
			eh.triggerEvent1WithCatchClause("test");
			exceptionCaught = false;
		}
		catch (e)
		{
			exceptionCaught = true;
		}

		expect(exceptionThrown).toBe(true);
		expect(exceptionCaught).toBe(false);
	});
	
	it("TestThrowJavaExceptionFromJsAndCatchInJava", function () {
		
		__log("TEST: TestThrowJavaExceptionFromJsAndCatchInJava");
		
		var exceptionThrown = false;
		var exceptionCaught = true;
		
		var ex = new java.lang.Exception("This exception is thrown from JavaScript!");
		
		var EH = com.tns.tests.ExceptionHandlingTest.extend("ExceptionHandlingTest121", {
			onEvent1: function(s) {
				exceptionThrown = true;
				throw ex;
			}
		});
		
		var eh = new EH();
		
		try
		{
			eh.triggerEvent1WithCatchClause("test");
			exceptionCaught = false;
		}
		catch (e)
		{
			exceptionCaught = true;
		}

		expect(exceptionThrown).toBe(true);
		expect(exceptionCaught).toBe(false);
	});
	
	it("TestMethodThatThrowsException", function () {
		
		__log("TEST: TestMethodThatThrowsException");
		
		var exceptionCaught = false;
		
		var dummy = new com.tns.tests.DummyClass();
		
		try
		{
			dummy.methodThatThrowsException();
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestErrorObjectContainsJavaNativeException", function () {
		
		
		__log("TEST: TestErrorObjectContainsJavaNativeException");
		
		var nativeException = undefined;
		
		var dummy = new com.tns.tests.DummyClass();
		
		try
		{
			dummy.methodThatThrowsException();
		}
		catch (e)
		{
			var nativeException = e.nativeException;
		}

		__log("nativeException=" + nativeException);
		
		expect(nativeException).not.toEqual(undefined);
		
		var nativeExceptionFrameCount = nativeException.getStackTrace().length;
		
		expect(nativeExceptionFrameCount).toBeGreaterThan(0);
	});
	
	it("TestConstructorThatThrowsException", function () {
		
		__log("TEST: TestConstructorThatThrowsException");
		
		var exceptionCaught = false;
		
		try
		{
			var dummy = new com.tns.tests.DummyClass(true /* throwsException */);
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestArrayElementGetAccessThatThrowsException", function () {
		
		__log("TEST: TestArrayElementGetAccessThatThrowsException");
		
		var exceptionCaught = false;

		var d = new com.tns.tests.DummyClass();
		
		var arr = d.getDummyClassArrayAsObject();
		
		var arrLength = arr.length;
		
		expect(arrLength).toEqual(1);
		
		try
		{
			var dummy = arr[arrLength];
			
			var name = dummy.getName();
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
	});
	
	it("TestArrayElementSetAccessThatThrowsException", function () {
		
		__log("TEST: TestArrayElementSetAccessThatThrowsException");
		
		var exceptionCaught = false;

		var d = new com.tns.tests.DummyClass();
		
		var arr = d.getDummyClassArrayAsObject();
		
		var arrLength = arr.length;
		
		expect(arrLength).toEqual(1);
		
		var last = arr[arrLength - 1];
		
		try
		{
			arr[arrLength] = last;
		}
		catch (e)
		{
			exceptionCaught = true;
		}
		
		expect(exceptionCaught).toBe(true);
		
	});
	
	it("should not wrap the thrown exception into NativeScriptException", function () {
		var MyTest1 = com.tns.tests.ExceptionHandlingTest.extend({
			onGetFile: function(s) {
				new java.io.File("/blah/blah").createNewFile();
			}
		});
		var mt1 = new MyTest1();
		var e1 = mt1.getException("myfile1.txt");
		expect(e1.getClass()).toBe(java.io.IOException.class);

		var MyTest2 = com.tns.tests.ExceptionHandlingTest.extend({
			onGetFile: function(s) {
				throw new java.io.IOException(s);
			}
		});
		var mt2 = new MyTest2();
		var e2 = mt2.getException("myfile2.txt");
		expect(e2.getClass()).toBe(java.io.IOException.class);
	});

});