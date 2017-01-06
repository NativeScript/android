package com.tns;

import android.content.Context;
import android.os.Handler;
import android.util.Log;

import java.io.IOException;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

import fi.iki.elonen.NanoHTTPD;
import fi.iki.elonen.NanoWSD;

public class AndroidJsV8Inspector
{
    private static boolean DEBUG_LOG_ENABLED = false;

    private JsV8InspectorServer server;
    private Logger logger;
    private Context context;

    protected native final void init();

    protected native final void connect(Object connection);

    protected static native final void disconnect();

    protected native final void dispatchMessage(String message);
    protected Handler mainHandler;
    private LinkedBlockingQueue<String> inspectorMessages = new LinkedBlockingQueue<String>();

    public AndroidJsV8Inspector(Context context, Logger logger)
    {
        this.context = context;
        this.logger = logger;
    }

    public void start() throws IOException
    {
        if (this.server == null)
        {
            Runtime currentRuntime = Runtime.getCurrentRuntime();

            mainHandler = currentRuntime.getHandler();

            this.server = new JsV8InspectorServer(context.getPackageName() + "-inspectorServer");
            this.server.start(-1);

            if (DEBUG_LOG_ENABLED)
            {
                Log.d("V8Inspector", "start debugger ThreadId:" + Thread.currentThread().getId());
            }

            init();
        }
    }

    private static void send(Object connection, String payload) throws IOException
    {
        ((JsV8InspectorWebSocket) connection).send(payload);
    }

    private static String getInspectorMessage(Object connection)
    {
        return ((JsV8InspectorWebSocket) connection).getInspectorMessage();
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
            if (DEBUG_LOG_ENABLED)
            {
                Log.d("{N}.v8-inspector", "http request for " + session.getUri());
            }
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
            if (DEBUG_LOG_ENABLED)
            {
                Log.d("V8Inspector", "onOpen: ThreadID:  " + Thread.currentThread().getId());
            }

            mainHandler.post(new Runnable()
            {
                @Override
                public void run()
                {
                    if (DEBUG_LOG_ENABLED)
                    {
                        Log.d("V8Inspector", "Connecting. threadID :  " + Thread.currentThread().getId());
                    }

                    connect(JsV8InspectorWebSocket.this);
                }
            });
        }

        @Override
        protected void onClose(NanoWSD.WebSocketFrame.CloseCode code, String reason, boolean initiatedByRemote)
        {
            if (DEBUG_LOG_ENABLED)
            {
                Log.d("V8Inspector", "onClose");
            }

            mainHandler.post(new Runnable()
            {
                @Override
                public void run()
                {
                    if (DEBUG_LOG_ENABLED)
                    {
                        Log.d("V8Inspector", "Disconnecting");
                    }
                    disconnect();
                }
            });
        }

        @Override
        protected void onMessage(final NanoWSD.WebSocketFrame message)
        {
            if (DEBUG_LOG_ENABLED)
            {
                Log.d("V8Inspector", "To dbg backend: " + message.getTextPayload() + " ThreadId:" + Thread.currentThread().getId());
            }

            inspectorMessages.offer(message.getTextPayload());

            mainHandler.post(new Runnable()
            {
                @Override
                public void run()
                {
                    String nextMessage = inspectorMessages.poll();
                    while (nextMessage != null)
                    {
                        dispatchMessage(nextMessage);
                        nextMessage = inspectorMessages.poll();
                    }
                }
            });
        }

        @Override
        public void send(String payload) throws IOException
        {
            if (DEBUG_LOG_ENABLED)
            {
                Log.d("V8Inspector", "To dbg client: " + payload);
            }

            super.send(payload);
        }

        public String getInspectorMessage()
        {
            try
            {
                String message = inspectorMessages.take();
                return message;
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }

            return null;
        }

        @Override
        protected void onPong(NanoWSD.WebSocketFrame pong)
        {
        }

        @Override
        protected void onException(IOException exception)
        {
            exception.printStackTrace();
            disconnect();
        }
    }
}