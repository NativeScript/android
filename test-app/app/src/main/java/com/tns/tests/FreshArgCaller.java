package com.tns.tests;

// Passes JS an argument Java registers for that call only. The argument is also reachable
// through a field, which JS reads without a method call.
public class FreshArgCaller {
    public interface Target {
        String take(Object arg);
    }

    public static Object pending;

    public static String callWithNewObject(Target target) {
        StringBuilder arg = new StringBuilder("fresh");
        pending = arg;
        try {
            return target.take(arg);
        } finally {
            pending = null;
        }
    }
}
