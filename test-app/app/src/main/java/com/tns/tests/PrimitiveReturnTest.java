package com.tns.tests;

/**
 * Calls each primitive-returning method of a JS implementation and reports either the
 * returned value or the class of what was thrown, so a spec can pin how JS return values
 * convert to each primitive type.
 */
public class PrimitiveReturnTest {
    public interface Returns {
        boolean z();

        byte b();

        char c();

        short s();

        int i();

        long j();

        float f();

        double d();
    }

    public static String call(Returns r, String kind) {
        try {
            switch (kind) {
                case "z":
                    return "ok:" + r.z();
                case "b":
                    return "ok:" + r.b();
                case "c":
                    return "ok:" + (int) r.c();
                case "s":
                    return "ok:" + r.s();
                case "i":
                    return "ok:" + r.i();
                case "j":
                    return "ok:" + r.j();
                case "f":
                    return "ok:" + r.f();
                case "d":
                    return "ok:" + r.d();
                default:
                    throw new IllegalArgumentException(kind);
            }
        } catch (Throwable t) {
            return "ex:" + t.getClass().getSimpleName();
        }
    }
}
