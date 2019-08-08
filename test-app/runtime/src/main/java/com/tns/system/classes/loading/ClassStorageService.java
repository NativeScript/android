package com.tns.system.classes.loading;

public interface ClassStorageService {
    /**
     * Retrieve class by given key
     *
     * @param lookupKey key used for getting the class
     * @return stored class
     */
    Class<?> retrieveClass(String lookupKey);

    /**
     * Adds a class to the storage
     *
     * @param lookupKey key used for getting the class at later point; must not be null or empty string; at most times it's probably best to be the class' name
     * @param clazz     class to store
     */
    void storeClass(String lookupKey, Class<?> clazz);
}
