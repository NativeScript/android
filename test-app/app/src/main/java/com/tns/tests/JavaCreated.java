package com.tns.tests;

/** Instantiates a class from Java, for benchmarks of objects JS did not construct. */
public class JavaCreated {
    public static Object newInstance(Class<?> clazz) throws Exception {
        return clazz.getDeclaredConstructor().newInstance();
    }
}
