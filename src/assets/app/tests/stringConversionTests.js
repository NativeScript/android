var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestCanConvertNonTrivalJavaString1 = function() {

	Log("TEST: TestCanConvertNonTrivalJavaString1");
	
	var strConvTest = new com.tns.tests.StringConversionTest();
	
	var s = strConvTest.getString();
	
	var len = strConvTest.getLength();
	
	var isEqualsLength = strConvTest.equalsLength(s);
	
	Assert(isEqualsLength === true, "TestCanConvertNonTrivalJavaString1 FAILED: Expected value is " + len + ", actual value=" + s.length);
	
	var isEqualsString = strConvTest.equalsString(s);
	
	Assert(isEqualsString === true, "TestCanConvertNonTrivalJavaString1 FAILED: Expected value is 'true', actual value=" + isEqualsString);
}

var TestCanConvertNonTrivalJavaString2 = function() {

	Log("TEST: TestCanConvertNonTrivalJavaString2");

	var strConvTest = new com.tns.tests.StringConversionTest();
	
	var s = strConvTest.getString(1);
	
	var arr = strConvTest.getStringArr();
	
	arr[0] = s;
	
	var newS = arr[0];
	
	var isEqualsLength = strConvTest.equalsLength(newS);
	
	Assert(isEqualsLength === true, "TestCanConvertNonTrivalJavaString2 FAILED: Expected value is " + s.length + ", actual value=" + newS.length);
	
	var isEqualsString = strConvTest.equalsString(newS);
	
	Assert(isEqualsString === true, "TestCanConvertNonTrivalJavaString2 FAILED: Expected value is 'true', actual value=" + isEqualsString);
}

var TestCanConvertNonTrivalJavaString3 = function() {

	Log("TEST: TestCanConvertNonTrivalJavaString3");

	var strConvTest = new com.tns.tests.StringConversionTest();
	
	var s = strConvTest.s;
	
	var len = strConvTest.getLength();
	
	var isEqualsLength = strConvTest.equalsLength(s);
	
	Assert(isEqualsLength === true, "TestCanConvertNonTrivalJavaString3 FAILED: Expected value is " + len + ", actual value=" + s.length);
	
	var isEqualsString = strConvTest.equalsString(s);
	
	Assert(isEqualsString === true, "TestCanConvertNonTrivalJavaString3 FAILED: Expected value is 'true', actual value=" + isEqualsString);
}

var TestCanConvertNonTrivalJavaString4 = function() {

	Log("TEST: TestCanConvertNonTrivalJavaString4");
	
	var s = "";
	var len = 0;
	var isEqualsLength = false;
	var isEqualsString = false;
	
	var MyStringConversionTest = com.tns.tests.StringConversionTest.extend("StringConversionTest77", {
		callback: function(str) {
			s = str;
			len = this.getLength();
			isEqualsLength = this.equalsLength(s);
			isEqualsString = this.equalsString(s);
		}
	});
	
	var strConvTest = new MyStringConversionTest();
	
	strConvTest.triggerCallback();
	
	Assert(isEqualsLength === true, "TestCanConvertNonTrivalJavaString4 FAILED: Expected value is " + len + ", actual value=" + s.length);
		
	Assert(isEqualsString === true, "TestCanConvertNonTrivalJavaString4 FAILED: Expected value is 'true', actual value=" + isEqualsString);
}

TestCanConvertNonTrivalJavaString1();
TestCanConvertNonTrivalJavaString2();
TestCanConvertNonTrivalJavaString3();
TestCanConvertNonTrivalJavaString4();