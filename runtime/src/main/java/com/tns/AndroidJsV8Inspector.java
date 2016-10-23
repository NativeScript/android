package com.tns;

import android.content.Context;
import android.util.Log;

import java.io.IOException;

import fi.iki.elonen.NanoHTTPD;
import fi.iki.elonen.NanoWSD;

public class AndroidJsV8Inspector
{
    private JsV8InspectorServer server;
    private Logger logger;
	private Context context;

    protected native final void init();
    protected native final void connect(Object connection);
    protected native final void disconnect();
    protected native final void dispatchMessage(String message);


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
            this.server.start(-1);

            init();
        }
    }

    private static void send(Object connection, String payload) throws IOException
    {
        ((JsV8InspectorWebSocket)connection).send(payload);
    }

    class JsV8InspectorServer extends NanoWSD
    {
        public JsV8InspectorServer(String name)
        {
            super(name);
        }

        @Override
        protected Response serveHttp(IHTTPSession session)
        {
            Log.d("{N}.v8-inspector", "http request for " + session.getUri());
            return super.serveHttp(session);
        }

        @Override
        protected WebSocket openWebSocket(IHTTPSession handshake)
        {
            return new JsV8InspectorWebSocket(handshake);
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
            Log.d("V8Inspector", "onOpen");
            connect(this);
        }

        @Override
        protected void onClose(NanoWSD.WebSocketFrame.CloseCode code, String reason, boolean initiatedByRemote)
        {
            Log.d("V8Inspector", "onClose");
            disconnect();
        }

        @Override
        protected void onMessage(NanoWSD.WebSocketFrame message)
        {
            Log.d("V8Inspector", "onMessage");
            Log.d("V8Inspector", "onMessage TextPayload" + message.getTextPayload());
            dispatchMessage(message.getTextPayload());
        }

        @Override
        public void send(String payload) throws IOException
        {
            Log.d("V8Inspector", "send " + payload);
            super.send(payload);
        }

        @Override
        protected void onPong(NanoWSD.WebSocketFrame pong)
        {
            Log.d("V8Inspector", "onPong");
        }

        @Override
        protected void onException(IOException exception)
        {
            exception.printStackTrace();
            disconnect();
        }
    }
}