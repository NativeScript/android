package com.tns.tests;

import com.tns.tests.interfaces.defaultmethods.contract.AllDefaultListener;
import com.tns.tests.interfaces.defaultmethods.contract.Producer;

import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.PrimitiveIterator;
import java.util.Spliterator;
import java.util.function.Consumer;
import java.util.function.IntConsumer;
import java.util.function.IntPredicate;
import java.util.function.LongPredicate;
import java.util.function.Predicate;

// Java callers of interface default methods for testInterfaceDefaultMethodOverrides.js. Uses
// API 24 types, so specs touch it only on API 24+.
public class DefaultMethodFixtures {
    public static final String CALLER_THREAD = "default-method-caller";

    // Only classes the runtime generates on the device declare this field; the static
    // binding generator's classes do not.
    public static boolean isRuntimeGenerated(Object instance) {
        try {
            instance.getClass().getDeclaredField("__initialized");
            return true;
        } catch (NoSuchFieldException e) {
            return false;
        }
    }

    public static String className(Object instance) {
        return instance.getClass().getName();
    }

    public static boolean isClassLoaded(String name, Object anyAppObject) {
        try {
            Class.forName(name, false, anyAppObject.getClass().getClassLoader());
            return true;
        } catch (ClassNotFoundException e) {
            return false;
        }
    }

    public static String remove(Iterator<?> it) {
        try {
            it.remove();
            return "removed";
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    public static String next(Iterator<?> it) {
        try {
            return java.util.Objects.toString(it.next());
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    public static String drain(Iterator<?> it) {
        final List<String> seen = new ArrayList<>();
        it.forEachRemaining(new Consumer<Object>() {
            @Override
            public void accept(Object item) {
                seen.add(java.util.Objects.toString(item));
            }
        });
        return android.text.TextUtils.join(",", seen);
    }

    public static String drainInts(PrimitiveIterator.OfInt it) {
        final StringBuilder seen = new StringBuilder();
        it.forEachRemaining(new IntConsumer() {
            @Override
            public void accept(int value) {
                if (seen.length() > 0) {
                    seen.append(',');
                }
                seen.append(value);
            }
        });
        return seen.toString();
    }

    @SuppressWarnings("unchecked")
    public static String negate(Predicate<?> p, Object value) {
        Predicate<Object> predicate = (Predicate<Object>) p;
        return predicate.test(value) + "|" + predicate.negate().test(value);
    }

    @SuppressWarnings("unchecked")
    public static String and(Predicate<?> p, Predicate<?> other, Object value) {
        return Boolean.toString(((Predicate<Object>) p).and((Predicate<Object>) other).test(value));
    }

    public static String intNegate(IntPredicate p, int value) {
        return p.test(value) + "|" + p.negate().test(value);
    }

    public static String longNegate(LongPredicate p, long value) {
        return p.test(value) + "|" + p.negate().test(value);
    }

    public static String getComparator(Spliterator<?> spliterator) {
        try {
            return java.util.Objects.toString(spliterator.getComparator());
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    public static String spliteratorDefaults(Spliterator<?> spliterator) {
        return spliterator.getExactSizeIfKnown() + "|" + spliterator.hasCharacteristics(Spliterator.SIZED)
                + "|" + spliterator.hasCharacteristics(Spliterator.SORTED);
    }

    public static String fire(AllDefaultListener listener) {
        try {
            listener.onText("a");
            listener.onNumber(7);
            return listener.describe();
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    public static String produce(Producer producer) {
        try {
            return producer.produce();
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    // Only on API levels whose Enumeration has the asIterator() default method.
    @SuppressWarnings("NewApi")
    public static String asIterator(Enumeration<?> enumeration) {
        try {
            return enumeration.asIterator().getClass().getName();
        } catch (Throwable t) {
            return t.getClass().getName();
        }
    }

    public static boolean isDesugared() throws NoSuchMethodException {
        return !AllDefaultListener.class.getMethod("describe").isDefault();
    }

    /*
     * Calls it.remove() on a new thread while the calling (JS) thread waits here, so a
     * call that needed the JS thread could not complete before the timeout. Reports the
     * thread the exception was thrown on, its type, and whether Iterator's own body threw it.
     */
    public static String removeOnBackgroundThread(final Iterator<?> it, long timeoutMillis) throws InterruptedException {
        final String[] result = new String[1];
        Thread caller = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    it.remove();
                    result[0] = Thread.currentThread().getName() + "|removed";
                } catch (Throwable t) {
                    boolean fromIteratorBody = false;
                    for (StackTraceElement frame : t.getStackTrace()) {
                        if (frame.getClassName().equals("java.util.Iterator") && frame.getMethodName().equals("remove")) {
                            fromIteratorBody = true;
                        }
                    }
                    result[0] = Thread.currentThread().getName() + "|" + t.getClass().getName() + "|" + fromIteratorBody;
                }
            }
        }, CALLER_THREAD);
        caller.start();
        caller.join(timeoutMillis);
        return caller.isAlive() ? "timeout" : result[0];
    }

    public static String describeOnBackgroundThread(final AllDefaultListener listener, long timeoutMillis) throws InterruptedException {
        final String[] result = new String[1];
        Thread caller = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    result[0] = listener.describe();
                } catch (Throwable t) {
                    result[0] = t.getClass().getName();
                }
            }
        }, CALLER_THREAD);
        caller.start();
        caller.join(timeoutMillis);
        return caller.isAlive() ? "timeout" : result[0];
    }
}
