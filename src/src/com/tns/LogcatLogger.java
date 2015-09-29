package com.tns;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;

import android.content.Context;
import android.util.Log;

public final class LogcatLogger implements Logger
{
	private boolean enabled;

	public LogcatLogger(boolean isEnabled, Context context)
	{
		this.enabled = isEnabled;
		
		if (!isEnabled)
		{
			this.initLogging(context);
		}
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
	
    private void initLogging(Context context)
    {
		boolean isDebuggableApp = Util.isDebuggableApp(context);
		
		if (isDebuggableApp)
		{
	    	String verboseLoggingProp = readSystemProperty("nativescript.verbose.logging");
	    	
	    	if (verboseLoggingProp.equals("true") || verboseLoggingProp.equals("TRUE") ||
	    		verboseLoggingProp.equals("yes") || verboseLoggingProp.equals("YES") ||
	    		verboseLoggingProp.equals("enabled") || verboseLoggingProp.equals("ENABLED"))
	    	{
	    		setEnabled(true);
	    	}
		}
    }
    
	private String readSystemProperty(String name)
	{
		InputStreamReader in = null;
		BufferedReader reader = null;
		try
		{
			Process proc = Runtime.getRuntime().exec(new String[] { "/system/bin/getprop", name });
			in = new InputStreamReader(proc.getInputStream());
			reader = new BufferedReader(in);
			return reader.readLine();
		}
		catch (IOException e)
		{
			return null;
		}
		finally
		{
			silentClose(in);
			silentClose(reader);
		}
	}
	
	private void silentClose(Closeable closeable)
	{
		if (closeable == null)
		{
			return;
		}
		try
		{
			closeable.close();
		}
		catch (IOException ignored)
		{
		}
	}

	private final static String DEFAULT_LOG_TAG = "TNS.Java";
}
