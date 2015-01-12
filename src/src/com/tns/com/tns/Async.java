package com.tns.com.tns;

public class Async
{
	public static class CompleteCallback implements com.tns.Async.CompleteCallback {

		@Override
		public void onComplete(Object result, Object context)
		{
			java.lang.Object[] params = new Object[2];
			params[0] = result;			
			params[1] = context;
			com.tns.Platform.callJSMethod(this, "onComplete", params);
		}
	}
}
