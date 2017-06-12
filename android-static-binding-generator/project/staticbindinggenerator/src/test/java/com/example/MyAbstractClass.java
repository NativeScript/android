package com.example;

public abstract class MyAbstractClass {
    String className;

    public MyAbstractClass(String name) {
        this.className = name;
    }

    public abstract void abstractMethod();
}
