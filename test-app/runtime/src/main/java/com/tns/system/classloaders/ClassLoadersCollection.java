package com.tns.system.classloaders;

import java.util.Collection;

public interface ClassLoadersCollection {

    /**
     * Adds a class loader to the internal collection of class loaders
     *
     * @param classLoader class loader to add; should not be null
     */
    void addClassLoader(ClassLoader classLoader);

    /**
     * Returns an unmodifiable collection of the stored class loaders
     *
     * @return unmodifiable collection
     */
    Collection<ClassLoader> getClassLoadersCollection();
}
