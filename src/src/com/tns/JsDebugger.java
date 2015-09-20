package com.tns;

import java.io.BufferedReader;
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
import android.os.Bundle;

public class JsDebugger
{
	private static native void processDebugMessages();

	private static native void enable();

	private static native void disable();

	private static native void debugBreak();

	private static native void sendCommand(byte[] command, int length);

	private static ThreadScheduler threadScheduler;

	private static final int INVALID_PORT = -1;

	private static final String portEnvInputFile = "envDebug.in";
	
	private static final String portEnvOutputFile = "envDebug.out";

	private static final String DEBUG_BREAK_FILENAME = "debugbreak";

	private static int currentPort = INVALID_PORT;

	private static LinkedBlockingQueue<String> dbgMessages = new LinkedBlockingQueue<String>();

	private final File debuggerSetupDirectory;
	
	private Boolean shouldDebugBreakFlag = null;
	
	private final Logger logger;

	public JsDebugger(Logger logger, ThreadScheduler threadScheduler, File debuggerSetupDirectory)
	{
		this.logger = logger;
		JsDebugger.threadScheduler = threadScheduler;
		this.debuggerSetupDirectory = debuggerSetupDirectory;
	}

	private static ServerSocket serverSocket;
	private static ServerThread serverThread = null;
	private static Thread javaServerThread = null;

	private static class ServerThread implements Runnable
	{
		private volatile boolean running;
		private final int port;
		private ResponseWorker responseWorker;
		private ListenerWorker commThread;

		public ServerThread(int port)
		{
			this.port = port;
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

		//when someone runs our server we do:
		public void run()
		{
			try
			{
				//open server port to run on
				serverSocket = new ServerSocket(this.port);
				running = true;
			}
			catch (IOException e)
			{
				running = false;
				e.printStackTrace();
			}

			//start listening and responding through that socket
			while (running)
			{
				try
				{
					//wait for someone to connect to port and if he does ... open a socket
					Socket socket = serverSocket.accept();

					//out (send messages to node inspector)
					this.responseWorker = new ResponseWorker(socket);
					new Thread(this.responseWorker).start();

					//in (recieve messages from node inspector)
					commThread = new ListenerWorker(socket.getInputStream());
					new Thread(commThread).start();
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
			}
		}
	}

	private static class ListenerWorker implements Runnable
	{
		private enum State
		{
			Header, Message
		}

		private BufferedReader input;

		public ListenerWorker(InputStream inputStream)
		{
			this.input = new BufferedReader(new InputStreamReader(inputStream));
		}

		private volatile boolean running = true;

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
				while (running && ((line = (leftOver != null) ? leftOver : scanner.nextLine()) != null))
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
								leftOver = null;
						}
						break;
					case Message:
						if ((-1 < messageLength) && (messageLength <= line.length()))
						{
							String msg = line.substring(0, messageLength);
							if (messageLength < line.length())
								leftOver = line.substring(messageLength);
							state = State.Header;
							headers.clear();

							try
							{
								byte[] cmdBytes = msg.getBytes("UTF-16LE");
								int cmdLength = cmdBytes.length;
								sendCommand(cmdBytes, cmdLength);

								boolean success = JsDebugger.threadScheduler.post(dispatchProcessDebugMessages);
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
				scanner.close();
			}
		}
	}

	private static class ResponseWorker implements Runnable
	{
		private Socket socket;

		private final static String END_MSG = "#end#";

		private OutputStream output;

