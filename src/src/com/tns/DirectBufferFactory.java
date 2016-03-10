package com.tns;

import java.lang.ref.PhantomReference;
import java.lang.ref.ReferenceQueue;
import java.nio.ByteBuffer;
import java.util.HashSet;
import java.util.Set;

public class DirectBufferFactory
{
	private class DirectBufferWeakReference<T> extends PhantomReference<T>
	{
		public long address;

		public DirectBufferWeakReference(T ref, ReferenceQueue<T> queue, long address)
		{
			super(ref, queue);
			this.address = address;
		}
	}

	private class ReferenceQueueListener implements Runnable
	{
		public ReferenceQueueListener()
		{
		}

		@Override
		public void run()
		{
			while (true)
			{
				try
				{
					@SuppressWarnings("unchecked")
					DirectBufferWeakReference<ByteBuffer> ref = (DirectBufferWeakReference<ByteBuffer>) DirectBufferFactory.this.queue.remove();
					DirectBufferFactory.this.enqueueBuffers.remove(ref);
					freeByteBuffer(ref.address);
				}
				catch (Throwable t)
				{
					DirectBufferFactory.this.logger.write(t.getMessage());
				}
			}
		}
	}

	private final Logger logger;

	private final ReferenceQueue<ByteBuffer> queue;

	private final ReferenceQueueListener listener;

	private final Set<DirectBufferWeakReference<ByteBuffer>> enqueueBuffers;

	private final Thread workerThread;

	public DirectBufferFactory(Logger logger)
	{
		this.logger = logger;
		this.queue = new ReferenceQueue<ByteBuffer>();
		this.listener = new ReferenceQueueListener();
		this.enqueueBuffers = new HashSet<DirectBufferWeakReference<ByteBuffer>>();
		this.workerThread = new Thread(this.listener, "My DirectBuffer Clean Thread");
	}

	public void startListener()
	{
		this.workerThread.start();
	}

	public ByteBuffer create(int capacity)
	{
		long[] address = new long[1];
		ByteBuffer buffer = allocateByteBuffer(capacity, address);
		if (buffer != null)
		{
			DirectBufferWeakReference<ByteBuffer> ref = new DirectBufferWeakReference<ByteBuffer>(buffer, this.queue, address[0]);
			enqueueBuffers.add(ref);
		}
		return buffer;
	}

	private static native ByteBuffer allocateByteBuffer(int capacity, long[] address);

	private static native void freeByteBuffer(long address);
}
