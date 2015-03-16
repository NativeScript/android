package com.tns.tests;

import com.tns.Platform;

import android.util.Log;


public class ExceptionHandlingTest
{
	public void triggerEvent1(String s) throws Exception
	{
		this.onEvent1(s);
	}
	
	public void triggerEvent1WithCatchClause(String s)
	{
		try
		{
			this.onEvent1(s);
		}
		catch (Throwable e)
		{
		}
	}
	
	public void onEvent1(String s)
	{
		Log.d(Platform.DEFAULT_LOG_TAG, "ExceptionHandlingTest.onEvent1 is called");
	}
}
