package com.tns.com.tns.tests;

public class DummyClassWithInit extends com.tns.tests.DummyClassWithInit implements com.tns.NativeScriptHashCodeProvider
{
	public DummyClassWithInit()
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
	
	public void init()
	{
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
		if (__ho1)
		{
			Object[] params = null;
			com.tns.Platform.callJSMethod(this, "init", false, params);
		}
		else
		{
			super.init();
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
		// Edit "if" statements when necessary
		for (String name: overrides)
		{
			if (name.equals("init"))
			{
				__ctorOverriden = true;
			}

			if (name.equals("init"))
			{
				__ho1 = true;
			}
		}
	}
	
	private boolean __initialized;
	private boolean __ctorOverriden;
	private boolean __ho1;

}
