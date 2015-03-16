package com.tns.tests;

import com.tns.Platform;

import android.util.Log;


public class View
{

	public View()
	{
		Log.d(Platform.DEFAULT_LOG_TAG, "View instance created from javascript");
	}

	public interface OnClickListener
	{
		public void onClick(View view);
	}
}