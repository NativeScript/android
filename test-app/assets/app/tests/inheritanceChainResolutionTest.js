var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestCallWithStringArgumentWhenThereIsStringOverload = function() {

	__log("TEST: TestCallWithStringArgumentWhenThereIsStringOverload");
	
	var i = new com.tns.tests.InheritanceChainResolutionTest();
	
	var s = i.echo1("test123");
	
	Assert(s === "String=test123", "TestCallWithStringArgumentWhenThereIsStringOverload FAILED: Expected value is 'String=test123', actual value=" + s);
}

var TestCallWithStringArgumentWhenThereIsNoStringOverload = function() {

	__log("TEST: TestCallWithStringArgumentWhenThereIsNoStringOverload");
	
	var i = new com.tns.tests.InheritanceChainResolutionTest();
	
	var s = i.echo2("test123");
	
	Assert(s === "Object=test123", "TestCallWithStringArgumentWhenThereIsNoStringOverload FAILED: Expected value is 'Object=test123', actual value=" + s);
}

TestCallWithStringArgumentWhenThereIsStringOverload();
TestCallWithStringArgumentWhenThereIsNoStringOverload();