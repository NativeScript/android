package com.tns.tests;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.ByteArrayOutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

/**
 * Loopback HTTP/1.1 fixture server for the in-app Jasmine suite. Mirrors the
 * module-serving routes of the iOS TestRunnerTests ModuleTestServer so the
 * HTTP ESM loader specs can run identically on both platforms.
 */
public final class ModuleTestServer {
    private static final String JS_MIME = "application/javascript; charset=utf-8";
    private static final Charset UTF8 = Charset.forName("UTF-8");

    private static ServerSocket serverSocket;
    private static Thread acceptThread;
    private static int boundPort = -1;
    private static final List<Thread> workers = new ArrayList<Thread>();

    private ModuleTestServer() {
    }

    /**
     * Starts the server if it is not already running and returns the port it is
     * bound to on 127.0.0.1. Safe to call from any thread, any number of times.
     */
    public static synchronized int ensureStarted() {
        if (serverSocket != null && !serverSocket.isClosed()) {
            return boundPort;
        }
        try {
            serverSocket = new ServerSocket(0, 64, InetAddress.getByName("127.0.0.1"));
        } catch (IOException e) {
            throw new RuntimeException("ModuleTestServer failed to bind", e);
        }
        boundPort = serverSocket.getLocalPort();

        final ServerSocket listener = serverSocket;
        acceptThread = new Thread(new Runnable() {
            @Override
            public void run() {
                acceptLoop(listener);
            }
        }, "ModuleTestServer");
        acceptThread.setDaemon(true);
        acceptThread.start();
        return boundPort;
    }

    public static synchronized void stop() {
        if (serverSocket != null) {
            try {
                serverSocket.close();
            } catch (IOException ignored) {
            }
            serverSocket = null;
        }
        synchronized (workers) {
            for (Thread t : workers) {
                t.interrupt();
            }
            workers.clear();
        }
        acceptThread = null;
        boundPort = -1;
    }

    public static synchronized int getPort() {
        return boundPort;
    }

