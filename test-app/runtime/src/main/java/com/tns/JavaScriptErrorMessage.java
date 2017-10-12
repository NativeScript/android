package com.tns;

public class JavaScriptErrorMessage {
    private String message;
    private String stackTrace;
    private String filename;
    private int lineno;
    private String threadName;

    JavaScriptErrorMessage(String message, String stackTrace, String filename, int lineno) {
        this.message = message;
        this.filename = filename;
        this.stackTrace = stackTrace;
        this.lineno = lineno;
    }

    JavaScriptErrorMessage(String message, String stackTrace, String filename, int lineno, String threadName) {
        this(message, stackTrace, filename, lineno);
        this.threadName = threadName;
    }

    public String getMessage() {
        return message;
    }

    public String getStackTrace() {
        return stackTrace;
    }

    public String getFilename() {
        return filename;
    }

    public int getLineno() {
        return lineno;
    }

    public String getThreadName() {
        return threadName;
    }
}
