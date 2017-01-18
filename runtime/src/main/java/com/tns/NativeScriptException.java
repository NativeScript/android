package com.tns;

public class NativeScriptException extends RuntimeException {
    @SuppressWarnings("unused")
    private long jsValueAddress = 0;

    public NativeScriptException() {
        super();
    }

    public NativeScriptException(String detailMessage) {
        super(detailMessage);
    }

    public NativeScriptException(Throwable throwable) {
        super(throwable);
    }

    public NativeScriptException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public NativeScriptException(String detailMessage, long jsValueAddress) {
        super(detailMessage);
        this.jsValueAddress = jsValueAddress;
    }

    @RuntimeCallable
    public static String getStackTraceAsString(Throwable ex) {
        String errMessage;
        try {
            errMessage = ex.toString();
            for (StackTraceElement frame: ex.getStackTrace()) {
                errMessage += "\n    ";
                errMessage += frame;
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
}
