package com.tns.system.classes.caching.impl;

import com.tns.system.classes.caching.ClassCache;

import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class ClassCacheImplTest {

    private static final String TEST_CLASS_NAME = "com.test.Test";
    private final ClassCache classCache = ClassCacheImpl.INSTANCE;

    @Test
    public void testGetClassShouldThrowExceptionWhenAddedClassNameIsNull(){
        try {
            classCache.getCachedClass(null);
            fail();
        } catch (RuntimeException e){
            assertCorrectExceptionForEmptyClassName(e);
        }
    }

    private void assertCorrectExceptionForEmptyClassName(RuntimeException e){
        assertEquals("Unexpected exception type", RuntimeException.class.getName(), e.getClass().getName());
        assertEquals("Unexpected exception message", "Provide a class name", e.getMessage());
    }

    @Test
    public void testGetClassShouldThrowExceptionWhenAddedClassNameIsEmpty(){
        try{
            classCache.getCachedClass("");
            fail();
        } catch (RuntimeException e){
            assertCorrectExceptionForEmptyClassName(e);
        }
    }

    @Test
    public void testAddClassShouldThrowExceptionWhenAddedClassIsNull() {
        try {
            classCache.addClass(TEST_CLASS_NAME, null);
            fail();
        } catch (RuntimeException e) {
            assertEquals("Unexpected exception type", RuntimeException.class.getName(), e.getClass().getName());
            assertEquals("Unexpected exception message", "Provide a non null class", e.getMessage());
        }
    }

    @Test
    public void testAddClassShouldThrowExceptionWhenAddedClassNameIsNull() {
        try {
            classCache.addClass(null, Object.class);
            fail();
        } catch (RuntimeException e) {
            assertCorrectExceptionForEmptyClassName(e);
        }
    }

    @Test
    public void testAddClassShouldThrowExceptionWhenAddedClassNameIsEmpty() {
        try {
            classCache.addClass("", Object.class);
            fail();
        } catch (RuntimeException e) {
            assertCorrectExceptionForEmptyClassName(e);
        }
    }

    @Test
    public void testClassCacheImplIsASingleton() {
        ClassCache classCache1 = ClassCacheImpl.INSTANCE;
        ClassCache classCache2 = ClassCacheImpl.INSTANCE;

        assertTrue("Unexpected difference in instances", classCache1 == classCache2);
    }
}