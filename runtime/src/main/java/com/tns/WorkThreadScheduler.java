package com.tns;

import android.os.Handler;

public final class WorkThreadScheduler implements ThreadScheduler
{
	private final Handler threadHandler;

	public WorkThreadScheduler(Handler handler)
	{
		this.threadHandler = handler;
	}

	@Override
	public final boolean post(Runnable r)
	{
		return threadHandler.post(r);
	}

	@Override
	public final Thread getThread()
	{
		return threadHandler.getLooper().getThread();
	}
}
