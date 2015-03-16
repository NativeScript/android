package com.tns.com.tns.tests;

public class AbsClassImpl extends com.tns.tests.AbsClassImpl implements com.tns.NativeScriptHashCodeProvider
{
	public AbsClassImpl()
	{
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
	}
	
	protected String echoString(String s)
	{
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
		if (__echoString)
		{
			Object[] params = new Object[1];
			params[0] = s;
			return (String)com.tns.Platform.callJSMethod(this, "echoString", params);
		}
		else
		{
			return super.echoString(s);
		}
	}

	public int hashCode__super()
	{
		return super.hashCode();
	}

	public boolean equals__super(Object other)
	{
		return super.equals(other);
	}

	public void setNativeScriptOverrides(String[] overrides)
	{
		for (String name: overrides)
		{
			setNativeScriptOverride(name);
		}
	}

        private boolean __initialized;
	private boolean __echoString;
	@Override
	public void setNativeScriptOverride(String name)
	{
		if (name.equals("echoString"))
		{
			__echoString = true;
		}
	}
}