package com.tns;

import android.os.Handler;

public class MainThreadScheduler implements ThreadScheduler
{
	private final Handler threadHandler;
	
	public MainThreadScheduler(Handler threadHandler)
	{
		this.threadHandler = threadHandler;
	}

	@Override
	public boolean post(Runnable r)
	{
		return threadHandler.post(r);
	}
}
