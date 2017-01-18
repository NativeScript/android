package com.tns.tests;

import java.io.Serializable;

public class InheritanceChainResolutionTest {
    public InheritanceChainResolutionTest() {
    }

    public String echo1(Object s) {
        return "Object=" + s;
    }

    public String echo1(Serializable s) {
        return "Serializable=" + s;
    }

    public String echo1(CharSequence s) {
        return "CharSequence=" + s;
    }

    public String echo1(String s) {
        return "String=" + s;
    }

    public String echo2(Object s) {
        return "Object=" + s;
    }

    public String echo2(Serializable s) {
        return "Serializable=" + s;
    }

    public String echo2(CharSequence s) {
        return "CharSequence=" + s;
    }
}
