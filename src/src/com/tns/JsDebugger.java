package com.tns;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.concurrent.LinkedBlockingQueue;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.os.Bundle;

public class JsDebugger
{
	public static boolean enableDebuggingOverride;


	private static native void processDebugMessages();

	private static native void enable();

	private static native void disable();

	private static native void debugBreak();

	private static native void sendCommand(byte[] command, int length);

	private ThreadScheduler threadScheduler;

	private LinkedBlockingQueue<String> dbgMessages = new LinkedBlockingQueue<String>();

	private Logger logger;
	private Context context;
	private DebugLocalServerSocketThread debugServerThread;
	private Thread debugJavaServerThread;

	
	public JsDebugger(Context context, Logger logger, ThreadScheduler threadScheduler)
	{
		this.context = context;
		this.logger = logger;
		this.threadScheduler = threadScheduler;
	}

	private class DebugLocalServerSocketThread implements Runnable
    {
        private volatile boolean running;
        private final String name;

        private ListenerWorker workerThread;
        private LocalServerSocket serverSocket;
		private ResponseWorker responseWorker;
		
        public DebugLocalServerSocketThread(String name)
        {
            this.name = name;
            this.running = false;
        }

        public void stop()
		{
			this.running = false;
			this.responseWorker.stop();
			try
			{
				serverSocket.close();
			}
			catch (IOException e)
			{
				e.printStackTrace();
			}
		}

        public void run()
        {
            running = true;
            try
            {
                serverSocket = new LocalServerSocket(this.name);
                while (running)
                {
                	try
    				{
                		LocalSocket socket = serverSocket.accept();
    
    					//out (send messages to node inspector)
    					this.responseWorker = new ResponseWorker(socket);
    					new Thread(this.responseWorker).start();
    
    					//in (recieve messages from node inspector)
    					workerThread = new ListenerWorker(socket);
    					new Thread(workerThread).start();
    				}
    				catch (IOException e)
    				{
    					e.printStackTrace();
    				}
                }
            }
            catch (IOException e)
            {
            	e.printStackTrace();
            }
        }
    }

	private enum State
	{
		Header, Message
	}
	
	private class ListenerWorker implements Runnable
	{
		private BufferedReader input;
		private LocalSocket socket;

		public ListenerWorker(LocalSocket socket) throws IOException
		{
			this.socket = socket;
			this.input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		}

		public void run()
		{
			Scanner scanner = new Scanner(this.input);
			scanner.useDelimiter("\r\n");

			ArrayList<String> headers = new ArrayList<String>();
			String line;
			State state = State.Header;
			int messageLength = -1;
			String leftOver = null;

			Runnable dispatchProcessDebugMessages = new Runnable()
			{
				@Override
				public void run()
				{
					processDebugMessages();
				}
			};

			try
			{
				while ((line = (leftOver != null) ? leftOver : scanner.nextLine()) != null)
				{
					switch (state)
					{
					case Header:
						if (line.length() == 0)
						{
							state = State.Message;
						}
						else
						{
							headers.add(line);
							if (line.startsWith("Content-Length:"))
							{
								String strLen = line.substring(15).trim();
								messageLength = Integer.parseInt(strLen);
							}
							
							if (leftOver != null)
							{
								leftOver = null;
							}
						}
						break;
						
					case Message:
						if ((-1 < messageLength) && (messageLength <= line.length()))
						{
							String msg = line.substring(0, messageLength);
							if (messageLength < line.length())
							{
								leftOver = line.substring(messageLength);
							}
							
							state = State.Header;
							headers.clear();

							try
							{
								byte[] cmdBytes = msg.getBytes("UTF-16LE");
								int cmdLength = cmdBytes.length;
								sendCommand(cmdBytes, cmdLength);

								boolean success = threadScheduler.post(dispatchProcessDebugMessages);
								assert success;
							}
							catch (UnsupportedEncodingException e)
							{
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
						}
						else
						{
							if (leftOver == null)
							{
								leftOver = line;
							}
							else
							{
								leftOver += line;
							}
						}
						break;
					}
				}
			}
			catch (NoSuchElementException e)
			{
				e.printStackTrace();
			}
			finally
			{
				try
				{
					socket.close();
				}
				catch (IOException ex)
				{
					ex.printStackTrace();
				}
				
				scanner.close();
			}
		}
	}

