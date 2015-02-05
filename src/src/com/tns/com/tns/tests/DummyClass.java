package com.tns.com.tns.tests;

import android.util.Log;

import com.tns.NativeScriptHashCodeProvider;
import com.tns.Platform;

public class DummyClass
{
	public static class MyInterface implements com.tns.tests.DummyClass.MyInterface, NativeScriptHashCodeProvider
	{
		public MyInterface()
		{
			Log.d(Platform.DEFAULT_LOG_TAG, "MyInterfaceImpl ctor");
		}
	
		public int echoInt(int i)
		{
			java.lang.Object[] params = { i };
			return (Integer) com.tns.Platform.callJSMethod(this, "echoInt", params);
		}
		
		public void doSomething()
		{
			java.lang.Object[] params = null;
			com.tns.Platform.callJSMethod(this, "doSomething", params);
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
		}
                @Override
		public void setNativeScriptOverride(String override)
		{
		}
	}

	public static class DummyDerivedClass extends com.tns.tests.DummyClass.DummyDerivedClass implements NativeScriptHashCodeProvider
	{
		private boolean __initialized;
		private boolean __ctorOverridden;
		private boolean __ho0;

		public DummyDerivedClass()
		{
			super();
			if (!__initialized) { 
				__initialized = true;
				com.tns.Platform.initInstance(this);
			}
			if (__ctorOverridden) {
				java.lang.Object[] params = null;
				com.tns.Platform.callJSMethod(this, "AccessibilityService", params);
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
		public String dummyMethod(com.tns.tests.DummyClass dummy)
		{
			if (!__initialized) { 
				__initialized = true;
				com.tns.Platform.initInstance(this);
			}
			if (__ho0) { 
				java.lang.Object[] params = new Object[1];
				params[0] = dummy;
				return (String)com.tns.Platform.callJSMethod(this, "dummyMethod", params);
			} else {
				return super.dummyMethod(dummy);
			}
		}

		@Override
		public void setNativeScriptOverride(String name)
		{
			if (name.equals("dummyMethod"))
			{
				__ho0 = true;
			}
			
		}
	}
}	
