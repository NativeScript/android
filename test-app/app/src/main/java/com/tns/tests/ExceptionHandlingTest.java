package com.tns.tests;

import com.tns.NativeScriptException;

import android.util.Log;


public class ExceptionHandlingTest
{
	private static class BadException extends Exception
	{
		public BadException() {}

		@Override
		public StackTraceElement[] getStackTrace() {
			throw new RuntimeException();
		}
	}

	public void triggerEvent1(String s, int n) throws Exception
	{
		this.onEvent1(s, n);
	}
	
	public void triggerEvent1WithCatchClause(String s, int n)
	{
		try
		{
			this.onEvent1(s, n);
		}
		catch (Throwable e)
		{
		}
	}
	
	public void onEvent1(String s, int n)
	{
		Log.d("TestApp", "ExceptionHandlingTest.onEvent1 is called");
	}
	
	public void onGetFile(String s, int n)
	{
		Log.d("TestApp", "ExceptionHandlingTest.onGetFile is called");
	}
	
	public Throwable getException(String s, int n)
	{
		Throwable ret = null;
		try
		{
			getExceptionRec(s, n);
		}
		catch (NativeScriptException e)
		{
			ret = e;
		}
		return ret;
	}

	public void getExceptionRec(String s, int n)
	{
		this.onGetFile(s, n);
	}

    public void throwException() throws BadException {
        throw new BadException();
    }
}
