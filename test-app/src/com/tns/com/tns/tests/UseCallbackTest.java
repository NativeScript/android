package com.tns.com.tns.tests;

public class UseCallbackTest extends com.tns.tests.UseCallbackTest
{
	public UseCallbackTest(int data)
	{
		super(data);
	}
	
	public static class DataCallback implements com.tns.tests.UseCallbackTest.DataCallback
	{
		public void onData(com.tns.tests.UseCallbackTest.Data data, long delay)
		{
			java.lang.Object[] params = { data, delay };
			com.tns.Platform.callJSMethodWithDelay(this, "onData", delay, params);
		}
	}
	
	public static class CleanCallback implements com.tns.tests.UseCallbackTest.CleanCallback
	{
		public void onClean(long delay)
		{
			java.lang.Object[] params = { delay };
			com.tns.Platform.callJSMethodWithDelay(this, "onClean", delay, params);
		}
	}
}
