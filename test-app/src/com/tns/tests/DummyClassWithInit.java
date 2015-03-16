package com.tns.tests;

import com.tns.Platform;

import android.util.Log;


public class DummyClassWithInit
{
	public void callInit()
	{
		init();
	}
	
	public void init()
	{
		Log.d(Platform.DEFAULT_LOG_TAG, "init method called");
	}
}
