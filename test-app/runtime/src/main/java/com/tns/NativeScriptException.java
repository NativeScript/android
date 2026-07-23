package com.tns;

@SuppressWarnings("serial")
public class NativeScriptException extends RuntimeException {
    @SuppressWarnings("unused")
    private long jsValueAddress = 0;
    private String incomingStackTrace;
    /*
     * Set by the native side when this exception is a branded
     * interop.escapeException(...) forward. An explicit forward always
     * reaches the Java caller - it is exempt from the (deprecated)
     * discardUncaughtJsExceptions handling.
     */
    private boolean escapedFromJs = false;

    boolean isEscapedFromJs() {
        return escapedFromJs;
    }

    public NativeScriptException() {
        super();
    }

    public NativeScriptException(String message) {
        super(message);
    }

    public NativeScriptException(Throwable throwable) {
        super(throwable);
    }

    public NativeScriptException(String message, Throwable throwable) {
        super(message, throwable);
    }

    public NativeScriptException(String message, String stackTrace, Throwable throwable) {
        super(message, throwable);
        this.incomingStackTrace = stackTrace;
    }

    public NativeScriptException(String message, String stackTrace, long jsValueAddress) {
        super(message);
        this.incomingStackTrace = stackTrace;
        this.jsValueAddress = jsValueAddress;
    }

    public String getIncomingStackTrace() {
        return incomingStackTrace;
    }

    @RuntimeCallable
    public static String getStackTraceAsString(Throwable ex) {
        String errMessage = "";
        try {
            for (StackTraceElement frame: ex.getStackTrace()) {
                errMessage += "\t";
                errMessage += frame + "\n";
            }

            Throwable cause = ex.getCause();
            if (cause != null) {
                errMessage += "\nCaused by: ";
                errMessage += getStackTraceAsString(cause);
            }
        } catch (Throwable err) {
            errMessage = "Unknown error. Cannot get error message.";
        }
        return errMessage;
    }

    @RuntimeCallable
    public static String getMessage(Throwable ex) {
        return ex.toString();
    }
}
