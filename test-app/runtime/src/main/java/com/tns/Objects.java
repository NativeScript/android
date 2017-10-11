package com.tns;

public final class Objects {
    private Objects() {
    }

    /**
     * Returns true if two possibly-null objects are equal.
     */
    public static boolean equal(Object a, Object b) {
        return a == b || (a != null && a.equals(b));
    }

}
