package com.tns.tests;

public class MyTestBaseClass3 {
    public void callMeWithAString(java.lang.String[] stringArr, Runnable arbitraryInterface) {
        String res = "I lied, call me with an array!";
        android.util.Log.d("Log", "callMeWithAString called.");
    }
}