package com.tns.tests;

import java.lang.ref.WeakReference;
import java.util.HashMap;

import kotlin.jvm.functions.Function1;

// Fixtures for testInterfaceProxies.js. Its specs implement these interfaces only through
// calls hidden from the static binding generator, so none of them is ever prebuilt, and
// make the calls from Java.
public class InterfaceProxyFixtures {
    public interface Values {
        boolean booleanValue(boolean value);

        byte byteValue(byte value);

        char charValue(char value);

        short shortValue(short value);

        int intValue(int value);

        long longValue(long value);

        float floatValue(float value);

        double doubleValue(double value);

        void run();

        String stringValue(String value);

        Object objectValue(Object value);

        int[] intArray(int[] value);

        String join(String... parts);
    }

    public interface Base {
        String base();
    }

    public interface Derived extends Base {
        String derived();
    }

    public interface Generic<T> {
        T apply(T value);
    }

    public interface Identity {
        String name();
    }

    public interface ThreadReporter {
        String report(String caller);
    }

    public interface WorkerTarget {
        String describe();
    }

    // A java.lang.reflect.Proxy cannot implement this: its method clashes with the
    // runtime's own NativeScriptRuntimeBound.getRuntimeTag__ns(), which returns int.
    public interface ProxyIncompatible {
        long getRuntimeTag__ns();

        String describe();
    }

    // Implemented only through an anonymous extend, so its shared class is always asked
    // for under a call-site name.
    public interface ProxyIncompatibleExtended extends ProxyIncompatible {
    }

    public static String callAllValues(Values v) {
        StringBuilder sb = new StringBuilder();
        sb.append(v.booleanValue(true)).append('|');
        sb.append(v.byteValue((byte) -5)).append('|');
        sb.append(v.charValue('x')).append('|');
        sb.append(v.shortValue((short) -300)).append('|');
        sb.append(v.intValue(70000)).append('|');
        sb.append(v.longValue(1L << 40)).append('|');
        sb.append(v.floatValue(1.5f)).append('|');
        sb.append(v.doubleValue(-2.25)).append('|');
        v.run();
        sb.append(v.stringValue("str")).append('|');
        sb.append(v.objectValue("obj")).append('|');
        int[] ints = v.intArray(new int[]{1, 2, 3});
        sb.append(ints == null ? "null" : String.valueOf(ints.length)).append('|');
        sb.append(v.join("a", "b", "c"));
        return sb.toString();
    }

    public static String callDefaultedValues(Values v) {
        return v.booleanValue(true) + "|" + v.byteValue((byte) 1) + "|" + (int) v.charValue('x') + "|" + v.shortValue((short) 1)
                + "|" + v.intValue(1) + "|" + v.longValue(1) + "|" + v.floatValue(1) + "|" + v.doubleValue(1)
                + "|" + v.stringValue("s") + "|" + v.objectValue("o") + "|" + v.intArray(null);
    }

    public static String callRun(Values v) {
        try {
            v.run();
            return "returned";
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    public static String callStringValue(Values v, String value) {
        try {
            return v.stringValue(value);
        } catch (Throwable t) {
            return t.getClass().getName() + ": " + t.getMessage();
        }
    }

    public static String callDerived(Derived d) {
        Base asBase = d;
        return asBase.base() + "|" + d.derived();
    }

    @SuppressWarnings("unchecked")
    public static Object applyGeneric(Generic<?> g, Object value) {
        return ((Generic<Object>) g).apply(value);
    }

    @SuppressWarnings("unchecked")
    public static Object invokeFunction1(Function1<?, ?> f, Object value) {
        return ((Function1<Object, Object>) f).invoke(value);
    }

    public static boolean hashCodeIsIdentity(Object o) {
        return o.hashCode() == System.identityHashCode(o);
    }

    public static boolean equalsIsIdentity(Object o) {
        return o.equals(o) && !o.equals(new Object()) && !o.equals(null);
    }

    public static boolean toStringIsIdentity(Object o) {
        return o.toString().equals(o.getClass().getName() + "@" + Integer.toHexString(System.identityHashCode(o)));
    }

    public static boolean worksAsHashMapKey(Object key, Object other) {
        HashMap<Object, String> map = new HashMap<>();
        map.put(key, "key");
        map.put(other, "other");
        return "key".equals(map.get(key)) && "other".equals(map.get(other)) && map.size() == 2;
    }

    public static Object echo(Object o) {
        return o;
    }

    public static String produce(com.tns.tests.interfaces.defaultmethods.contract.Producer producer) {
        try {
            return producer.produce();
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    public static String describe(ProxyIncompatible target) {
        return target.describe();
    }

    public static String describeWorkerTarget(WorkerTarget target) {
        return target.describe();
    }

    public interface PrimitiveReturns extends PrimitiveReturnTest.Returns {
    }

    private static volatile String threadReport;

    // Returns at once: without multithreaded JS the call waits for the JS thread to go idle.
    public static void startReportOnNewThread(final ThreadReporter reporter) {
        threadReport = null;
        new Thread(new Runnable() {
            @Override
            public void run() {
                threadReport = reporter.report(Thread.currentThread().getName());
            }
        }, "InterfaceProxyFixtures-caller").start();
    }

    public static String threadReport() {
        return threadReport;
    }

    private static Object held;

    public static void hold(Object o) {
        held = o;
    }

    public static String callHeld() {
        return ((Identity) held).name();
    }

    public static void release() {
        held = null;
    }

    public static WeakReference<Object> weakly(Object o) {
        return new WeakReference<>(o);
    }

    public static boolean isCleared(WeakReference<?> ref) {
        return ref.get() == null;
    }
}
