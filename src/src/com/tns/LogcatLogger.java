package com.tns;

import android.util.Log;

public final class LogcatLogger implements Logger
{
	private boolean enabled;

	public LogcatLogger(boolean isEnabled)
	{
		this.enabled = isEnabled;
	}
	
	public final boolean isEnabled()
	{
		return enabled;
	}
	
	public final void setEnabled(boolean isEnabled)
	{
		enabled = isEnabled;
	}
	
	public final void write(String msg)
	{
		Log.d(DEFAULT_LOG_TAG, msg);
	}
	
	public final void write(String tag, String msg)
	{
		Log.d(tag, msg);
	}
	
	private final static String DEFAULT_LOG_TAG = "TNS.Java";
}
