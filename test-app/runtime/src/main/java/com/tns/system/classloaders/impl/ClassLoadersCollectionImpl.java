package com.tns.system.classloaders.impl;

import com.tns.system.classloaders.ClassLoadersCollection;

import java.util.Collection;
import java.util.Collections;
import java.util.Set;
import java.util.concurrent.CopyOnWriteArraySet;

public enum ClassLoadersCollectionImpl implements ClassLoadersCollection {

    INSTANCE;

    // Iterated on every class lookup miss by any runtime's thread while another may be adding a
    // loader; loaders are added rarely, so iteration works on a snapshot instead of a lock.
    private final Set<ClassLoader> classLoaders = new CopyOnWriteArraySet<ClassLoader>();

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
