package com.tns;

public interface ThreadScheduler
{
	boolean post(Runnable r);
	
	Thread getThread();
}
