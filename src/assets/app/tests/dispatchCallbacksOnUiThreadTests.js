var Assert = function(condition, failMessage) {
	if	(condition == false) {
		fail(failMessage);
	}
}

var TestProcessDataCallbackShouldBeExecutedOnUiThread = function() {

	Log("TEST: TestProcessDataCallbackShouldBeExecutedOnUiThread"); 

	var D = com.tns.tests.DispatchAsyncOpOnUIThreadTest.extends({
		processData: function(index, data) {
			if (index === 0) {
				Assert(data === 123, "TestProcessDataCallbackShouldBeExecutedOnUiThread FAILED: Expected value is 123, actual value=" + data);
			} else if (index === 1) {
				Assert(data === 456, "TestProcessDataCallbackShouldBeExecutedOnUiThread FAILED: Expected value is 456, actual value=" + data);
			} else if (index === 2) {
				Assert(data === 789, "TestProcessDataCallbackShouldBeExecutedOnUiThread FAILED: Expected value is 789, actual value=" + data);
			} else {
				Assert(false, "TestProcessDataCallbackShouldBeExecutedOnUiThread FAILED: Expected index is 0, 1 or 2. Actual value=" + index);
			}
		}
	});
	
	var d = new D();
	
	d.processDataAsync([123, 456, 789]);
}

TestProcessDataCallbackShouldBeExecutedOnUiThread();
