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
    public static final String DISCONNECT_MESSAGE = "nativescript-inspector-disconnect";
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

            Log.d("V8Inspector", "init ThreadId:" + Thread.currentThread().getId());

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
            Log.d("V8Inspector", "onOpen: ThreadID:  " + Thread.currentThread().getId());

            final Object waitObject = new Object();

            mainHandler.post(new Runnable()
            {
                @Override
                public void run()
                {
                    try
                    {
                        Log.d("V8Inspector", "onOpen: runnable ThreadID :  " + Thread.currentThread().getId());
                        connect(JsV8InspectorWebSocket.this);
                    }
                    finally
                    {
                        synchronized (waitObject)
                        {
                            waitObject.notify();
                        }
                    }
                }
            });

            try
            {
                synchronized (waitObject)
                {
                    waitObject.wait();
                }
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }

        @Override
        protected void onClose(NanoWSD.WebSocketFrame.CloseCode code, String reason, boolean initiatedByRemote)
        {
            Log.d("V8Inspector", "onClose");
            mainHandler.post(new Runnable()
            {
                @Override
                public void run()
                {
                    disconnect();
                }
            });

            inspectorMessages.offer(DISCONNECT_MESSAGE);
        }

        @Override
        protected void onMessage(final NanoWSD.WebSocketFrame message)
        {
            Log.d("V8Inspector", "onMessage");
            Log.d("V8Inspector", "onMessage TextPayload" + message.getTextPayload() + " ThreadId:" + Thread.currentThread().getId());
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

//            mainHandler.post(new Runnable()
//            {
//                @Override
//                public void run()
//                {
//
//                    dispatchMessage(message.getTextPayload());
//                }
//            });
        }

        @Override
        public void send(String payload) throws IOException
        {
            Log.d("V8Inspector", "send " + payload);
            super.send(payload);
        }

        public String getInspectorMessage()
        {
            try
            {
                String message = inspectorMessages.take();

                if (message != null && message.equalsIgnoreCase(DISCONNECT_MESSAGE))
                {
                    disconnect();
                    return null;
                }

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