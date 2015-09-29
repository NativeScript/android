package com.tns.tests;

import android.util.Log;


public class View
{

	public View()
	{
		Log.d("TestApp", "View instance created from javascript");
	}

	public interface OnClickListener
	{
		public void onClick(View view);
	}
}