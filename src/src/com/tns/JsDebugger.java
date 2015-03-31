package com.tns;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.util.Log;

public class JsDebugger
{
	private static native void processDebugMessages();

	private static native boolean enableAgentNative(String packageName, int port, boolean waitForConnection);

	private static native void disableAgentNative();

	private static native int getCurrentDebuggerPort();

	private final Context context;

	private static final Handler mainThreadHandler = new Handler(Looper.getMainLooper());

	static final int INVALID_PORT = -1;

	private static final String portEnvInputFile = "envDebug.in";

	private static final String portEnvOutputFile = "envDebug.out";

	private static final HandlerThread getDebuggerPortHandlerThread = new HandlerThread("getDebuggerPortHandlerThread");

	private static final Handler getDebuggerPortHandler;

	static
	{
		getDebuggerPortHandlerThread.start();
		Looper looper = getDebuggerPortHandlerThread.getLooper();
		getDebuggerPortHandler = new Handler(looper);
	}

	public JsDebugger(Context context)
	{
		this.context = context;
	}
	
	int getDebuggerPortFromEnvironment()
	{
		int port = INVALID_PORT;

		if (shouldEnableDebugging(context))
		{
			File baseDir = context.getExternalFilesDir(null);
			File envOutFile = new File(baseDir, portEnvOutputFile);
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
			
			try
			{
				Thread.sleep(3 * 1000);
			}
			catch (InterruptedException e1)
			{
				e1.printStackTrace();
			}
			
			File envInFile = new File(baseDir, portEnvInputFile);
			if (envInFile.exists())
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
					port = localPort;
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

	static boolean enableAgent(String packageName, int port, boolean waitForConnection)
	{
		boolean success = enableAgentNative(packageName, port, waitForConnection);
		return success;
	}

	static void disableAgent()
	{
		disableAgentNative();
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
							port = getAvailablePort();
						}
						String packageName = bundle.getString("packageName", context.getPackageName());
						boolean waitForDebugger = bundle.getBoolean("waitForDebugger", false);
						boolean success = JsDebugger.enableAgent(packageName, port, waitForDebugger);
						if (!success)
						{
							Log.d("TNS.Java", "enableAgent = false");
						}
						int resultCode = success ? port : INVALID_PORT;
						this.setResultCode(resultCode);
					}
					else
					{
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
				int port = getCurrentDebuggerPort();
				this.setResultCode(port);
			}
		}, new IntentFilter(getDebuggerPortAction), null, getDebuggerPortHandler);
	}

	private static final Runnable callProcessDebugMessages = new Runnable()
	{
		@Override
		public void run()
		{
			processDebugMessages();
		}
	};

	private static boolean dispatchMessagesDebugAgentCallback()
	{
		boolean success = mainThreadHandler.post(callProcessDebugMessages);
		return success;
	}
	
	public static boolean shouldEnableDebugging(Context context)
	{
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
		boolean shouldEnableDebugging = ((flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
		return shouldEnableDebugging;
	}
}
