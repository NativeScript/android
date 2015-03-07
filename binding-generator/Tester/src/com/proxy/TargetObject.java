package com.proxy;

import android.util.Log;

public class TargetObject
{
	public boolean overrideCalled = false;
	public Object arg;
	public Object arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17;
	
	public void methodWithObjectArg(Object a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithObjectArg");
	}
	
	public void methodWithStringArg(String a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithStringArg: " + a);
	}
	
	public void methodWithIntArg(int a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithIntArg: " + a);
	}
	
	public void methodWithBooleanArg(boolean a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithBooleanArg: " + a);
	}
	
	public void methodWithCharArg(char a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithCharArg: " + a);
	}
	
	public void methodWithLongArg(long a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithLongArg: " + a);
	}
	
	public void methodWithDoubleArg(double a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithDoubleArg: " + a);
	}
	
	public void methodWithFloatArg(float a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithFloatArg: " + a);
	}
	
	public void methodWithShortArg(short a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithShortArg: " + a);
	}
	
	public void methodWithByteArg(byte a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithByteArg: " + a);
	}
	
	public void methodWithByteArrayArg(byte[] a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodWithByteArrayArg: " + a);
	}
	
	
	public byte methodReturningByte(byte a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningByte: " + a);
		return a;
	}
	
	public char methodReturningChar(char a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningChar: " + a);
		return a;
	}
	
	public short methodReturningShort(short a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningShort: " + a);
		return a;
	}
	
	public int methodReturningInt(int a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningInt: " + a);
		return a;
	}
	
	public float methodReturningFloat(float a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningFloat: " + a);
		return a;
	}
	
	public double methodReturningDouble(double a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningDouble: " + a);
		return a;
	}
	
	public long methodReturningLong(long a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningLong: " + a);
		return a;
	}
	
	public String methodReturningString(String a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningString: " + a);
		return a;
	}
	
	public Object methodReturningObject(Object a)
	{
		arg = a;
		Log.d("Proxy.Tester", "methodReturningObject: " + a);
		return a;
	}
	
	public void methodWithLongAndDoubleArgs(Object arg0, long arg1, Object arg2, double arg3)
	{
		this.arg0 = arg0;
		this.arg1 = arg1;
		this.arg2 = arg2;
		this.arg3 = arg3;
		Log.d("Proxy.Tester", "Base methodWithLongAndDoubleArgs called");
	}
	
	
	
	public void methodWithAllTypesOfArgs(
			Object arg0, 
			int arg1, 
			Integer arg2, 
			char arg3, 
			Character arg4,
			byte arg5,
			Byte arg6,
			short arg7,
			Short arg8,
			boolean arg9,
			Boolean arg10,
			long arg11,
			Long arg12,
			float arg13,
			Float arg14,
			double arg15,
			Double arg16,
			String arg17) throws Exception
	{
		this.arg0 = arg0;
		this.arg1 = arg1;
		this.arg2 = arg2;
		this.arg3 = arg3;
		this.arg4 = arg4;
		this.arg5 = arg5;
		this.arg6 = arg6;
		this.arg7 = arg7;
		this.arg8 = arg8;
		this.arg9 = arg9;
		this.arg10 = arg10;
		this.arg11 = arg11;
		this.arg12 = arg12;
		this.arg13 = arg13;
		this.arg14 = arg14;
		this.arg15 = arg15;
		this.arg16 = arg16;
		this.arg17 = arg17;
		
		Log.d("Proxy.Tester", "methodWithAllTypesOfArgs: ");
	}
	
	public interface TargetInterface
	{
		void methodWithObjectArg(Object a);
	}
}
