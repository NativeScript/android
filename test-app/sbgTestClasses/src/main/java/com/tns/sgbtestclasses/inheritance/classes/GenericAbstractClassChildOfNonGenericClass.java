package com.tns.sgbtestclasses.inheritance.classes;

public abstract class GenericAbstractClassChildOfNonGenericClass<K, P> extends NonGenericAbstractClassBase {

    private static final String CLASS_NAME = "GenericAbstractClassChildOfNonGenericClass";

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

    @Override
    public void workAbstract() {
        return;
    }

    @Override
    public void workAbstract(String str) {
        return;
    }

    @Override
    public void workAbstract(String str, int i) {
        return;
    }

    @Override
    public String getStringAbstract() {
        return CLASS_NAME;
    }

    @Override
    public String getStringAbstract(String str) {
        return CLASS_NAME + str;
    }

    @Override
    public String getStringAbstract(String str, int i) {
        return CLASS_NAME + str + i;
    }
}
