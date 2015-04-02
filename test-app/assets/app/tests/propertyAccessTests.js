var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestAccessPropertyGetterStartingWithLowerLetter = function() {

	Log("TEST: TestAccessPropertyStartingWithLowerLetter");
	
	var dummyClass = com.tns.tests.DummyClass.class;
	
	var name1 = dummyClass.getName();
	Assert(name1 === "com.tns.tests.DummyClass", "TestAccessPropertyStartingWithLowerLetter FAILED: Expected value is 'com.tns.tests.DummyClass', actual value=" + name1);

	var name2 = dummyClass.Name;
	Assert(name2 === "com.tns.tests.DummyClass", "TestAccessPropertyStartingWithLowerLetter FAILED: Expected value is 'com.tns.tests.DummyClass', actual value=" + name2);

	var name3 = dummyClass.name;
	Assert(name3 === "com.tns.tests.DummyClass", "TestAccessPropertyStartingWithLowerLetter FAILED: Expected value is 'com.tns.tests.DummyClass', actual value=" + name3);
}

var TestAccessPropertySetterStartingWithLowerLetter = function() {

	Log("TEST: TestAccessPropertySetterStartingWithLowerLetter");
	
	var dummy = new com.tns.tests.DummyClass();
	
	var name = dummy.getName();
	
	var newName = name + "!";
	
	dummy.name = newName;
	
	var name1 = dummy.getName();
	
	Assert(name1 === newName, "TestAccessPropertySetterStartingWithLowerLetter FAILED: Expected value should be equal to='" + newName + "' but actual value=" + name1);
}

var TestAccessNonCamelCasePropertyWithoutGetter = function() {

	Log("TEST: TestAccessNonCamelCasePropertyWithoutGetter");
	
	var d = new com.tns.tests.DummyClass("test");
	
	var name1 = d.getname2();
	Assert(name1 === "test", "TestAccessNonCamelCasePropertyWithoutGetter FAILED: Expected value is 'test', actual value=" + name1);

	var name2 = d.name2;
	Assert(name2 === "test", "TestAccessNonCamelCasePropertyWithoutGetter FAILED: Expected value is 'test', actual value=" + name2);
}

var TestAccessNonCamelCasePropertyWithoutSetter = function() {

	Log("TEST: TestAccessNonCamelCasePropertyWithoutSetter");
	
	var dummy = new com.tns.tests.DummyClass();
	
	var name = dummy.getname2();
	
	var newName = name + "!";
	
	dummy.name2 = newName;
	
	var name1 = dummy.getname2();
	
	Assert(name1 === newName, "TestAccessNonCamelCasePropertyWithoutSetter FAILED: Expected value should be equal to='" + newName + "' but actual value=" + name1);
}

TestAccessPropertyGetterStartingWithLowerLetter();
TestAccessPropertySetterStartingWithLowerLetter();
TestAccessNonCamelCasePropertyWithoutGetter();
TestAccessNonCamelCasePropertyWithoutSetter();
