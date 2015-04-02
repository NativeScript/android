package com.tns.com.tns.tests;


public class ExceptionHandlingTest extends com.tns.tests.ExceptionHandlingTest implements com.tns.NativeScriptHashCodeProvider 
{
	private boolean __initialized;
	private boolean __ctorOverriden;
	private boolean isOnEvent1Overriden;

	public ExceptionHandlingTest()
	{
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
		if (__ctorOverriden)
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "init", true, params);
		}
	}
	
	public void onEvent1(String s)
	{
		if (isOnEvent1Overriden)
		{
			Object[] params = { s };
			com.tns.Platform.callJSMethod(this, "onEvent1", params);
		}
		else
		{
			super.onEvent1(s);
		}
	}
	
	@Override
	public int hashCode__super()
	{
		return super.hashCode();
	}

	@Override
	public boolean equals__super(Object other)
	{
		return super.equals(other);
	}

	@Override
	public void setNativeScriptOverride(String name)
	{
		if (name.equals("init"))
		{
			__ctorOverriden = true;
		}
		else if (name.equals("onEvent1"))
		{
			isOnEvent1Overriden = true;
		}
		
	}

}