		public ResponseWorker(Socket clientSocket) throws IOException
		{
			this.socket = clientSocket;
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

	int getDebuggerPortFromEnvironment()
	{
		if (logger.isEnabled()) logger.write("getDebuggerPortFromEnvironment");
		int port = INVALID_PORT;
		
		File envOutFile = new File(debuggerSetupDirectory, portEnvOutputFile);
		OutputStreamWriter w = null;
		try
		{
			w = new OutputStreamWriter(new FileOutputStream(envOutFile, false));
			String currentPID = "PID=" + android.os.Process.myPid() + "\n";
			w.write(currentPID);
		}
		catch (IOException e1)
		{
			e1.printStackTrace();
		}
		finally
		{
			if (w != null)
			{
				try
				{
					w.close();
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
			}
			w = null;
		}
		
		boolean shouldDebugBreakFlag = shouldDebugBreak(); 
		
		if (logger.isEnabled()) logger.write("shouldDebugBreakFlag=" + shouldDebugBreakFlag);
		
		if (shouldDebugBreakFlag)
		{
			
			try
			{
				Thread.sleep(3 * 1000);
			}
			catch (InterruptedException e1)
			{
				e1.printStackTrace();
			}
		}

		File envInFile = new File(debuggerSetupDirectory, portEnvInputFile);
		
		boolean envInFileFlag = envInFile.exists();
		
		if (logger.isEnabled()) logger.write("envInFileFlag=" + envInFileFlag);
		
		if (envInFileFlag)
		{
			BufferedReader reader = null;
			try
			{
				reader = new BufferedReader(new FileReader(envInFile));
				String line = reader.readLine();
				int requestedPort;
				try
				{
					requestedPort = Integer.parseInt(line);
				}
				catch (NumberFormatException e)
				{
					requestedPort = INVALID_PORT;
				}

				w = new OutputStreamWriter(new FileOutputStream(envOutFile, true));
				int localPort = (requestedPort != INVALID_PORT) ? requestedPort : getAvailablePort();
				String strLocalPort = "PORT=" + localPort + "\n";
				w.write(strLocalPort);
				port = currentPort = localPort;
				//
				enable();
				debugBreak();
				serverThread = new ServerThread(port);
				javaServerThread = new Thread(serverThread);
				javaServerThread.start();
			}
			catch (IOException e1)
			{
				e1.printStackTrace();
			}
			finally
			{
				if (reader != null)
				{
					try
					{
						reader.close();
					}
					catch (IOException e)
					{
						e.printStackTrace();
					}
				}
				if (w != null)
				{
					try
					{
						w.close();
					}
					catch (IOException e)
					{
						e.printStackTrace();
					}
				}
				envInFile.delete();
			}
		}
		
		return port;
	}

	private static int getAvailablePort()
	{
		int port = 0;
		ServerSocket s = null;
		try
		{
			s = new ServerSocket(0);
			port = s.getLocalPort();
		}
		catch (IOException e)
		{
			port = INVALID_PORT;
		}
		finally
		{
			if (s != null)
			{
				try
				{
					s.close();
				}
				catch (IOException e)
				{
				}
			}
		}
		return port;
	}
	
	@RuntimeCallable
	private static void enqueueMessage(String message)
	{
		dbgMessages.add(message);
	}

	@RuntimeCallable
	private static void enableAgent(String packageName, int port, boolean waitForConnection)
	{
		enable();
		if (serverThread == null)
		{
			serverThread = new ServerThread(port);
		}
		javaServerThread = new Thread(serverThread);
		javaServerThread.start();
	}

	@RuntimeCallable
	private static void disableAgent()
	{
		disable();
		if (serverThread != null)
		{
			serverThread.stop();
		}
	}

	static void registerEnableDisableDebuggerReceiver(Context context)
	{
		String debugAction = context.getPackageName() + "-Debug";
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
						int port = bundle.getInt("debuggerPort", INVALID_PORT);
						if (port == INVALID_PORT)
						{
							if(currentPort == INVALID_PORT) {
								currentPort = getAvailablePort();
							}
							port = currentPort;
						}
						String packageName = bundle.getString("packageName", context.getPackageName());
						boolean waitForDebugger = bundle.getBoolean("waitForDebugger", false);
						JsDebugger.enableAgent(packageName, port, waitForDebugger);
						currentPort = port;
						this.setResultCode(currentPort);
					}
					else
					{
						// keep socket on the same port when we disable
						JsDebugger.disableAgent(); 
					}
				}
			}
		}, new IntentFilter(debugAction));
	}

	static void registerGetDebuggerPortReceiver(Context context)
	{
		String getDebuggerPortAction = context.getPackageName() + "-GetDbgPort";
		context.registerReceiver(new BroadcastReceiver()
		{
			@Override
			public void onReceive(Context context, Intent intent)
			{
				this.setResultCode(currentPort);
			}
		}, new IntentFilter(getDebuggerPortAction));
	}

	private boolean shouldDebugBreak()
	{
		if (shouldDebugBreakFlag != null)
		{
			return shouldDebugBreakFlag;
		}
		
		File debugBreakFile = new File(debuggerSetupDirectory, DEBUG_BREAK_FILENAME);
		
		shouldDebugBreakFlag = debugBreakFile.exists();
		
		if (shouldDebugBreakFlag)
		{
			debugBreakFile.delete();
		}
		
		return shouldDebugBreakFlag;
	}
}
