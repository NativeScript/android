package com.tns.system.classes.caching.impl;

import com.tns.system.classes.caching.ClassCache;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public enum ClassCacheImpl implements ClassCache {

    INSTANCE;

    private Map<String, Class<?>> classesCache = Collections.synchronizedMap(new HashMap<String, Class<?>>());

    @Override
    public Class<?> getClass(String className) {
        return classesCache.get(className);
    }

    @Override
    public void addClass(String className, Class<?> clazz) {
        if (clazz == null) {
            throw new RuntimeException("Provide a non null class");
        }

        if (className == null || className.equals("")) {
            throw new RuntimeException("Provide a class name");
        }

        classesCache.put(className, clazz);
    }

    @Override
    public void addGeneratedClass(String className, Class<?> clazz) {
        addClass(className, clazz);
    }
}
