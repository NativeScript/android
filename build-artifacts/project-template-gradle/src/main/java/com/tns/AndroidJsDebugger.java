package com.tns;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.Scanner;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.LocalServerSocket;
import android.net.LocalSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;

public class AndroidJsDebugger implements Debugger
{
	@Override
	public void onConnect(JsDebugger context)
	{
		this.debugContext = context;
	}

	public static boolean enableDebuggingOverride;

	private Logger logger;
	private Context context;
	private DebugLocalServerSocketThread debugServerThread;
	private JsDebugger debugContext;

	private final static  String STOP_MESSAGE = "STOP_MESSAGE";

	private byte[] LINE_END_BYTES;

	private HandlerThread handlerThread;

	public AndroidJsDebugger(Context context, Logger logger)
	{
		this.context = context;
		this.logger = logger;
		LINE_END_BYTES = new byte[2];
		LINE_END_BYTES[0] = (byte) '\r';
		LINE_END_BYTES[1] = (byte) '\n';
	}

	private class DebugLocalServerSocketThread extends Thread
    {
        private volatile boolean running;
        private final String name;

        private RequestHandler requestHandler;
        private LocalServerSocket serverSocket;
		private ResponseHandler responseHandler;
		
        public DebugLocalServerSocketThread(String name)
        {
            this.name = name;
            this.running = false;
        }

        public void stopRunning()
		{
			running = false;
			this.responseHandler.stop();
		}

