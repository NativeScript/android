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
import java.util.concurrent.LinkedBlockingQueue;

import org.json.JSONException;
import org.json.JSONObject;

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

public class JsDebugger
{
	public static boolean enableDebuggingOverride;


	private static native void processDebugMessages();

	private static native void enable();

	private static native void disable();

	private static native void debugBreak();

	private static native void sendCommand(byte[] command, int length);
	
	private static native boolean isDebuggerActive();

	private ThreadScheduler threadScheduler;

	private LinkedBlockingQueue<String> dbgMessages = new LinkedBlockingQueue<String>();
	private LinkedBlockingQueue<String> compileMessages = new LinkedBlockingQueue<String>();
	
	private Logger logger;
	private Context context;
	private DebugLocalServerSocketThread debugServerThread;

	private final static  String STOP_MESSAGE = "STOP_MESSAGE";
	
	private byte[] LINE_END_BYTES;

	private HandlerThread handlerThread;
	
	public JsDebugger(Context context, Logger logger, ThreadScheduler threadScheduler)
	{
		this.context = context;
		this.logger = logger;
		this.threadScheduler = threadScheduler;
		
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

        public void stopResponseHandler()
		{
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
	    					
	    					dbgMessages.clear();
	                		dbgMessages.addAll(compileMessages);
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

		Runnable dispatchProcessDebugMessages = new Runnable()
		{
			@Override
			public void run()
			{
				processDebugMessages();
			}
		};

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
							String msg = line.substring(0, messageLength);
							if (messageLength < line.length())
							{
								leftOver = line.substring(messageLength);
							}
							
							state = State.Header;
							headers.clear();

							sendMessageToV8(msg);
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
				sendMessageToV8("{\"seq\":0,\"type\":\"request\",\"command\":\"disconnect\"}");
				scanner.close();
			}
		}

		private void sendMessageToV8(String message)
		{
			try
			{
				//Log.d("TNS.JAVA.JsDebugger", "Sending message to v8:" + message);
				
				byte[] cmdBytes = message.getBytes("UTF-16LE");
				int cmdLength = cmdBytes.length;
				sendCommand(cmdBytes, cmdLength);

				threadScheduler.post(dispatchProcessDebugMessages);
			}
			catch (UnsupportedEncodingException e)
			{
				e.printStackTrace();
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
			dbgMessages.add(STOP_MESSAGE);
		}

		@Override
		public void run()
		{
			while (!stop)
			{
				try
				{
					String message = dbgMessages.take();
					if (message.equals(STOP_MESSAGE))
					{
						break;
					}
					
					//Log.d("TNS.JAVA.JsDebugger", "Sending message to inspector:" + message);
					
					sendMessageToInspector(message);
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

	@RuntimeCallable
	private void enqueueMessage(String message)
	{
		//logger.write("Debug msg:" + message);
		
		dbgMessages.add(message);
		
		if (message.contains("type\":\"event") &&
			message.contains("event\":\"afterCompile") &&
			message.contains("success\":true"))
		{
			compileMessages.add(message);
		}
	}
	
	@RuntimeCallable
	private void enqueueConsoleMessage(String text, String level, int lineNumber, int columnNumber, String srcFileName)
	{
		//    			var consoleEvent = {
		//	    			"seq":0,
		//	    			"type":"event",
		//	    			"event":"messageAdded",
		//	    			"success":true,
		//	    			"body":
		//	    			{
		//	    				"message":
		//	    				{
		//	    			        "source":"console-api",
		//	    			        "type": "log",
		//	    			        "level": 'error',
		//	    			        "line": 0,
		//	    			        "column": 0,
		//	    			        "url": "",
		//	    			        "groupLevel": 7,
		//	    			        "repeatCount": 1,
		//	    			        "text": "My message"
		//	    			    }
		//	    			}
		//    			};

		try
		{
			JSONObject consoleMessage = new JSONObject();
			consoleMessage.put("seq", 0);			
			consoleMessage.put("type", "event");
			consoleMessage.put("event", "messageAdded");
			consoleMessage.put("success", true);
			

			JSONObject message = new JSONObject();
			message.put("source", "console-api");
			message.put("type", "log");
			message.put("level", level);
			message.put("line", lineNumber);
			message.put("column", columnNumber);
			message.put("url", srcFileName);
			message.put("groupLevel", 7);
			message.put("repeatCount", 1);
			message.put("text", text);
			
			JSONObject body = new JSONObject();
			body.put("message", message);
			
			consoleMessage.put("body", body);
			
			String sendingText = consoleMessage.toString();
			
			Log.d("TNS.JAVA.JsDebugger", "Sending console message to inspector:" + sendingText);
			
			dbgMessages.add(sendingText);
		}
		catch (JSONException e)
		{
			e.printStackTrace();
		}
	}

	@RuntimeCallable
	private void enableAgent()
	{
		logger.write("Enabling NativeScript Debugger Agent");
		enable();
	}

	@RuntimeCallable
	private void disableAgent()
	{
		logger.write("Disabling NativeScript Debugger Agent");
		disable();
		
		
		String message = "{\"seq\":0,\"type\":\"request\",\"command\":\"disconnect\"}";
		
		byte[] cmdBytes;
		try
		{
			cmdBytes = message.getBytes("UTF-16LE");
			int cmdLength = cmdBytes.length;
			sendCommand(cmdBytes, cmdLength);
		}
		catch (UnsupportedEncodingException e)
		{
			e.printStackTrace();
		}
		
		debugServerThread.stopResponseHandler();
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
				Log.e("TNS", "Debug break temp file can not be marked as used. Debug sessions may not work correctly. file: " + debugBreakFile.getAbsolutePath());
				e.printStackTrace();
			}
			
			return true;
		}
		
		return false;
	}

	public void start()
	{
		debugServerThread = new DebugLocalServerSocketThread(context.getPackageName() + "-debug");
		debugServerThread.start();
		
		
		handlerThread = new HandlerThread("debugAgentBroadCastReceiverHandler");
		handlerThread.start();
		Handler handler = new Handler(handlerThread.getLooper());
		
		registerEnableDisableDebuggerReceiver(handler);
		
		logger.write("Enabling Debugger Agent");
		enable();

		boolean shouldDebugBrake = getDebugBreakFlagAndClearIt();
		if (shouldDebugBrake)
		{
			debugBreak();
		}
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

	public static boolean isJsDebuggerActive()
	{
		return isDebuggerActive();
	}
}
