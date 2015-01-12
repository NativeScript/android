package com.tns.com.tns.tests;

public class StringConversionTest extends com.tns.tests.StringConversionTest implements com.tns.NativeScriptHashCodeProvider
{
	public StringConversionTest() throws Exception
	{
		super();
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
	}
	
	public void callback(String str)
	{
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
		if (isCallabackOverriden)
		{
			Object[] params = { str };
			com.tns.Platform.callJSMethod(this, "callback", params);
		}
		else
		{
			super.callback(str);
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
			if (name.equals("callback"))
			{
				isCallabackOverriden = true;
			}
		}
	}

	private boolean __initialized;
	private boolean isCallabackOverriden;

}