	private class ResponseWorker implements Runnable
	{
		private LocalSocket socket;

		private final static String END_MSG = "#end#";

		private OutputStream output;

		public ResponseWorker(LocalSocket socket) throws IOException
		{
			this.socket = socket;
			this.output = this.socket.getOutputStream();
		}

		public void stop()
		{
			dbgMessages.add(END_MSG);
		}

		@Override
		public void run()
		{
			byte[] LINE_END_BYTES = new byte[2];
			LINE_END_BYTES[0] = (byte) '\r';
			LINE_END_BYTES[1] = (byte) '\n';
			while (true)
			{
				try
				{
					String msg = dbgMessages.take();

					if (msg.equals(END_MSG))
						break;

					byte[] utf8;
					try
					{
						utf8 = msg.getBytes("UTF8");
					}
					catch (UnsupportedEncodingException e1)
					{
						utf8 = null;
						e1.printStackTrace();
					}

					if (utf8 != null)
					{
						try
						{
							String s = "Content-Length: " + utf8.length;
							byte[] arr = s.getBytes("UTF8");
							output.write(arr, 0, arr.length);
							output.write(LINE_END_BYTES, 0, LINE_END_BYTES.length);
							output.write(LINE_END_BYTES, 0, LINE_END_BYTES.length);
							output.write(utf8, 0, utf8.length);
							output.flush();
						}
						catch (IOException e)
						{
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				}
				catch (InterruptedException e)
				{
					e.printStackTrace();
				}
			}
		}
	}

	@RuntimeCallable
	private void enqueueMessage(String message)
	{
		dbgMessages.add(message);
	}

	@RuntimeCallable
	private void enableAgent(boolean debugBrake)
	{
		enable();
		
		if (debugBrake)
		{
			debugBreak();
		}

		
		debugServerThread = new DebugLocalServerSocketThread(context.getPackageName() + "-debug");
		debugJavaServerThread = new Thread(debugServerThread);
		debugJavaServerThread.start();
	}

	@RuntimeCallable
	private void disableAgent()
	{
		disable();

		if (debugServerThread != null)
		{
			debugServerThread.stop();
		}
	}

	void registerEnableDisableDebuggerReceiver()
	{
		String debugAction = context.getPackageName() + "-debug";
		context.registerReceiver(new BroadcastReceiver()
		{
			@Override
			public void onReceive(Context context, Intent intent)
			{
				Bundle bundle = intent.getExtras();
				if (bundle != null)
				{
					boolean enable = bundle.getBoolean("enable", false);
					if (enable)
					{
						boolean debugBrake = bundle.getBoolean("waitForDebugger", false);
						enableAgent(debugBrake);
					}
					else
					{
						disableAgent(); 
					}
				}
			}
		}, new IntentFilter(debugAction));
	}

	private boolean shouldDebugBreak()
	{
		String shouldDebugBreakFlag = Util.readSystemProperty(context.getPackageName() + "-debugbrake");
		return Util.isPositive(shouldDebugBreakFlag);
	}

	public void start()
	{
		enableAgent(shouldDebugBreak());
	}
	
	public static boolean isDebuggableApp(Context context)
	{
		if (enableDebuggingOverride)
		{
			return true;
		}
		
		int flags;
		try
		{
			flags = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).applicationInfo.flags;
		}
		catch (NameNotFoundException e)
		{
			flags = 0;
			e.printStackTrace();
		}

		boolean isDebuggableApp = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
		return isDebuggableApp;
	}
}
