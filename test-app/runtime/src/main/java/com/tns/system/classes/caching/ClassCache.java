package com.tns.system.classes.caching;

public interface ClassCache {
    Class<?> getClass(String className);
    void addClass(String className, Class<?> clazz);
    void addGeneratedClass(String className, Class<?> clazz);
}
