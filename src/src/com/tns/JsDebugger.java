package com.tns;

import java.io.UnsupportedEncodingException;
import java.util.concurrent.LinkedBlockingQueue;

import org.json.JSONException;
import org.json.JSONObject;

public class JsDebugger
{
	public native void processDebugMessages();

	public native void enable();

	public native void disable();

	public native void debugBreak();

	public native void sendCommand(byte[] command, int length);
	
	public native static boolean isDebuggerActive();

	private final static  String STOP_MESSAGE = "STOP_MESSAGE";
	
	private byte[] LINE_END_BYTES;

	private final Debugger debugger;
	
	private final ThreadScheduler threadScheduler;
	
	private final Runnable dispatchProcessDebugMessages = new Runnable()
	{
		@Override
		public void run()
		{
			processDebugMessages();
		}
	};
	
	public LinkedBlockingQueue<String> dbgMessages = new LinkedBlockingQueue<String>();
	public LinkedBlockingQueue<String> compileMessages = new LinkedBlockingQueue<String>();
	
	public JsDebugger(Debugger debugger, ThreadScheduler threadScheduler)
	{
		this.debugger = debugger;
		this.threadScheduler = threadScheduler;
		
		this.debugger.onConnect(this);
	}

	public void sendMessage(String message)
	{
		byte[] cmdBytes = null;
		
		try
		{
			cmdBytes = message.getBytes("UTF-16LE");
		}
		catch (UnsupportedEncodingException e)
		{
			e.printStackTrace();
		}
		int cmdLength = cmdBytes.length;
		
		sendCommand(cmdBytes, cmdLength);
		
		threadScheduler.post(dispatchProcessDebugMessages);
	}
	
	@RuntimeCallable
	public void enqueueMessage(String message)
	{
		dbgMessages.add(message);
		
		if (message.contains("type\":\"event") &&
			message.contains("event\":\"afterCompile") &&
			message.contains("success\":true"))
		{
			compileMessages.add(message);
		}
	}
	
	@RuntimeCallable
	public void enqueueConsoleMessage(String text, String level, int lineNumber, int columnNumber, String srcFileName)
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
			
			//logger.write("Sending console message to inspector:" + sendingText);
			
			dbgMessages.add(sendingText);
		}
		catch (JSONException e)
		{
			e.printStackTrace();
		}
	}

	@RuntimeCallable
	public void enableAgent()
	{
		enable();
		
		this.debugger.enableAgent();
	}

	@RuntimeCallable
	public void disableAgent()
	{
		disable();
		
		this.debugger.disableAgent();
	}

	public void start()
	{
		this.debugger.start();
	}
	
	public static boolean isJsDebuggerActive()
	{
		return isDebuggerActive();
	}
}
