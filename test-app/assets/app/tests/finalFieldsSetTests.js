var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var When_a_java_final_field_is_set_exception_is_thrown = function() {

	var exceptionCaught = false;
	try
	{
		com.tns.tests.Button1.STATIC_IMAGE_ID = "NEW STATIC IMAGE ID VALUE";	
	}
	catch (e)
	{
		Log("Don't try to SET a final field " + e);
		exceptionCaught = true;
	}
	
	Assert(exceptionCaught === true, "When_a_java_final_field_is_set_exception_is_thrown FAILED: Exception(illegal access) should be thrown");
}

When_a_java_final_field_is_set_exception_is_thrown();
