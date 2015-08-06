describe("Tests numeric conversions and constructor/method resolutions", function() {

			var myCustomEquality = function(first, second) {
				return first == second;
			};
			
			beforeEach(function() {
				jasmine.addCustomEqualityTester(myCustomEquality);
			});
			
			it("TestCreateInstanceWithConstructorResolutionWithNumberLiteral", function () {
				
				__log("TEST: TestCreateInstanceWithConstructorResolutionWithNumberLiteral");
				
				var n = new com.tns.tests.NumericConversionTest(123);
				
				var s = n.getInit();
		
				expect(s).toBe("byte");
			});
		
			it("TestCreateInstanceWithConstructorResolutionWithCastFunctions", function () {
				
				__log("TEST: TestCreateInstanceWithConstructorResolutionWithCastFunctions");
				
				var n1 = new com.tns.tests.NumericConversionTest(byte(123));
				var s1 = n1.getInit();
				expect(s1).toBe("byte");
				
				var n2 = new com.tns.tests.NumericConversionTest(short(123));
				var s2 = n2.getInit();
				expect(s2).toBe("byte");
				
				var n3 = new com.tns.tests.NumericConversionTest(long(123));
				var s3 = n3.getInit();
				expect(s3).toBe("byte");
			});
			
			it("TestCreateInstanceWithConstructorResolutionWithValuesFromJavaCalls", function () {
				
				__log("TEST: TestCreateInstanceWithConstructorResolutionWithValuesFromJavaCalls");
				
				var b = java.lang.Byte.parseByte("123");
				var n1 = new com.tns.tests.NumericConversionTest(b);
				var s1 = n1.getInit();
				expect(s1).toBe("byte");
				
				var i = java.lang.Integer.parseInt("12345");
				var n2 = new com.tns.tests.NumericConversionTest(i);
				var s2 = n2.getInit();
				expect(s2).toBe("byte");
			});
			
			it("TestCreateInstanceWithConstructorResolutionWithPromotingValueUp", function () {
		
				__log("TEST: TestCreateInstanceWithConstructorResolutionWithPromotingValueUp");
				
				var n = new com.tns.tests.NumericConversionTest(null, short(1));
				var s = n.getInit();
				expect(s).toBe("Object,int");
			});
			
			it("TestCreateInstanceWithConstructorResolutionWithPromotingValueDown", function () {
				
				__log("TEST: TestCreateInstanceWithConstructorResolutionWithPromotingValueDown");
				
				var n = new com.tns.tests.NumericConversionTest(null, null, long(1));
				var s = n.getInit();
				expect(s).toBe("Object,Object,short");
			});
			
			it("TestCallMethodWithResolutionWithPromotingValueUp", function () {
				
				__log("TEST: TestCallMethodWithResolutionWithPromotingValueUp");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method1(byte(1));
				expect(s).toBe("short=1");
			});
			
			it("TestCallMethodWithResolutionWithPromotingValueDown", function () {
				
				__log("TEST: TestCallMethodWithResolutionWithPromotingValueDown");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method1(1);
				expect(s).toBe("short=1");
				
				var n1 = new com.tns.tests.NumericConversionTest();
				var s1 = n1.method1(long((1 << 16) + 2));
				expect(s1).toBe("short=2");
			});
			
			it("TestLongCastToFloatConversionWhenThereIsDoubleOverload", function () {
				
				__log("TEST: TestLongCastToFloatConversionWhenThereIsDoubleOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method2(long(65536 + 2));
				expect(s).toBe("float=65538.0");
			});
			
			it("TestByteCastToFloatConversionWhenThereIsDoubleOverload", function () {
				
				__log("TEST: TestByteCastToFloatConversionWhenThereIsDoubleOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method2(byte(65536 + 2));
				expect(s).toBe("float=2.0");
			});
			
			it("TestShortCastToFloatConversionWhenThereIsDoubleOverload", function () {
				
				__log("TEST: TestShortCastToFloatConversionWhenThereIsDoubleOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method2(short(65536 + 2));
				expect(s).toBe("float=2.0");
			});
			
			it("TestDoubleCastWhenThereIsDoubleOverload", function () {
				
				__log("TEST: TestDoubleCastWhenThereIsDoubleOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method2(double(65536 + 2));
				expect(s).toBe("double=65538.0");
			});
			
			it("TestNumberExpressionToFloatConversionWhenThereIsDoubleOverload", function () {
				
				__log("TEST: TestNumberExpressionToFloatConversionWhenThereIsDoubleOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method2(65536 + 2);
				expect(s).toBe("float=65538.0");
				expect(true).toEqual(true);
			});
			
			it("TestDoubleCastToLongConversionWhenThereIsShortOverload", function () {
		
				__log("TEST: TestDoubleCastToLongConversionWhenThereIsShortOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method3(double(65536 + 2));
				expect(s).toBe("long=65538");
			});
			
			it("TestFloatCastToLongConversionWhenThereIsShortOverload", function () {
				
				__log("TEST: TestFloatCastToLongConversionWhenThereIsShortOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method3(float(65536 + 2));
				expect(s).toBe("long=65538");
			});
			
			it("TestFloatCastToShortConversionWhenThereIsObjectOverload", function () {
				
				__log("TEST: TestFloatCastToShortConversionWhenThereIsObjectOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method4(float(65536 + 2));
				expect(s).toBe("short=2");
			});
			
			it("TestByteCastToShortConversionWhenThereIsObjectOverload", function () {
				
				__log("TEST: TestByteCastToShortConversionWhenThereIsObjectOverload");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method4(byte(65536 + 2));
				expect(s).toBe("short=2");
			});
			
			it("TestResolveMethodWithByteCast", function () {
				
				__log("TEST: TestResolveMethodWithByteCast");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(byte(65536 + 123));
				expect(s).toBe("byte=123");
			});
			
			it("TestResolveMethodWithShortCast", function () {
				
				__log("TEST: TestResolveMethodWithShortCast");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(short(65536 + 1234));
				expect(s).toBe("short=1234");
			});
			
			it("TestResolveMethodWithoutCastFunction", function () {
				
				__log("TEST: TestResolveMethodWithoutCastFunction");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(123456);
				expect(s).toBe("int=123456");
			});
			
			it("TestResolveMethodWithLongCast", function () {
				
				__log("TEST: TestResolveMethodWithLongCast");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(long("123456789012"));
				expect(s).toBe("long=123456789012");
			});
			
			it("TestResolveMethodWithFloatCast", function () {
				
				__log("TEST: TestResolveMethodWithFloatCast");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(float(1.23));
				expect(s).toBe("float=1.23");
			});
			
			it("TestResolveMethodWithDoubleCast", function () {
				
				__log("TEST: TestResolveMethodWithDoubleCast");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(double(1));
				expect(s).toBe("double=1.0");
			});
			
			it("TestResolveIntMethodWithNumberObjectWithIntArgument", function () {
				
				__log("TEST: TestResolveIntMethodWithNumberObjectWithIntArgument");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(new Number(1));
				expect(s).toBe("int=1");
			});
			
			it("TestResolveIntMethodWithNumberObjectWithDoubleArgument", function () {
				
				__log("TEST: TestResolveIntMethodWithNumberObjectWithDoubleArgument");
				
				var n = new com.tns.tests.NumericConversionTest();
				var s = n.method5(new Number(1.23));
				expect(s).toBe("float=1.23"); // changed to float=1.23 because of new casting (double, and float not just double)
			});
			
			// long cast
			it("TestIfNeedsToReturnLongItAlwaysReturnsLong", function() {

				__log("TEST: TestIfNeedsToReturnLongItAlwaysReturnsLong");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnLong : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnLong(instance, 1234);
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(value).toBe(1234);
			});
			
			it("TestIfNeedsToReturnLongItAlwaysReturnsLong1", function() {

				__log("TEST: TestIfNeedsToReturnLongItAlwaysReturnsLong1");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnLong : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnLong(instance, 3147483647);
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(value).toBe(-1147483649); // this is because v8 treats 3147483647 as in if not passed like so long(3147483647)
			});

			it("TestIfNeedsToReturnLongItAlwaysReturnsLong2", function() {

				__log("TEST: TestIfNeedsToReturnLongItAlwaysReturnsLong2");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnLong : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnLong(instance, long(3147483647));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(value).toBe(3147483647);
			});
			
			it("TestIfNeedsToReturnLongItAlwaysReturnsLong3", function() {

				__log("TEST: TestIfNeedsToReturnLongItAlwaysReturnsLong3");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnLong : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnLong(instance, long(1234));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(value).toBe(1234);
			});

			// int cast
			it("TestIfNeedsToReturnIntItAlwaysReturnsInt", function() {

						__log("TEST: TestIfNeedsToReturnIntItAlwaysReturnsInt");

						var N = com.tns.tests.NumericConversions.extend({
							hasToReturnInt : function(val) {
								return val;
							}
						});

						var instance = new N();

						var exceptionCaught = false, 
							exceptionThrown = true;
						try {
							var value = com.tns.tests.NumericConversions.callHasToReturnInt(instance, 1234);
							exceptionThrown = false;
						} catch (e) {
							exceptionCaught = true;
							__log("Error: " + e);
						}

						expect(exceptionThrown).toBe(false);
						expect(exceptionCaught).toBe(false);
						expect(value).toBe(1234);
			});
			
			it("TestIfNeedsToReturnIntAndWePassLongExceptionShouldBeThrown", function() {

				__log("TEST: TestIfNeedsToReturnIntAndWePassLongExceptionShouldBeThrown");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnInt : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnInt(instance, long(3147483647));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				expect(exceptionThrown).toBe(true);
				expect(exceptionCaught).toBe(true);
			});
			
			// double cast
			it("TestIfNeedsToReturnDoubleItAlwaysReturnsDouble", function() {

				__log("TEST: TestIfNeedsToReturnDoubleItAlwaysReturnsDouble");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnDouble : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnDouble(instance, 1234.1234); //float
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}
				
				var isReturnValueCorrect = false;
				
				var difference = Math.abs(value - 1234.1234);
				var e = 0.001;
				
				if(difference <= e) {
					isReturnValueCorrect = true;
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(isReturnValueCorrect).toBe(true);
			});

			it("TestIfNeedsToReturnDoubleItAlwaysReturnsDouble1", function() {

				__log("TEST: TestIfNeedsToReturnDoubleItAlwaysReturnsDouble1");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnDouble : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnDouble(instance, double(1234.1234));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(value).toBe(1234.1234);
			});
			
			it("TestIfNeedsToReturnDoubleItAlwaysReturnsDouble2", function() {

				__log("TEST: TestIfNeedsToReturnDoubleItAlwaysReturnsDouble2");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnDouble : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false,
					exceptionThrown = true;
				try {
					var doubleVal = 1e-40;
					var value = com.tns.tests.NumericConversions.callHasToReturnDouble(instance, doubleVal);
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				var isReturnValueCorrect = false;
				
				var difference = Math.abs(value - doubleVal);
				var e = 0.000000000000000000000000000000000000001;
				
				if(difference <= e) {
					isReturnValueCorrect = true;
				}
				
				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(isReturnValueCorrect).toBe(true);
			});
			
			it("TestIfNeedsToReturnDoubleItAlwaysReturnsDouble3", function() {

				__log("TEST: TestIfNeedsToReturnDoubleItAlwaysReturnsDouble3");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnDouble : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false,
					exceptionThrown = true;
				try {
					var doubleVal = 1e-40;
					var value = com.tns.tests.NumericConversions.callHasToReturnDouble(instance, double(doubleVal));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}
				
				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(value).toBe(doubleVal);
			});
			
			it("TestIfNeedsToReturnDoubleItAlwaysReturnsDouble4", function() {

				__log("TEST: TestIfNeedsToReturnDoubleItAlwaysReturnsDouble4");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnDouble : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnDouble(instance, float(1234.1234));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}

				var isReturnValueCorrect = false;
				
				var difference = Math.abs(value - 1234.1234);
				var e = 0.001;
				
				if(difference <= e) {
					isReturnValueCorrect = true;
				}
				
				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(isReturnValueCorrect).toBe(true);
			});
			
			// float cast
			it("TestIfNeedsToReturnFloatItAlwaysReturnsFloat", function() {

				__log("TEST: TestIfNeedsToReturnFloatItAlwaysReturnsFloat");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnFloat : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var testFloat = 1234.1234;
					var value = com.tns.tests.NumericConversions.callHasToReturnFloat(instance, testFloat);
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}
				
				var isReturnValueCorrect = false;
				
				var difference = Math.abs(value - 1234.1234);
				var e = 0.001;
				
				if(difference <= e) {
					isReturnValueCorrect = true;
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(isReturnValueCorrect).toBe(true);
			});
			
			it("TestIfNeedsToReturnFloatItAlwaysReturnsFloat1", function() {

				__log("TEST: TestIfNeedsToReturnFloatItAlwaysReturnsFloat1");

				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnFloat : function(val) {
						return val;
					}
				});

				var instance = new N();

				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var value = com.tns.tests.NumericConversions.callHasToReturnFloat(instance, float(1234.1234));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}
				
				var isReturnValueCorrect = false;
				
				var difference = Math.abs(value - 1234.1234);
				var e = 0.001;
				
				if(difference <= e) {
					isReturnValueCorrect = true;
				}

				expect(exceptionThrown).toBe(false);
				expect(exceptionCaught).toBe(false);
				expect(isReturnValueCorrect).toBe(true);
			});
	
			it("TestIfNeedsToReturnFloatAndWePassDoubleExceptionShouldBeThrown", function() {
		
				__log("TEST: TestIfNeedsToReturnFloatAndWePassDoubleExceptionShouldBeThrown");
		
				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnFloat : function(val) {
						return val;
					}
				});
		
				var instance = new N();
		
				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var doubleVal = 1e-40;
					var value = com.tns.tests.NumericConversions.callHasToReturnFloat(instance, double(doubleVal));
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}
		
				expect(exceptionThrown).toBe(true);
				expect(exceptionCaught).toBe(true);
			});
			
			it("TestIfNeedsToReturnFloatAndWePassDoubleExceptionShouldBeThrown1", function() {
				
				__log("TEST: TestIfNeedsToReturnFloatAndWePassDoubleExceptionShouldBeThrown1");
		
				var N = com.tns.tests.NumericConversions.extend({
					hasToReturnFloat : function(val) {
						return val;
					}
				});
		
				var instance = new N();
		
				var exceptionCaught = false, 
					exceptionThrown = true;
				try {
					var doubleVal = 1e-40;
					var value = com.tns.tests.NumericConversions.callHasToReturnFloat(instance, doubleVal);
					exceptionThrown = false;
				} catch (e) {
					exceptionCaught = true;
					__log("Error: " + e);
				}
		
				expect(exceptionThrown).toBe(true);
				expect(exceptionCaught).toBe(true);
			});
});