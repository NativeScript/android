package com.tns.tests;

import java.io.IOException;

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
		Log.d("TestApp", "ExceptionHandlingTest.onEvent1 is called");
	}
	
	public void onGetFile(String s) throws IOException
	{
		Log.d("TestApp", "ExceptionHandlingTest.onGetFile is called");
	}

	public Throwable getException(String s)
	{
		Throwable ret = null;
		try
		{
			this.onGetFile(s);
		}
		catch (IOException e)
		{
			ret = e;
		}
		return ret;
	}
}
