var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}


var TestCallMethodWithLongParameter = function() {

	Log("TEST: TestCallMethodWithLongParameter");
	
	var d = new com.tns.tests.DummyClass();
	
	var n1 = java.lang.Long.parseLong("9007199254740991"); // 9007199254740991 = 2^53-1
	var s1 = d.getLongAsString(n1);
	Assert(s1 === "9007199254740991", "TestCallMethodWithLongParameter FAILED: Expected value is '9007199254740991', actual value=" + s1);

	var n2 = java.lang.Long.parseLong("9007199254740992"); // 9007199254740992 = 2^53
	var s2 = d.getLongAsString(n2);
	Assert(s2 === "9007199254740992", "TestCallMethodWithLongParameter FAILED: Expected value is '9007199254740992', actual value=" + s2);
	
	var n3 = java.lang.Long.parseLong("9007199254740993"); // 9007199254740992 = 2^53+1
	var s3 = d.getLongAsString(n3);
	Assert(s3 === "9007199254740993", "TestCallMethodWithLongParameter FAILED: Expected value is '9007199254740993', actual value=" + s3);
}


//TestCallMethodWithLongParameter();


//fail("SUCCESS");



global.__onUncaughtError = function(error){
	if(error.nativeException){
		Log("err.message: " + error.message);
		Log("err.stackTrace: " + error.stackTrace);
		// false == do not continue execution
		return false;
	}
	
	// true == continue execution
	return true;
}

require("./mainpage");