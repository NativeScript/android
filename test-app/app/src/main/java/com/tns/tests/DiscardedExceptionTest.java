package com.tns.tests;

public class DiscardedExceptionTest {
    public void reportSupressedException() {
        try {
            throw new Exception("Exception to suppress");
        } catch (Throwable ex) {
            com.tns.Runtime.passSuppressedExceptionToJs(ex, "reportSupressedException");
        }
    }
}
