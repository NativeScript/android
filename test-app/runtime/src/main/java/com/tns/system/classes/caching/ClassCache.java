package com.tns.system.classes.caching;

public interface ClassCache {

    /**
     * Get cached class by given key
     * @param lookupKey key used for getting the class
     * @return cached class
     */
    Class<?> getCachedClass(String lookupKey);

    /**
     * Adds a class to the cache
     *
     * @param lookupKey key used for getting the class at later point; must not be null or empty string; at most times it's probably best to be the class' name
     * @param clazz     class to store in the cache
     */
    void addClass(String lookupKey, Class<?> clazz);
}
