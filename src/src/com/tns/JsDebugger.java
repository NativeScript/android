package com.tns;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.ServerSocket;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
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
	
	private static final String portEnvFile = "envDebug.txt";
	
	public JsDebugger(Context context) {
		this.context = context;
	}
	
	int getDebuggerPortFromEnvironment() {
		int port = INVALID_PORT;
		
		if (BuildConfig.DEBUG) {
			File envFile = new File(context.getExternalFilesDir(null), portEnvFile);
			if (envFile.exists()) {
				BufferedWriter w = null;
				BufferedReader r = null;
				try
				{
					long len = envFile.length();
					int localPort;
					if (len == 0) {
						w = new BufferedWriter(new FileWriter(envFile));
						localPort = getAvailablePort();
						if (localPort != INVALID_PORT) {
							String strLocalPort = "" + localPort;
							w.write(strLocalPort);
						}
					} else {
						r = new BufferedReader(new FileReader(envFile));
						String strLocalPort = r.readLine();
						localPort = Integer.parseInt(strLocalPort);
					}
					port = localPort;
				} catch (FileNotFoundException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
				finally {
					if (r != null) {
						try {
							r.close();
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
					if (w != null) {
						try {
							w.close();
						} catch (IOException e) {
							e.printStackTrace();
						}
					}

				}
			}
		}
		return port;
	}

	private int getAvailablePort() {
		int port = 0;		
		ServerSocket s = null;
		try {
			s = new ServerSocket(0);
			port = s.getLocalPort();
		} catch (IOException e) {
			port = INVALID_PORT;
		} finally {
			if (s != null) {
				try {
					s.close();
				} catch (IOException e) {}
			}
		}
		return port;
	}
	
	static boolean enableAgent(String packageName, int port, boolean waitForConnection) {
		boolean success = enableAgentNative(packageName, port, waitForConnection);
		return success;
	}
	
	static void disableAgent() {
		disableAgentNative();	
	}
	
	static void registerEnableDisableDebuggerReceiver(Context context) {
		String debugAction = context.getPackageName() + "-Debug";
		context.registerReceiver(new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				Bundle b = intent.getExtras();
				if (b != null) {
					boolean enable = b.getBoolean("enable", false);
					if (enable) {
						int port = b.getInt("debuggerPort", JsDebugger.INVALID_PORT);
						String packageName = b.getString("packageName", null);
						boolean waitForDebugger = b.getBoolean("waitForDebugger", false); 
						boolean success = JsDebugger.enableAgent(packageName, port, waitForDebugger);
						if (!success) {
							Log.d("TNS.Java", "enableAgent = false");
						}
					} else {
						JsDebugger.disableAgent();
					}
				}
			}
		}, new IntentFilter(debugAction));
	}
	
	static void registerGetDebuggerPortReceiver(Context context) {
		String getDebuggerPortAction = context.getPackageName() + "-GetDbgPort";
		context.registerReceiver(new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				int port = getCurrentDebuggerPort();
				this.setResultCode(port);
			}
		}, new IntentFilter(getDebuggerPortAction));
	}
	
	private static final Runnable callProcessDebugMessages = new Runnable() {
		@Override
		public void run() {
			processDebugMessages();
		}
	};

	private static boolean dispatchMessagesDebugAgentCallback() {
		boolean success = mainThreadHandler.post(callProcessDebugMessages);
		return success;
	}
}
