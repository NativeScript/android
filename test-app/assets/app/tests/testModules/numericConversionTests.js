/*
	Test suite for numeric conversions and constructor/method resolutions.
*/

var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestCreateInstanceWithConstructorResolutionWithNumberLiteral = function() {

	__log("TEST: TestCreateInstanceWithConstructorResolutionWithNumberLiteral");
	
	var n = new com.tns.tests.NumericConversionTest(123);
	
	var s = n.getInit();

	Assert(s === "byte", "TestCreateInstanceWithConstructorResolutionWithNumberLiteral FAILED: Expected value is 'byte', actual value=" + s);
}

var TestCreateInstanceWithConstructorResolutionWithCastFunctions = function() {

	__log("TEST: TestCreateInstanceWithConstructorResolutionWithCastFunctions");
	
	var n1 = new com.tns.tests.NumericConversionTest(byte(123));
	var s1 = n1.getInit();
	Assert(s1 === "byte", "TestCreateInstanceWithConstructorResolutionWithCastFunctions FAILED: Expected value is 'byte', actual value=" + s1);

	var n2 = new com.tns.tests.NumericConversionTest(short(123));
	var s2 = n2.getInit();
	Assert(s2 === "byte", "TestCreateInstanceWithConstructorResolutionWithCastFunctions FAILED: Expected value is 'byte', actual value=" + s2);

	var n3 = new com.tns.tests.NumericConversionTest(long(123));
	var s3 = n3.getInit();
	Assert(s3 === "byte", "TestCreateInstanceWithConstructorResolutionWithCastFunctions FAILED: Expected value is 'byte', actual value=" + s3);
}

var TestCreateInstanceWithConstructorResolutionWithValuesFromJavaCalls = function() {

	__log("TEST: TestCreateInstanceWithConstructorResolutionWithValuesFromJavaCalls");
	
	var b = java.lang.Byte.parseByte("123");
	var n1 = new com.tns.tests.NumericConversionTest(b);
	var s1 = n1.getInit();
	Assert(s1 === "byte", "TestCreateInstanceWithConstructorResolutionWithValuesFromJavaCalls FAILED: Expected value is 'byte', actual value=" + s1);

	var i = java.lang.Integer.parseInt("12345");
	var n2 = new com.tns.tests.NumericConversionTest(i);
	var s2 = n2.getInit();
	Assert(s2 === "byte", "TestCreateInstanceWithConstructorResolutionWithValuesFromJavaCalls FAILED: Expected value is 'byte', actual value=" + s2);
}

var TestCreateInstanceWithConstructorResolutionWithPromotingValueUp = function() {

	__log("TEST: TestCreateInstanceWithConstructorResolutionWithPromotingValueUp");
	
	var n = new com.tns.tests.NumericConversionTest(null, short(1));
	var s = n.getInit();
	Assert(s === "Object,int", "TestCreateInstanceWithConstructorResolutionWithPromotingValueUp FAILED: Expected value is 'Object,int', actual value=" + s);
}

var TestCreateInstanceWithConstructorResolutionWithPromotingValueDown = function() {

	__log("TEST: TestCreateInstanceWithConstructorResolutionWithPromotingValueDown");
	
	var n = new com.tns.tests.NumericConversionTest(null, null, long(1));
	var s = n.getInit();
	Assert(s === "Object,Object,short", "TestCreateInstanceWithConstructorResolutionWithPromotingValueDown FAILED: Expected value is 'Object,Object,short', actual value=" + s);
}

var TestCallMethodWithResolutionWithPromotingValueUp = function() {

	__log("TEST: TestCallMethodWithResolutionWithPromotingValueUp");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method1(byte(1));
	Assert(s === "short=1", "TestCallMethodWithResolutionWithPromotingValueUp FAILED: Expected value is 'short=1', actual value=" + s);
}

