package com.tns.system.classloaders.impl;

import com.tns.system.classloaders.ClassLoadersCollection;

import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

public enum ClassLoadersCollectionImpl implements ClassLoadersCollection {

    INSTANCE;

    private Set<ClassLoader> classLoaders = Collections.synchronizedSet(new HashSet<ClassLoader>());

    @Override
    public void addClassLoader(ClassLoader classLoader) {
        if (classLoader == null) {
            throw new RuntimeException("Provided class loader should not be null");
        }

        classLoaders.add(classLoader);
    }

    @Override
    public Collection<ClassLoader> getClassLoadersCollection() {
        return Collections.unmodifiableCollection(classLoaders);
    }
}
