package com.tns;

import java.lang.Thread.UncaughtExceptionHandler;

import android.content.Context;

public class NativeScriptUncaughtExceptionHandler implements UncaughtExceptionHandler {
    private final Context context;

    private final UncaughtExceptionHandler defaultHandler;

    public NativeScriptUncaughtExceptionHandler(Context context) {
        this.context = context;
        defaultHandler = Thread.getDefaultUncaughtExceptionHandler();
    }

    @Override
    public void uncaughtException(Thread thread, Throwable ex) {
        String currentThreadMessage = "An uncaught Exception occurred on \"" + thread.getName() + "\" thread.\n";

        String errorMessage = currentThreadMessage + ErrorReport.getErrorMessage(ex);

        if (Runtime.isInitialized()) {
            try {
                ex.printStackTrace();

                Runtime runtime = Runtime.getCurrentRuntime();

                if (runtime != null) {
                    runtime.passUncaughtExceptionToJs(ex, errorMessage);
                }

                if (JsDebugger.isJsDebuggerActive()) {
                    return;
                }
            } catch (Throwable t) {
                t.printStackTrace();
            }
        }

        DefaultTracer.trace(Tracer.Descriptor.EXCEPTION, "Uncaught Exception Message=" + errorMessage);

        if (!ErrorReport.startActivity(context, errorMessage) && defaultHandler != null) {
            defaultHandler.uncaughtException(thread, ex);
        }
    }
}
