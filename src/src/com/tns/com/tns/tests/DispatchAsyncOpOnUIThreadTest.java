package com.tns.com.tns.tests;

public class DispatchAsyncOpOnUIThreadTest extends com.tns.tests.DispatchAsyncOpOnUIThreadTest implements com.tns.NativeScriptHashCodeProvider
{
	public DispatchAsyncOpOnUIThreadTest()
	{
		super();
		if (!__initialized) { 
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
	}
	
	public void processData(int index, int data)
	{
		if (isDoAsyncOverriden)
		{ 
			java.lang.Object[] params = { index, data };
			com.tns.Platform.callJSMethod(this, "processData", params);
		}
		else
		{
			super.processData(index, data);
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
	public void setNativeScriptOverrides(String[] overrides)
	{
		for (String name: overrides)
		{
			if (name.equals("processData"))
			{
				isDoAsyncOverriden = true;
			}
		}
	}

	private boolean __initialized;
	private boolean isDoAsyncOverriden;
}
