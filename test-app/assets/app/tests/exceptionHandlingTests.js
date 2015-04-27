var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestThrowJSExceptionThroughJavaAndCatchInJS = function() {

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

	Assert(exceptionThrown === true, "TestThrowJSExceptionThroughJavaAndCatchInJS FAILED: Exception should be thrown");
	Assert(exceptionCaught === true, "TestThrowJSExceptionThroughJavaAndCatchInJS FAILED: Exception should be caught");
	Assert(sameExObject === true, "TestThrowJSExceptionThroughJavaAndCatchInJS FAILED: The expected caught object should be strict equal to 'ex'");
}

var TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS = function() {

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

	Assert(exceptionThrown === true, "TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS FAILED: Exception should be thrown");
	Assert(exceptionCaught === true, "TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS FAILED: Exception should be caught");
	Assert(nativeExceptionFound === true, "TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS FAILED: The expected caught object should have 'nativeException' property set");
	Assert(exMsg === "This exception is thrown from JavaScript!", "TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS FAILED: Expected value is 'This exception is thrown from JavaScript!', actual value=" + exMsg);
}

var TestThrowJSExceptionAndCatchInJava = function() {

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

	Assert(exceptionThrown === true, "TestThrowJSExceptionAndCatchInJava FAILED: Exception should be thrown");
	Assert(exceptionCaught === false, "TestThrowJSExceptionAndCatchInJava FAILED: Exception should not be caught");
}

var TestThrowJavaExceptionFromJsAndCatchInJava = function() {

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

	Assert(exceptionThrown === true, "TestThrowJavaExceptionFromJsAndCatchInJava FAILED: Exception should be thrown");
	Assert(exceptionCaught === false, "TestThrowJavaExceptionFromJsAndCatchInJava FAILED: Exception should not be caught");
}

var TestMethodThatThrowsException = function() {
	
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
	
	Assert(exceptionCaught === true, "TestMethodThatThrowsException FAILED: Java exception was not caught")
}

var TestErrorObjectContainsJavaNativeException = function() {
	
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
	
	Assert(nativeException != undefined, "TestErrorObjectContainsJavaNativeException FAILED: Error object does not contain Java native exception");
	
	var nativeExceptionFrameCount = nativeException.getStackTrace().length;
	
	Assert(nativeExceptionFrameCount > 0, "TestErrorObjectContainsJavaNativeException FAILED: Java native exception does not contains stack frames");
}

var TestConstructorThatThrowsException = function() {
	
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
	
	Assert(exceptionCaught === true, "TestConstructorThatThrowsException FAILED: Java exception was not caught");
}

var TestArrayElementGetAccessThatThrowsException = function() {

	__log("TEST: TestArrayElementGetAccessThatThrowsException");
	
	var exceptionCaught = false;

	var d = new com.tns.tests.DummyClass();
	
	var arr = d.getDummyClassArrayAsObject();
	
	var arrLength = arr.length;
	
	Assert(arrLength == 1, "TestArrayElementGetAccessThatThrowsException FAILED: Expected array length is 1, actual length=" + arrLength);
	
	try
	{
		var dummy = arr[arrLength];
		
		var name = dummy.getName();
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	
	Assert(exceptionCaught === true, "TestArrayElementGetAccessThatThrowsException FAILED: Java exception was not caught during getting array element");
}

var TestArrayElementSetAccessThatThrowsException = function() {

	__log("TEST: TestArrayElementSetAccessThatThrowsException");
	
	var exceptionCaught = false;

	var d = new com.tns.tests.DummyClass();
	
	var arr = d.getDummyClassArrayAsObject();
	
	var arrLength = arr.length;
	
	Assert(arrLength == 1, "TestArrayElementSetAccessThatThrowsException FAILED: Expected array length is 1, actual length=" + arrLength);
	
	var last = arr[arrLength - 1];
	
	try
	{
		arr[arrLength] = last;
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	
	Assert(exceptionCaught === true, "TestArrayElementSetAccessThatThrowsException FAILED: Java exception was not caught during setting array element");
}

TestThrowJSExceptionThroughJavaAndCatchInJS();
TestThrowJavaExceptionFromJsThroughJavaAndCatchInJS();
TestThrowJSExceptionAndCatchInJava();
TestThrowJavaExceptionFromJsAndCatchInJava();
TestMethodThatThrowsException();
TestErrorObjectContainsJavaNativeException();
TestConstructorThatThrowsException();
TestArrayElementGetAccessThatThrowsException();
TestArrayElementSetAccessThatThrowsException();




