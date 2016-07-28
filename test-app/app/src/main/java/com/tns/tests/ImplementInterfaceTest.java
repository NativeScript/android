package com.tns.tests;

import java.io.IOException;

public class ImplementInterfaceTest {
    public interface I1 {
        void f();
    }

    public interface I2 extends I1 {
        void g();
    }

    public interface I3 extends I2 {
    }

    public static void triggerF(I3 i3) {
        i3.f();
    }

    public static void triggerG(I3 i3) {
        i3.g();
    }

    public static void triggerClose(java.io.Closeable c) throws IOException {
        c.close();
    }

}
