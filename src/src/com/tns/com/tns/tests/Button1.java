package com.tns.com.tns.tests;

import com.tns.Platform;

import android.util.Log;


public class Button1 extends com.tns.tests.Button1 implements com.tns.NativeScriptHashCodeProvider
{
	public Button1()
	{
		super();
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
	
	public Button1(int value)
	{
		super(value);
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
		if (__ctorOverriden)
		{
			java.lang.Object[] params = new java.lang.Object[1];
			params[0] = value;
			com.tns.Platform.callJSMethod(this, "init", true, params);
		}
	}
	
	public String echo(String s) throws Throwable
	{
		if (__isEchoOverriden)
		{
			try
			{
				Object[] params = { s };
				return (java.lang.String) com.tns.Platform.callJSMethod(this, "echo", params);
			}
			catch (Throwable throwable)
			{
				Log.d(Platform.DEFAULT_LOG_TAG, "Button1.echo ex=" + throwable.getMessage());
				throw throwable;
			}
		}
		else
		{
			return super.echo(s);
		}
	}
	
	public void methodDummyClassAsObjectInArgs(Object arg)
	{
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
		com.tns.Platform.callJSMethod(this, "methodDummyClassAsObjectInArgs", arg);
	}

	public String getIMAGE_ID_PROP()
	{
		if (!__initialized)
		{
			__initialized = true;
			com.tns.Platform.initInstance(this);
		}
		if (__getIMAGE_ID_PROP)
		{
			Object[] params = null;
			return (String)com.tns.Platform.callJSMethod(this, "getIMAGE_ID_PROP", params);
		}
		else
		{
			return super.getIMAGE_ID_PROP();
		}
	}
	
	public java.lang.String toString()
	{
		if (__toString)
		{
			Log.w(Platform.DEFAULT_LOG_TAG, "Make sure Button1 you create in js has toString always overriden");
			return (java.lang.String) com.tns.Platform.callJSMethod(this, "toString", null);
		}
		else
		{
			return super.toString();
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
			if (name.equals("init"))
			{
				__ctorOverriden = true;
			}
			else if (name.equals("echoString"))
			{
				__getIMAGE_ID_PROP = true;
			}
			else if (name.equals("toString"))
			{
				__toString = true;
			}
			else if (name.equals("echo"))
			{
				__isEchoOverriden = true;
			}
		}
	}
	
	private boolean __initialized;
	private boolean __ctorOverriden;
	private boolean __getIMAGE_ID_PROP;
	private boolean __toString;
	private boolean __isEchoOverriden;
}