var TestCallMethodWithResolutionWithPromotingValueDown = function() {

	__log("TEST: TestCallMethodWithResolutionWithPromotingValueDown");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method1(1);
	Assert(s === "short=1", "TestCallMethodWithResolutionWithPromotingValueDown FAILED: Expected value is 'short=1', actual value=" + s);

	var n1 = new com.tns.tests.NumericConversionTest();
	var s1 = n1.method1(long((1 << 16) + 2));
	Assert(s1 === "short=2", "TestCallMethodWithResolutionWithPromotingValueDown FAILED: Expected value is 'short=2', actual value=" + s1);
}

var TestLongCastToFloatConversionWhenThereIsDoubleOverload = function() {

	__log("TEST: TestLongCastToFloatConversionWhenThereIsDoubleOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method2(long(65536 + 2));
	Assert(s === "float=65538.0", "TestLongCastToFloatConversionWhenThereIsDoubleOverload FAILED: Expected value is 'float=65538.0', actual value=" + s);
}

var TestByteCastToFloatConversionWhenThereIsDoubleOverload = function() {

	__log("TEST: TestByteCastToFloatConversionWhenThereIsDoubleOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method2(byte(65536 + 2));
	Assert(s === "float=2.0", "TestByteCastToFloatConversionWhenThereIsDoubleOverload FAILED: Expected value is 'float=2.0', actual value=" + s);
}

var TestShortCastToFloatConversionWhenThereIsDoubleOverload = function() {

	__log("TEST: TestShortCastToFloatConversionWhenThereIsDoubleOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method2(short(65536 + 2));
	Assert(s === "float=2.0", "TestShortCastToFloatConversionWhenThereIsDoubleOverload FAILED: Expected value is 'float=2.0', actual value=" + s);
}

var TestDoubleCastWhenThereIsDoubleOverload = function() {

	__log("TEST: TestDoubleCastWhenThereIsDoubleOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method2(double(65536 + 2));
	Assert(s === "double=65538.0", "TestDoubleCastWhenThereIsDoubleOverload FAILED: Expected value is 'double=65538.0', actual value=" + s);
}

var TestNumberExpressionToFloatConversionWhenThereIsDoubleOverload = function() {

	__log("TEST: TestNumberExpressionToFloatConversionWhenThereIsDoubleOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method2(65536 + 2);
	Assert(s === "float=65538.0", "TestNumberExpressionToFloatConversionWhenThereIsDoubleOverload FAILED: Expected value is 'float=65538.0', actual value=" + s);
}

var TestDoubleCastToLongConversionWhenThereIsShortOverload = function() {

	__log("TEST: TestDoubleCastToLongConversionWhenThereIsShortOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method3(double(65536 + 2));
	Assert(s === "long=65538", "TestDoubleCastToLongConversionWhenThereIsShortOverload FAILED: Expected value is 'long=65538', actual value=" + s);
}

var TestFloatCastToLongConversionWhenThereIsShortOverload = function() {

	__log("TEST: TestFloatCastToLongConversionWhenThereIsShortOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method3(float(65536 + 2));
	Assert(s === "long=65538", "TestFloatCastToLongConversionWhenThereIsShortOverload FAILED: Expected value is 'long=65538', actual value=" + s);
}

var TestFloatCastToShortConversionWhenThereIsObjectOverload = function() {

	__log("TEST: TestFloatCastToShortConversionWhenThereIsObjectOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method4(float(65536 + 2));
	Assert(s === "short=2", "TestFloatCastToShortConversionWhenThereIsObjectOverload FAILED: Expected value is 'short=2', actual value=" + s);
}

var TestByteCastToShortConversionWhenThereIsObjectOverload = function() {

	__log("TEST: TestByteCastToShortConversionWhenThereIsObjectOverload");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method4(byte(65536 + 2));
	Assert(s === "short=2", "TestByteCastToShortConversionWhenThereIsObjectOverload FAILED: Expected value is 'short=2', actual value=" + s);
}

var TestResolveMethodWithByteCast = function() {

	__log("TEST: TestResolveMethodWithByteCast");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(byte(65536 + 123));
	Assert(s === "byte=123", "TestResolveMethodWithByteCast FAILED: Expected value is 'byte=123', actual value=" + s);
}

var TestResolveMethodWithShortCast = function() {

	__log("TEST: TestResolveMethodWithShortCast");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(short(65536 + 1234));
	Assert(s === "short=1234", "TestResolveMethodWithShortCast FAILED: Expected value is 'short=1234', actual value=" + s);
}

var TestResolveMethodWithoutCastFunction = function() {

	__log("TEST: TestResolveMethodWithoutCastFunction");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(123456);
	Assert(s === "int=123456", "TestResolveMethodWithoutCastFunction FAILED: Expected value is 'int=123456', actual value=" + s);
}

var TestResolveMethodWithLongCast = function() {

	__log("TEST: TestResolveMethodWithLongCast");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(long("123456789012"));
	Assert(s === "long=123456789012", "TestResolveMethodWithLongCast FAILED: Expected value is 'long=123456789012', actual value=" + s);
}

var TestResolveMethodWithFloatCast = function() {

	__log("TEST: TestResolveMethodWithFloatCast");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(float(1.23));
	Assert(s === "float=1.23", "TestResolveMethodWithFloatCast FAILED: Expected value is 'float=1.23', actual value=" + s);
}

var TestResolveMethodWithDoubleCast = function() {

	__log("TEST: TestResolveMethodWithDoubleCast");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(double(1));
	Assert(s === "double=1.0", "TestResolveMethodWithDoubleCast FAILED: Expected value is 'double=1.0', actual value=" + s);
}

var TestResolveIntMethodWithNumberObjectWithIntArgument = function() {

	__log("TEST: TestResolveIntMethodWithNumberObjectWithIntArgument");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(new Number(1));
	Assert(s === "int=1", "TestResolveIntMethodWithNumberObjectWithIntArgument FAILED: Expected value is 'int=1', actual value=" + s);
}

var TestResolveIntMethodWithNumberObjectWithDoubleArgument = function() {

	__log("TEST: TestResolveIntMethodWithNumberObjectWithDoubleArgument");
	
	var n = new com.tns.tests.NumericConversionTest();
	var s = n.method5(new Number(1.23));
	Assert(s === "double=1.23", "TestResolveIntMethodWithNumberObjectWithDoubleArgument 123FAILED: Expected value is 'double=1.23', actual value=" + s);
}

TestCreateInstanceWithConstructorResolutionWithNumberLiteral();
TestCreateInstanceWithConstructorResolutionWithCastFunctions();
TestCreateInstanceWithConstructorResolutionWithValuesFromJavaCalls();
TestCreateInstanceWithConstructorResolutionWithPromotingValueUp();
TestCreateInstanceWithConstructorResolutionWithPromotingValueDown();
TestCallMethodWithResolutionWithPromotingValueUp();
TestCallMethodWithResolutionWithPromotingValueDown();
TestLongCastToFloatConversionWhenThereIsDoubleOverload();
TestByteCastToFloatConversionWhenThereIsDoubleOverload();
TestShortCastToFloatConversionWhenThereIsDoubleOverload();
TestDoubleCastWhenThereIsDoubleOverload();
TestNumberExpressionToFloatConversionWhenThereIsDoubleOverload();
TestDoubleCastToLongConversionWhenThereIsShortOverload();
TestFloatCastToLongConversionWhenThereIsShortOverload();
TestFloatCastToShortConversionWhenThereIsObjectOverload();
TestByteCastToShortConversionWhenThereIsObjectOverload();
TestResolveMethodWithByteCast();
TestResolveMethodWithShortCast();
TestResolveMethodWithoutCastFunction();
TestResolveMethodWithLongCast();
TestResolveMethodWithFloatCast();
TestResolveMethodWithDoubleCast();
TestResolveIntMethodWithNumberObjectWithIntArgument();
TestResolveIntMethodWithNumberObjectWithDoubleArgument();