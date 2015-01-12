package com.tns.com.tns.tests;

public class Class1
{
	public static class Callback1 implements com.tns.tests.Class1.Callback1
	{
		public Callback1()
		{
		}
		
		public String getMessage()
		{
			java.lang.Object[] params = null;
			return (String)com.tns.Platform.callJSMethod(this, "getMessage", params);
		}
	}

}
