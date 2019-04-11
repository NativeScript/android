package com.tns.sgbtestclasses.inheritance.classes;

public class GenericSimpleClassChildOfNonGenericClass<T> extends NonGenericSimpleClassBase {

    private static final String CLASS_NAME = "GenericSimpleClassChildOfNonGenericClass";

    public void work() {
        System.out.println("public void work()");
    }

    public void work(String str) {
        System.out.println("public void work(String str)");
    }

    public void work(String str, int i) {
        System.out.println("public void work(String str, int i)");
    }

    public String getString() {
        System.out.println("public String getString()");
        return CLASS_NAME + "string";
    }

    public String getString(String str) {
        System.out.println("public String getString(String str)");
        return CLASS_NAME + str;
    }

    public String getString(String str, int i) {
        System.out.println("public String getString(String str, int i)");
        return CLASS_NAME + str + i;
    }
}
