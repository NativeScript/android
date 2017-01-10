package com.tns.tests;

public class ClassX {
    public static boolean IsCollected;

    public ClassX() {
        IsCollected = false;
    }

    public void dummy() {
    }

    @Override
    protected void finalize() {
        IsCollected = true;
    }
}
