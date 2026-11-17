package com.tns.tests.interfaces.defaultmethods.contract;

// Every method is a default method. The app module is dexed for the app's minSdk, so below 24
// D8 turns these into abstract methods; at 24 and above they stay default methods.
public interface AllDefaultListener {
    default void onText(String text) {
    }

    default void onNumber(int number) {
    }

    default String describe() {
        return "java default on " + Thread.currentThread().getName();
    }
}
