package com.tns.system.classloaders.impl;

import com.tns.system.classloaders.ClassLoadersCollection;

import org.junit.Test;

import java.util.Collection;
import java.util.ConcurrentModificationException;
import java.util.concurrent.atomic.AtomicBoolean;

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

    @Test
    public void testIteratingWhileAnotherThreadAddsClassLoaders() throws InterruptedException {
        final AtomicBoolean adding = new AtomicBoolean(true);
        Thread writer = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    for (int i = 0; i < 2000; i++) {
                        classLoadersCollection.addClassLoader(new ClassLoader(null) {
                        });
                    }
                } finally {
                    adding.set(false);
                }
            }
        });
        writer.start();

        try {
            while (adding.get()) {
                for (ClassLoader classLoader : classLoadersCollection.getClassLoadersCollection()) {
                    classLoader.getParent();
                }
            }
        } catch (ConcurrentModificationException e) {
            fail("Iterating the class loaders failed while another thread added one: " + e);
        } finally {
            writer.join();
        }
    }
}
