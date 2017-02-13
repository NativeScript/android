package com.tns;

import android.content.Context;
import android.os.Handler;
import android.util.Log;
import android.util.Pair;
import android.webkit.MimeTypeMap;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

import fi.iki.elonen.NanoHTTPD;
import fi.iki.elonen.NanoWSD;

class AndroidJsV8Inspector {
    private static boolean DEBUG_LOG_ENABLED = false;

    private JsV8InspectorServer server;
    private Context context;
    private static String applicationDir;

    protected native final void init();

    protected native final void connect(Object connection);

    protected static native void disconnect();

    protected native final void dispatchMessage(String message);

    private Handler mainHandler;
    private LinkedBlockingQueue<String> inspectorMessages = new LinkedBlockingQueue<String>();

    AndroidJsV8Inspector(Context context, Logger logger) {
        this.context = context;
        applicationDir = context.getFilesDir().getAbsolutePath();
    }

    public void start() throws IOException {
        if (this.server == null) {
            Runtime currentRuntime = Runtime.getCurrentRuntime();

            mainHandler = currentRuntime.getHandler();

            this.server = new JsV8InspectorServer(this.context.getPackageName() + "-inspectorServer");
            this.server.start(-1);

            if (DEBUG_LOG_ENABLED) {
                Log.d("V8Inspector", "start debugger ThreadId:" + Thread.currentThread().getId());
            }

            init();
        }
    }

    @RuntimeCallable
    private static void sendToDevToolsConsole(Object connection, String message, String level) {
        try {
            JSONObject consoleMessage = new JSONObject();

            JSONObject params = new JSONObject();
            params.put("type", level);
            params.put("executionContextId", 0);
            params.put("timestamp", 0.000000000000000);

            JSONArray args = new JSONArray();
            args.put(message);
            params.put("args", args);

            consoleMessage.put("method", "Runtime.consoleAPICalled");
            consoleMessage.put("params", params);

            String sendingText = consoleMessage.toString();
            AndroidJsV8Inspector.send(connection, sendingText);

        } catch (JSONException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @RuntimeCallable
    private static void send(Object connection, String payload) throws IOException {
        ((JsV8InspectorWebSocket) connection).send(payload);
    }

    @RuntimeCallable
    private static String getInspectorMessage(Object connection) {
        return ((JsV8InspectorWebSocket) connection).getInspectorMessage();
    }

    @RuntimeCallable
    public static Pair<String, String>[] getPageResources() {
        // necessary to align the data dir returned by context (emulator) and that used by the v8 inspector
        if (applicationDir.startsWith("/data/user/0/")) {
            applicationDir = applicationDir.replaceFirst("/data/user/0/", "/data/data/");
        }

        String dataDir = applicationDir;
        File rootFilesDir = new File(dataDir, "app");


        List<Pair<String, String>> resources = traverseResources(rootFilesDir);

        return resources.toArray((Pair<String, String>[]) Array.newInstance(resources.get(0).getClass(), resources.size()));
    }

    private static List<Pair<String, String>> traverseResources(File dir) {
        List<Pair<String, String>> resources = new ArrayList<>();

        Queue<File> directories = new LinkedList<>();
        directories.add(dir);

        while (!directories.isEmpty()) {
            File currentDir = directories.poll();

            File[] files = currentDir.listFiles();
            for (File file : files) {
                if (file.isDirectory()) {
                    directories.add(file);
                } else {
                    resources.add(new Pair<>("file://" + file.getAbsolutePath(), getMimeType(file.getAbsolutePath())));
                }
            }
        }

        return resources;
    }

    private static String getMimeType(String url) {
        String type = null;
        String extension = MimeTypeMap.getFileExtensionFromUrl(url);
        if (extension != null) {
            type = MimeTypeMap.getSingleton().getMimeTypeFromExtension(extension);

            // getMimeType may sometime return incorrect results in the context of NativeScript
            // e.g. `.ts` returns video/MP2TS
            switch (extension) {
            case "js":
                type = "text/javascript";
                break;
            case "json":
                type = "application/json";
                break;
            case "css":
                type = "text/css";
                break;
            case "ts":
                type = "text/typescript";
                break;
            // handle shared libraries so they are marked properly and don't appear in the sources tab
            case "so":
                type = "application/binary";
                break;
            }
        }

        return type;
    }

    class JsV8InspectorServer extends NanoWSD {
        public JsV8InspectorServer(String name) {
            super(name);
        }

        @Override
        protected Response serveHttp(IHTTPSession session) {
            if (DEBUG_LOG_ENABLED) {
                Log.d("{N}.v8-inspector", "http request for " + session.getUri());
            }
            return super.serveHttp(session);
        }

        @Override
        protected WebSocket openWebSocket(IHTTPSession handshake) {
            return new JsV8InspectorWebSocket(handshake);
        }
    }

    class JsV8InspectorWebSocket extends NanoWSD.WebSocket {

        public JsV8InspectorWebSocket(NanoHTTPD.IHTTPSession handshakeRequest) {
            super(handshakeRequest);
        }

        @Override
        protected void onOpen() {
            if (DEBUG_LOG_ENABLED) {
                Log.d("V8Inspector", "onOpen: ThreadID:  " + Thread.currentThread().getId());
            }

            mainHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (DEBUG_LOG_ENABLED) {
                        Log.d("V8Inspector", "Connecting. threadID :  " + Thread.currentThread().getId());
                    }

                    connect(JsV8InspectorWebSocket.this);
                }
            });
        }

        @Override
        protected void onClose(NanoWSD.WebSocketFrame.CloseCode code, String reason, boolean initiatedByRemote) {
            if (DEBUG_LOG_ENABLED) {
                Log.d("V8Inspector", "onClose");
            }

            mainHandler.post(new Runnable() {
                @Override
                public void run() {
                    if (DEBUG_LOG_ENABLED) {
                        Log.d("V8Inspector", "Disconnecting");
                    }
                    disconnect();
                }
            });
        }

        @Override
        protected void onMessage(final NanoWSD.WebSocketFrame message) {
            if (DEBUG_LOG_ENABLED) {
                Log.d("V8Inspector", "To dbg backend: " + message.getTextPayload() + " ThreadId:" + Thread.currentThread().getId());
            }

            inspectorMessages.offer(message.getTextPayload());

            mainHandler.post(new Runnable() {
                @Override
                public void run() {
                    String nextMessage = inspectorMessages.poll();
                    while (nextMessage != null) {
                        dispatchMessage(nextMessage);
                        nextMessage = inspectorMessages.poll();
                    }
                }
            });
        }

        @Override
        public void send(String payload) throws IOException {
            if (DEBUG_LOG_ENABLED) {
                Log.d("V8Inspector", "To dbg client: " + payload);
            }

            super.send(payload);
        }

        public String getInspectorMessage() {
            try {
                String message = inspectorMessages.take();
                return message;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            return null;
        }

        @Override
        protected void onPong(NanoWSD.WebSocketFrame pong) {
        }

        @Override
        protected void onException(IOException exception) {
            exception.printStackTrace();
            disconnect();
        }
    }
}
