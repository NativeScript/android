package com.tns.system.classes.loading.impl;

import com.tns.system.classes.caching.ClassCache;
import com.tns.system.classes.caching.impl.ClassCacheImpl;
import com.tns.system.classes.loading.ClassStorageService;
import com.tns.system.classloaders.ClassLoadersCollection;
import com.tns.system.classloaders.impl.ClassLoadersCollectionImpl;

public class ClassStorageServiceImpl implements ClassStorageService {

    private final ClassCache classCache = ClassCacheImpl.INSTANCE;
    private final ClassLoadersCollection classLoadersCollection = ClassLoadersCollectionImpl.INSTANCE;

    @Override
    public Class<?> retrieveClass(String className) {
        Class<?> clazz = classCache.getClass(className);

        if (clazz != null) {
            return clazz;
        }

        try {
            clazz = Class.forName(className);
            classCache.addClass(className, clazz);
            return clazz;
        } catch (ClassNotFoundException e) {
            // continue to class loaders lookup
        }

        for (ClassLoader classLoader : classLoadersCollection.getClassLoadersCollection()) {
            try {
                clazz = classLoader.loadClass(className);
                if (clazz != null) {
                    classCache.addClass(className, clazz);
                    return clazz;
                }
            } catch (ClassNotFoundException e) {
                // continue to next class loader if any
            }
        }

        throw new RuntimeException("Class \"" + className + "\" not found.");
    }

    @Override
    public void storeClass(String className, Class<?> clazz) {
        classCache.addClass(className, clazz);
        classLoadersCollection.addClassLoader(clazz.getClassLoader());
    }


}
