package com.tns.tests;

import android.util.Log;


public class DummyClassWithInit
{
	public void callInit()
	{
		init();
	}
	
	public void init()
	{
		Log.d("TestApp", "init method called");
	}
}
