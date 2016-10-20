package com.tns;

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

import fi.iki.elonen.NanoHTTPD;
import fi.iki.elonen.NanoWSD;

public class AndroidJsV8Inspector
{
    private JsV8InspectorServer server;
    private Logger logger;
	private Context context;


	public AndroidJsV8Inspector(Context context, Logger logger)
	{
		this.context = context;
		this.logger = logger;
	}

    public void start() throws IOException
    {
        if (this.server == null)
        {
            this.server = new JsV8InspectorServer(context.getPackageName() + "-inspectorServer");
            this.server.start();
        }
    }

    class JsV8InspectorServer extends NanoWSD
    {
        public JsV8InspectorServer(String name)
        {
            super(name);
        }

        @Override
        protected WebSocket openWebSocket(IHTTPSession handshake)
        {
            return null;
        }
    }

    class JsV8InspectorWebSocket extends NanoWSD.WebSocket
    {

        public JsV8InspectorWebSocket(NanoHTTPD.IHTTPSession handshakeRequest)
        {
            super(handshakeRequest);
        }

        @Override
        protected void onOpen()
        {

        }

        @Override
        protected void onClose(NanoWSD.WebSocketFrame.CloseCode code, String reason, boolean initiatedByRemote)
        {

        }

        @Override
        protected void onMessage(NanoWSD.WebSocketFrame message)
        {

        }

        @Override
        protected void onPong(NanoWSD.WebSocketFrame pong)
        {

        }

        @Override
        protected void onException(IOException exception)
        {

        }
    }


}