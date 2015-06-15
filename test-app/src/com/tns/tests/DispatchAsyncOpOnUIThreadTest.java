package com.tns.tests;

public class DispatchAsyncOpOnUIThreadTest
{
	
	public void processDataAsync(final int[] data)
	{
		new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				for (int i=0; i<data.length; i++)
				{
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) { }
					processData(i, data[i]);
				}
			}
		}).start();
	}
	
	public void processData(int index, int data)
	{
		throw new UnsupportedOperationException("Method 'processData' should be overridden!");
	}
}
