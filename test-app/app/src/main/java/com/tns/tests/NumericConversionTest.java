package com.tns.tests;

public class NumericConversionTest {
    private String init;

    public NumericConversionTest() {
        init = "";
    }

    public NumericConversionTest(String s) {
        init = "String";
    }

    public NumericConversionTest(byte b) {
        init = "byte";
    }

    public NumericConversionTest(Object o, byte value) {
        init = "Object,byte";
    }

    public NumericConversionTest(Object o, int value) {
        init = "Object,int";
    }

    public NumericConversionTest(Object o, long value) {
        init = "Object,long";
    }

    public NumericConversionTest(Object o1, Object o2, byte value) {
        init = "Object,Object,byte";
    }

    public NumericConversionTest(Object o1, Object o2, short value) {
        init = "Object,Object,short";
    }

    public String getInit() {
        return init;
    }

    public String method1(Object value) {
        return "Object=" + value;
    }

    public String method1(short value) {
        return "short=" + value;
    }

    public String method1(Object o, short value) {
        return "short=" + value;
    }

    public String method2(float value) {
        return "float=" + value;
    }

    public String method2(double value) {
        return "double=" + value;
    }

    public String method2(Object value) {
        return "Object=" + value;
    }

    public String method3(Object value) {
        return "Object=" + value;
    }

    public String method3(short value) {
        return "short=" + value;
    }

    public String method3(long value) {
        return "long=" + value;
    }

    public String method4(Object value) {
        return "Object=" + value;
    }

    public String method4(short value) {
        return "short=" + value;
    }

    public String method5(Object value) {
        return "Object=" + value;
    }

    public String method5(byte value) {
        return "byte=" + value;
    }

    public String method5(short value) {
        return "short=" + value;
    }

    public String method5(int value) {
        return "int=" + value;
    }

    public String method5(long value) {
        return "long=" + value;
    }

    public String method5(float value) {
        return "float=" + value;
    }

    public String method5(double value) {
        return "double=" + value;
    }
}
