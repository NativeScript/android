package com.tns;

import java.lang.Thread.UncaughtExceptionHandler;
import android.content.Context;

public class NativeScriptUncaughtExceptionHandler implements UncaughtExceptionHandler {
    private final Context context;

    private final UncaughtExceptionHandler defaultHandler;

    private final Logger logger;

    public NativeScriptUncaughtExceptionHandler(Logger logger, Context context) {
        this.logger = logger;
        this.context = context;
        defaultHandler = Thread.getDefaultUncaughtExceptionHandler();
    }

    @Override
    public void uncaughtException(Thread thread, Throwable ex) {
        String currentThreadMessage = String.format("An uncaught Exception occurred on \"%s\" thread.\n%s\n", thread.getName(), ex.getMessage());
        String stackTraceErrorMessage = Runtime.getStackTraceErrorMessage(ex);
        String errorMessage = String.format("%s\nStackTrace:\n%s", currentThreadMessage, stackTraceErrorMessage);

        if (Runtime.isInitialized()) {
            try {
                if (Util.isDebuggableApp(context)) {
                    System.err.println(errorMessage);
                }

                Runtime runtime = Runtime.getCurrentRuntime();

                if (runtime != null) {
                    runtime.passUncaughtExceptionToJs(ex, ex.getMessage(), stackTraceErrorMessage, Runtime.getJSStackTrace(ex));
                }
            } catch (Throwable t) {
                if (Util.isDebuggableApp(context)) {
                    t.printStackTrace();
                }
            }
        }

        if (logger.isEnabled()) {
            logger.write("Uncaught Exception Message=" + errorMessage);
        }

        boolean res = false;

        if (Util.isDebuggableApp(context)) {
            try {
                Class<?> ErrReport = null;
                java.lang.reflect.Method startActivity = null;

                ErrReport = Class.forName("com.tns.ErrorReport");

                startActivity = ErrReport.getDeclaredMethod("startActivity", Context.class, String.class);

                res = (Boolean) startActivity.invoke(null, context, errorMessage);
            } catch (Exception e) {
                android.util.Log.v("Error", errorMessage);
                if (Util.isDebuggableApp(context)) {
                    e.printStackTrace();
                };
                android.util.Log.v("Application Error", "ErrorActivity default implementation not found. Reinstall android platform to fix.");
            }
        }

        if (!res && defaultHandler != null) {
            defaultHandler.uncaughtException(thread, ex);
        }
    }
}
