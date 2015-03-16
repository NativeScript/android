function TestSecondaryCallbackInvokationWithObjectParamsShouldWork()
{
	var u = new com.tns.tests.UseCallbackTest(123);
	
	u.setDataCallback(new com.tns.tests.UseCallbackTest.DataCallback("DataCallback_5", {
		onData: function(data, delay) {
			android.util.Log.i("TNS.TESTS", "data=" + data.getData());
		}
	}));
	
	u.setCleanCallback(new com.tns.tests.UseCallbackTest.CleanCallback("CleanCallback11", {
		onClean: function(delay) {
			gc();
			java.lang.System.gc();
		}
	}));

	u.enqueDataCallback(0);
	u.enqueDataCallback(10 * 1000);
	u.enqueCleanCallback(0);
}

TestSecondaryCallbackInvokationWithObjectParamsShouldWork();