    private static void acceptLoop(ServerSocket listener) {
        while (!listener.isClosed()) {
            final Socket socket;
            try {
                socket = listener.accept();
            } catch (IOException e) {
                return;
            }
            // A thread per connection: the /esm/timeout.mjs route parks its own
            // thread for delayMs, and the module-graph walk fetches concurrently.
            Thread worker = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        handle(socket);
                    } catch (Throwable ignored) {
                    } finally {
                        try {
                            socket.close();
                        } catch (IOException ignored) {
                        }
                        synchronized (workers) {
                            workers.remove(Thread.currentThread());
                        }
                    }
                }
            }, "ModuleTestServer-conn");
            worker.setDaemon(true);
            synchronized (workers) {
                workers.add(worker);
            }
            worker.start();
        }
    }

    private static void handle(Socket socket) throws IOException {
        socket.setSoTimeout(30000);
        socket.setTcpNoDelay(true);

        String head = readHead(socket.getInputStream());
        if (head == null) {
            return;
        }
        int lineEnd = head.indexOf("\r\n");
        String requestLine = lineEnd < 0 ? head : head.substring(0, lineEnd);
        String[] parts = requestLine.split(" ");
        if (parts.length < 2) {
            respond(socket, "400 Bad Request", null, new byte[0]);
            return;
        }
        String method = parts[0];
        String target = parts[1];
        String path = target;
        String query = "";
        int q = target.indexOf('?');
        if (q >= 0) {
            path = target.substring(0, q);
            query = target.substring(q + 1);
        }

        if (!"GET".equals(method)) {
            respondNotFound(socket);
            return;
        }
        route(socket, path, query);
    }

    private static void route(Socket socket, String path, String query) throws IOException {
        if ("/esm/query.mjs".equals(path) || "/ns/m/query.mjs".equals(path)) {
            String body = "export const path = \"" + jsStringLiteral(path) + "\";\n"
                    + "export const query = \"" + jsStringLiteral(query) + "\";\n"
                    + "export const evaluatedAt = " + System.currentTimeMillis() + ";\n"
                    + "export default { path, query, evaluatedAt };";
            respond(socket, "200 OK", JS_MIME, body.getBytes(UTF8));
            return;
        }

        if ("/esm/html-fallback.mjs".equals(path)) {
            // The SPA-fallback shape: an unknown path answered with the index
            // document, 200 OK. The module loader must reject it on MIME rather
            // than hand HTML to the JS parser.
            String body = "<!doctype html>\n<html><body>index</body></html>\n";
            respond(socket, "200 OK", "text/html; charset=utf-8", body.getBytes(UTF8));
            return;
        }

        if ("/esm/data.json".equals(path)) {
            String body = "{\"kind\":\"json-module\",\"n\":41}";
            respond(socket, "200 OK", "application/json; charset=utf-8", body.getBytes(UTF8));
            return;
        }

        if ("/esm/empty.mjs".equals(path)) {
            respond(socket, "200 OK", JS_MIME, new byte[0]);
            return;
        }

        if ("/esm/no-mime.mjs".equals(path)) {
            // Content-Type is omitted deliberately: this route exercises the
            // loader's missing-MIME branch.
            respond(socket, "200 OK", null, "export const ok = true;\n".getBytes(UTF8));
            return;
        }

        if ("/esm/graph-leaf.mjs".equals(path)) {
            // `k` gives each importer its own module identity (the query is part
            // of the key when no canonicalization vocabulary is configured), so
            // several specs share this one route.
            String key = param(query, "k=");
            if (key == null) {
                key = "x";
            }
            String body = "const bucket = \"__nsMixedOrder\" + \"" + key + "\";\n"
                    + "(globalThis[bucket] = globalThis[bucket] || []).push(\"leaf\");\n"
                    + "export const name = \"" + key + "\";";
            respond(socket, "200 OK", JS_MIME, body.getBytes(UTF8));
            return;
        }

        if ("/esm/syntax-error.mjs".equals(path)) {
            // Deliberately unparseable: pins that the loader surfaces V8's real
            // compile error instead of a generic failure.
            respond(socket, "200 OK", JS_MIME, "export const ok = ;\n".getBytes(UTF8));
            return;
        }

        if ("/esm/timeout.mjs".equals(path)) {
            int delayMs = 12000;
            String raw = param(query, "delayMs=");
            if (raw != null) {
                try {
                    delayMs = Integer.parseInt(raw);
                } catch (NumberFormatException ignored) {
                }
            }
            try {
                Thread.sleep(delayMs);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                return;
            }
            String body = "export const evaluatedAt = " + System.currentTimeMillis()
                    + "; export default { evaluatedAt };";
            respond(socket, "200 OK", JS_MIME, body.getBytes(UTF8));
            return;
        }

        respondNotFound(socket);
    }

    private static void respondNotFound(Socket socket) throws IOException {
        respond(socket, "404 Not Found", "text/plain; charset=utf-8", "Not Found".getBytes(UTF8));
    }

    /** Reads bytes up to and including the CRLFCRLF header terminator. */
    private static String readHead(InputStream in) throws IOException {
        ByteArrayOutputStream buffer = new ByteArrayOutputStream();
        int matched = 0;
        while (matched < 4) {
            int b = in.read();
            if (b < 0) {
                return null;
            }
            buffer.write(b);
            char expected = (matched == 0 || matched == 2) ? '\r' : '\n';
            matched = (b == expected) ? matched + 1 : (b == '\r' ? 1 : 0);
            if (buffer.size() > 64 * 1024) {
                return null;
            }
        }
        byte[] bytes = buffer.toByteArray();
        return new String(bytes, 0, bytes.length - 4, UTF8);
    }

    /**
     * Value of the first `&`-separated query component starting with `prefix`,
     * undecoded. Unknown components (the loader appends cache-bust nonces) are
     * ignored.
     */
    private static String param(String query, String prefix) {
        if (query == null || query.length() == 0) {
            return null;
        }
        for (String pair : query.split("&")) {
            if (pair.startsWith(prefix)) {
                return pair.substring(prefix.length());
            }
        }
        return null;
    }

    private static String jsStringLiteral(String s) {
        return s.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r");
    }

    private static void respond(Socket socket, String status, String contentType, byte[] body)
            throws IOException {
        StringBuilder headers = new StringBuilder();
        headers.append("HTTP/1.1 ").append(status).append("\r\n");
        if (contentType != null) {
            headers.append("Content-Type: ").append(contentType).append("\r\n");
        }
        headers.append("Content-Length: ").append(body.length).append("\r\n");
        // One request per connection: the client is HttpURLConnection, which
        // would otherwise pool a socket this server never services again.
        headers.append("Connection: close\r\n\r\n");

        OutputStream out = socket.getOutputStream();
        out.write(headers.toString().getBytes(UTF8));
        out.write(body);
        out.flush();
    }
}