        public void run()
        {
        	Closeable requestHandlerCloseable = new Closeable()
    		{
    			@Override
    			public void close() throws IOException
    			{
    				requestHandler.stop();
    			}
    		};
    		
    		Closeable responseHandlerCloseable = new Closeable()
    		{
    			@Override
    			public void close() throws IOException
    			{
    				responseHandler.stop();
    			}
    		};
        	
            running = true;
            try
            {
                serverSocket = new LocalServerSocket(this.name);
                try 
                {
	                while (running)
	                {
	                	try
	    				{
	                		LocalSocket socket = serverSocket.accept();

	                		logger.write("NativeScript Debugger new connection on: " + socket.getFileDescriptor().toString());
	                		
	    					//out (send messages to node inspector)
	    					this.responseHandler = new ResponseHandler(socket, requestHandlerCloseable);
	    					Thread responseThread = new Thread(this.responseHandler);
	    					responseThread.start();
	    
	    					//in (recieve messages from node inspector)
	    					requestHandler = new RequestHandler(socket, responseHandlerCloseable);
	    					Thread requestThread = new Thread(requestHandler);
	    					requestThread.start();
	    					
	    					requestThread.join();
	    					
	    					this.responseHandler.stop();
	    					socket.close();

                            AndroidJsDebugger.this.debugContext.dbgMessages.clear();
                            AndroidJsDebugger.this.debugContext.dbgMessages.addAll(AndroidJsDebugger.this.debugContext.compileMessages);
	    				}
	    				catch (IOException e)
	    				{
	    					e.printStackTrace();
	    				}
	                	catch (InterruptedException e) {
	                		e.printStackTrace();
	                	}
	                }
                }
                finally
                {
                	try
        			{
        				serverSocket.close();
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
	
	private class RequestHandler implements Runnable
	{
		private BufferedReader input;
		private Scanner scanner;
		private boolean stop;
		private Closeable responseHandlerCloseable;
		
		public RequestHandler(LocalSocket socket, Closeable responseHandlerCloseable) throws IOException
		{
			this.responseHandlerCloseable = responseHandlerCloseable;
			this.input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		}

		public void stop()
		{
			this.stop = true;
			this.scanner.close();
		}
		
		public void run()
		{
			scanner = new Scanner(this.input);
			scanner.useDelimiter("\r\n");

			ArrayList<String> headers = new ArrayList<String>();
			String line;
			State state = State.Header;
			int messageLength = -1;
			String leftOver = null;

			try
			{
				while (!stop && ((line = (leftOver != null) ? leftOver : scanner.nextLine()) != null))
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
							String message = line.substring(0, messageLength);
							if (messageLength < line.length())
							{
								leftOver = line.substring(messageLength);
							}
							
							state = State.Header;
							headers.clear();

							if (!message.equals("FLUSH BUFFERS")) {
								AndroidJsDebugger.this.debugContext.sendMessage(message);
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

				try
				{
					responseHandlerCloseable.close();
				}
				catch (IOException e1)
				{
					e1.printStackTrace();
				}
			}
			finally
			{
				this.stop = true;
				//logger.write("sending disconnect to v8 debugger");
				AndroidJsDebugger.this.debugContext.sendMessage("{\"seq\":0,\"type\":\"request\",\"command\":\"disconnect\"}");
				
				scanner.close();
			}
		}
	}

	private class ResponseHandler implements Runnable
	{
		private OutputStream output;
		private boolean stop;
		private Closeable requestHandlerCloseable;

		public ResponseHandler(LocalSocket socket, Closeable requestHandlerCloseable) throws IOException
		{
			this.requestHandlerCloseable = requestHandlerCloseable;
			this.output = socket.getOutputStream();
		}

		public void stop()
		{
			this.stop = true;
			AndroidJsDebugger.this.debugContext.dbgMessages.add(STOP_MESSAGE);
		}

		@Override
		public void run()
		{
			while (!stop)
			{
				try
				{
					String message = AndroidJsDebugger.this.debugContext.dbgMessages.take();
					if (message.equals(STOP_MESSAGE))
					{
						break;
					}
					
					//Log.d("TNS.JAVA.JsDebugger", "Sending message to inspector:" + message);
					
					this.sendMessageToInspector(message);
				}
				catch (InterruptedException e)
				{
					e.printStackTrace();
				}
			}
			
			try
			{
				this.output.close();
			}
			catch (IOException e)
			{
				e.printStackTrace();
			}
		}

		private void sendMessageToInspector(String msg)
		{
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
					output.write(arr);
					output.write(LINE_END_BYTES);
					output.write(LINE_END_BYTES);
					output.write(utf8);
					output.flush();
					
					//Log.d("TNS.JAVA.JsDebugger", "Sent message to inspector:" + msg);
				}
				catch (IOException e)
				{
					e.printStackTrace();
					
					try
					{
						requestHandlerCloseable.close();
					}
					catch (IOException e1)
					{
						e1.printStackTrace();
					}
				}
			}
		}
	}

	public void enableAgent()
	{
		logger.write("Enabling NativeScript Debugger Agent");

		if (debugServerThread == null)
		{
			debugServerThread = new DebugLocalServerSocketThread(context.getPackageName() + "-debug");
			debugServerThread.start();
		}
	}

	public void disableAgent()
	{
		logger.write("Disabling NativeScript Debugger Agent");
		String message = "{\"seq\":0,\"type\":\"request\",\"command\":\"disconnect\"}";
		
		AndroidJsDebugger.this.debugContext.sendMessage(message);
		
		debugServerThread.stopRunning();
        debugServerThread = null;
	}

	private void registerEnableDisableDebuggerReceiver(Handler handler)
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
						enableAgent();
					}
					else
					{
						disableAgent(); 
					}
				}
			}
		}, new IntentFilter(debugAction), null, handler);
	}
	
	private boolean getDebugBreakFlagAndClearIt()
	{
		File debugBreakFile = new File("/data/local/tmp", context.getPackageName() + "-debugbreak");
		if (debugBreakFile.exists() && !debugBreakFile.isDirectory() && debugBreakFile.length() == 0)
		{
			try
			{
				java.io.FileWriter fileWriter = new java.io.FileWriter(debugBreakFile);
				fileWriter.write("used");
				fileWriter.close();
			}
			catch (IOException e)
			{
				e.printStackTrace();
			}
			return true;
		}
		return false;
	}

    private void setDebuggerStartedFlag()
    {
        File debugBreakFile = new File("/data/local/tmp", context.getPackageName() + "-debugger-started");
        if (debugBreakFile.exists() && !debugBreakFile.isDirectory() && debugBreakFile.length() == 0)
        {
            try
            {
                java.io.FileWriter fileWriter = new java.io.FileWriter(debugBreakFile);
                fileWriter.write("started");
                fileWriter.close();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }
        }
    }

	public void start()
	{
		AndroidJsDebugger.this.debugContext.enableAgent();

		handlerThread = new HandlerThread("debugAgentBroadCastReceiverHandler");
		handlerThread.start();
		Handler handler = new Handler(handlerThread.getLooper());

		this.registerEnableDisableDebuggerReceiver(handler);

		boolean shouldDebugBreak = getDebugBreakFlagAndClearIt();
		if (shouldDebugBreak)
		{
			AndroidJsDebugger.this.debugContext.debugBreak();
		}

        setDebuggerStartedFlag();
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