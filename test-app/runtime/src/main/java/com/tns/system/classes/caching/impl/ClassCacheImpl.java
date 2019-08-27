package com.tns.system.classes.caching.impl;

import com.tns.system.classes.caching.ClassCache;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public enum ClassCacheImpl implements ClassCache {

    INSTANCE;

    private static final String NULL_CLASS_MESSAGE = "Provide a non null class";
    private static final String NULL_OR_EMPTY_CLASS_NAME_MESSAGE = "Provide a class name";

    private Map<String, Class<?>> classesCache = Collections.synchronizedMap(new HashMap<String, Class<?>>());

    @Override
    public Class<?> getCachedClass(String className) {
        if (className == null || className.equals("")) {
            throw new RuntimeException(NULL_OR_EMPTY_CLASS_NAME_MESSAGE);
        }

        return classesCache.get(className);
    }

    @Override
    public void addClass(String className, Class<?> clazz) {
        if (clazz == null) {
            throw new RuntimeException(NULL_CLASS_MESSAGE);
        }

        if (className == null || className.equals("")) {
            throw new RuntimeException(NULL_OR_EMPTY_CLASS_NAME_MESSAGE);
        }

        classesCache.put(className, clazz);
    }
}