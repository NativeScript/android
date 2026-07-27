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
        // An uncaughtErrorPolicy: "throw" exception was already fully reported
        // to JS (event + hook + log) at the throw decision point - reporting
        // it again here would double-dispatch the same failure. Checked first
        // so the already-reported path does no work at all: no JS roundtrip
        // and no eager stack rendering (the message strings below are built
        // lazily, only when something actually consumes them).
        boolean alreadyReportedToJs = ex instanceof NativeScriptException && ((NativeScriptException) ex).isReportedToJs();

        String errorMessage = null;
        boolean handledByJs = false;

        if (!alreadyReportedToJs) {
            // Resolve the reporting runtime FIRST: Runtime.isInitialized() is
            // thread-local, so gating on it would silently skip reporting for
            // crashes on threads with no runtime of their own. Those fall back
            // to the main runtime's isolate (the JNI layer takes the
            // v8::Locker, so entering it cross-thread is safe).
            Runtime runtime = Runtime.getCurrentRuntime();
            if (runtime == null) {
                runtime = Runtime.getMainRuntime();
            }

            if (runtime != null && runtime.isInitializedImpl()) {
                try {
                    String stackTraceErrorMessage = Runtime.getStackTraceErrorMessage(ex);
                    errorMessage = buildErrorMessage(thread, ex, stackTraceErrorMessage);

                    if (Util.isDebuggableApp(context)) {
                        System.err.println(errorMessage);
                    }

                    handledByJs = runtime.passUncaughtExceptionToJs(ex, ex.getMessage(), stackTraceErrorMessage, Runtime.getJSStackTrace(ex));
                } catch (Throwable t) {
                    if (Util.isDebuggableApp(context)) {
                        t.printStackTrace();
                    }
                }
            }
        }

        if (logger.isEnabled()) {
            if (errorMessage == null) {
                errorMessage = buildErrorMessage(thread, ex, Runtime.getStackTraceErrorMessage(ex));
            }
            logger.write("Uncaught Exception Message=" + errorMessage);
        }

        if (handledByJs) {
            // A JS `nativeuncaughterror` listener called preventDefault() -
            // the exception is fully handled: no error activity, no crash.
            return;
        }

        boolean res = false;

        if (Util.isDebuggableApp(context)) {
            if (errorMessage == null) {
                errorMessage = buildErrorMessage(thread, ex, Runtime.getStackTraceErrorMessage(ex));
            }
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

    private static String buildErrorMessage(Thread thread, Throwable ex, String stackTraceErrorMessage) {
        String currentThreadMessage = String.format("An uncaught Exception occurred on \"%s\" thread.\n%s\n", thread.getName(), ex.getMessage());
        return String.format("%s\nStackTrace:\n%s", currentThreadMessage, stackTraceErrorMessage);
    }
}
