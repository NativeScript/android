package com.tns.tests.inheritance.classes;

public abstract class NonGenericAbstractClassBase {
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
        return "string";
    }

    public String getString(String str) {
        System.out.println("public String getString(String str)");
        return str;
    }

    public String getString(String str, int i) {
        System.out.println("public String getString(String str, int i)");
        return str + i;
    }

    public abstract void workAbstract();

    public abstract void workAbstract(String str);

    public abstract void workAbstract(String str, int i);

    public abstract String getStringAbstract();

    public abstract String getStringAbstract(String str);

    public abstract String getStringAbstract(String str, int i);
}
