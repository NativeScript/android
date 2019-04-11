package com.tns.sgbtestclasses.inheritance.classes;

public abstract class NonGenericAbstractClassChildOfNonGenericClass extends NonGenericAbstractClassBase {

    private static final String CLASS_NAME = "NonGenericAbstractClassChildOfNonGenericClass";

    @Override
    public void work() {
        System.out.println("public void work()");
    }

    @Override
    public void work(String str) {
        System.out.println("public void work(String str)");
    }

    @Override
    public void work(String str, int i) {
        System.out.println("public void work(String str, int i)");
    }

    @Override
    public String getString() {
        System.out.println("public String getString()");
        return CLASS_NAME + "string";
    }

    @Override
    public String getString(String str) {
        System.out.println("public String getString(String str)");
        return CLASS_NAME + str;
    }

    @Override
    public String getString(String str, int i) {
        System.out.println("public String getString(String str, int i)");
        return CLASS_NAME + str + i;
    }
}
