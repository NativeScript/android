package com.tns.system.classes.loading;

public interface ClassStorageService {
    Class<?> retrieveClass(String className);
    void storeClass(String className, Class<?> clazz);
}
