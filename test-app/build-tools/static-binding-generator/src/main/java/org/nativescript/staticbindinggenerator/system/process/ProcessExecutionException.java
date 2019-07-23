package org.nativescript.staticbindinggenerator.system.process;

public class ProcessExecutionException extends RuntimeException {

    private static final long serialVersionUID = 1L;

    public ProcessExecutionException(String message) {
        super(message);
    }

    public ProcessExecutionException(String message, Throwable origin) {
        super(message, origin);
    }

    public ProcessExecutionException(Throwable origin) {
        super(origin);
    }
}
