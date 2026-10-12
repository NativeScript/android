package android.util;

/**
 * Stub for compiling generated bindings in tests: the exception-suppressing
 * variant emits a call to android.util.Log, which is not on the test classpath.
 */
public class Log {
    public static int w(String tag, String msg) {
        return 0;
    }
}
