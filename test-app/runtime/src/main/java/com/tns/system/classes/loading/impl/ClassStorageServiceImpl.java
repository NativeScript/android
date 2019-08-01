package com.tns.system.classes.loading.impl;

import com.tns.system.classes.caching.ClassCache;
import com.tns.system.classes.loading.ClassStorageService;
import com.tns.system.classloaders.ClassLoadersCollection;

public class ClassStorageServiceImpl implements ClassStorageService {

    private final ClassCache classCache;
    private final ClassLoadersCollection classLoadersCollection;

    public ClassStorageServiceImpl(ClassCache classCache, ClassLoadersCollection classLoadersCollection) {
        this.classCache = classCache;
        this.classLoadersCollection = classLoadersCollection;
    }

    @Override
    public Class<?> retrieveClass(String lookupKey) {
        Class<?> clazz = classCache.getCachedClass(lookupKey);

        if (clazz != null) {
            return clazz;
        }

        try {
            clazz = Class.forName(lookupKey);
            classCache.addClass(lookupKey, clazz);
            return clazz;
        } catch (ClassNotFoundException e) {
            // continue to class loaders lookup
        }

        for (ClassLoader classLoader : classLoadersCollection.getClassLoadersCollection()) {
            try {
                clazz = classLoader.loadClass(lookupKey);
                if (clazz != null) {
                    classCache.addClass(lookupKey, clazz);
                    return clazz;
                }
            } catch (ClassNotFoundException e) {
                // continue to next class loader if any
            }
        }

        throw new RuntimeException("Class \"" + lookupKey + "\" not found.");
    }

    @Override
    public void storeClass(String lookupKey, Class<?> clazz) {
        classCache.addClass(lookupKey, clazz);
        classLoadersCollection.addClassLoader(clazz.getClassLoader());
    }


}
