package com.tns.tests;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

public class Class1
{
	public static boolean IsCollected;
	
	public Class1()
	{
		IsCollected = false;
	}
	
	public String getMessage()
	{
		return "Hello from Class1 instance";
	}
	
	@Override
	protected void finalize()
	{
		IsCollected = true;
	}
	
	public interface Callback1
	{
		String getMessage();
	}
	
	public static class Class2
	{
		public static boolean printMessageWithDelay(final Callback1 cb, final long delay)
		{
			Handler mainThreadHandler = new Handler(Looper.getMainLooper());

			Runnable r = new Runnable()
			{
				@Override
				public void run()
				{
					String s = cb.getMessage();
					Log.d("TestApp", "printMessgeWithDelay=" + s);
				}
			};

			boolean success = mainThreadHandler.postDelayed(r, delay);

			return success;
		}
	}
}
