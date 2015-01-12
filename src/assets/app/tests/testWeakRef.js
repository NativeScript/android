var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestWeakRefGetsClearedAfterGC = function() {

	Log("TEST: TestWeakRefGetsClearedAfterGC");
	
	var wr = new WeakRef({ someProp: 12345 });
	
	var val = wr.get().someProp;
	Assert(val === 12345, "TestWeakRefGetsClearedAfterGC FAILED: Expected value '12345', actual value=" + val);
	
	gc();
	
	var val = wr.get();
	Assert(val === null, "TestWeakRefGetsClearedAfterGC FAILED: Expected value 'null', actual value=" + val);
}

var TestWeakRefGetsClearedAfterClear = function() {

	Log("TEST: TestWeakRefGetsClearedAfterClear");
	
	var wr = new WeakRef({ someProp: 54321 });
	
	var val = wr.get().someProp;
	Assert(val === 54321, "TestWeakRefGetsClearedAfterClear FAILED: Expected value '54321', actual value=" + val);
	
	wr.clear();
	
	var val = wr.get();
	Assert(val === null, "TestWeakRefGetsClearedAfterClear FAILED: Expected value 'null', actual value=" + val);
}

var TestWeakRefCanCreateMultipleInstances = function() {

	Log("TEST: TestWeakRefCanCreateMultipleInstances");
	
	var target = { someProp: 54321 };
	
	var wr1 = new WeakRef(target);
	var wr2 = new WeakRef(target);
	
	target = null;
	
	wr1.clear();
	
	var val = wr1.get();
	Assert(val === null, "TestWeakRefCanCreateMultipleInstances FAILED: Expected value 'null', actual value=" + val);
	
	val = wr2.get().someProp;
	Assert(val === 54321, "TestWeakRefCanCreateMultipleInstances FAILED: Expected value '54321', actual value=" + val);
}

var TestWeakRefCanCreateMultipleInstances2 = function() {

	Log("TEST: TestWeakRefCanCreateMultipleInstances2");
	
	var target = { someProp: 54321 };
	
	var wr1 = new WeakRef(target);
	var wr2 = new WeakRef(target);
	
	target = null;
	gc();
	
	var val1 = wr1.get();
	Assert(val1 === null, "TestWeakRefCanCreateMultipleInstances2 FAILED: Expected value (val1) 'null', actual value=" + val1);
	
	var val2 = wr2.get();
	Assert(val2 === null, "TestWeakRefCanCreateMultipleInstances2 FAILED: Expected value (val2) 'null', actual value=" + val2);
}

var TestWeakRefThrowsExceptionWhenConstructedWithWrongNumberOfParameters = function() {
	
	Log("TEST: TestWeakRefThrowsExceptionWhenConstructedWithWrongNumberOfParameters");
	
	var exceptionCaught = false;
	try
	{
		new WeakRef();
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	Assert(exceptionCaught === true, "TestWeakRefThrowsExceptionWhenConstructedWithWrongNumberOfParameters FAILED: Expected value 'true'");
	
	exceptionCaught = false;
	try
	{
		new WeakRef(1, 2);
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	Assert(exceptionCaught === true, "TestWeakRefThrowsExceptionWhenConstructedWithWrongNumberOfParameters FAILED: Expected value 'true'");
}

var TestWeakRefThrowsExceptionWhenConstructedWithNonObject = function() {
	
	Log("TEST: TestWeakRefThrowsExceptionWhenConstructedWithNonObject");
	
	var exceptionCaught = false;
	try
	{
		new WeakRef(1);
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	Assert(exceptionCaught === true, "TestWeakRefThrowsExceptionWhenConstructedWithNonObject FAILED: Expected value 'true'");
	
	exceptionCaught = false;
	try
	{
		new WeakRef(false);
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	Assert(exceptionCaught === true, "TestWeakRefThrowsExceptionWhenConstructedWithNonObject FAILED: Expected value 'true'");

	exceptionCaught = false;
	try
	{
		new WeakRef(null);
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	Assert(exceptionCaught === true, "TestWeakRefThrowsExceptionWhenConstructedWithNonObject FAILED: Expected value 'true'");

	exceptionCaught = false;
	try
	{
		new WeakRef(undefined);
	}
	catch (e)
	{
		exceptionCaught = true;
	}
	Assert(exceptionCaught === true, "TestWeakRefThrowsExceptionWhenConstructedWithNonObject FAILED: Expected value 'true'");
}

TestWeakRefGetsClearedAfterGC();
TestWeakRefGetsClearedAfterClear();
TestWeakRefCanCreateMultipleInstances();
TestWeakRefCanCreateMultipleInstances2();
TestWeakRefThrowsExceptionWhenConstructedWithWrongNumberOfParameters();
TestWeakRefThrowsExceptionWhenConstructedWithNonObject();
