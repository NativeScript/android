package com.tns.system.classloaders.impl;

import com.tns.system.classloaders.ClassLoadersCollection;

import org.junit.Test;

import java.util.Collection;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

public class ClassLoadersCollectionImplTest {

    private final ClassLoadersCollection classLoadersCollection = ClassLoadersCollectionImpl.INSTANCE;

    @Test
    public void testAddClassLoaderShouldThrowExceptionIfClassLoaderIsNull() {
        try {
            classLoadersCollection.addClassLoader(null);
            fail();
        } catch (RuntimeException e) {
            assertEquals("Unexpected exception type", RuntimeException.class.getName(), e.getClass().getName());
            assertEquals("Unexpected exception message", "Provided class loader should not be null", e.getMessage());
        }
    }

    @Test
    public void testGetClassLoadersCollectionIsImmutable() {
        Collection<ClassLoader> classLoaders = classLoadersCollection.getClassLoadersCollection();

        assertEquals("Unexpected collection type", "UnmodifiableCollection", classLoaders.getClass().getSimpleName());
    }
}