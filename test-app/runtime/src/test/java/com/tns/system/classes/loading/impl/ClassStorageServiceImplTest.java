package com.tns.system.classes.loading.impl;

import com.tns.system.classes.caching.ClassCache;
import com.tns.system.classes.loading.ClassStorageService;
import com.tns.system.classes.loading.LookedUpClassNotFound;
import com.tns.system.classloaders.ClassLoadersCollection;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.MockitoAnnotations;

import java.util.Arrays;
import java.util.Collections;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

public class ClassStorageServiceImplTest {

    private static final String TEST_CLASS_LOOKUP_NAME = "com.test.Test";

    @Mock
    private ClassCache classCache;

    @Mock
    private ClassLoadersCollection classLoadersCollection;

    private ClassStorageService classStorageService;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        classStorageService = new ClassStorageServiceImpl(classCache, classLoadersCollection);
    }

    @Test
    public void testRetrieveClassShouldCheckClassCacheFirst() {
        Class<?> testResult = Object.class;
        Mockito.<Class<?>>when(classCache.getCachedClass(TEST_CLASS_LOOKUP_NAME)).thenReturn(testResult);

        Class<?> clazz = classStorageService.retrieveClass(TEST_CLASS_LOOKUP_NAME);

        assertSame("Unexpected class returned", testResult, clazz);
        verify(classCache).getCachedClass(TEST_CLASS_LOOKUP_NAME);
        verifyZeroInteractions(classLoadersCollection);
    }

    @Test
    public void testRetrieveClassShouldCheckClassForNameSecond() {
        Class<?> clazz = classStorageService.retrieveClass(Object.class.getName());

        assertSame("Unexpected class returned", Object.class, clazz);
        verify(classCache).getCachedClass(Object.class.getName());
        verifyZeroInteractions(classLoadersCollection);
    }

    @Test
    public void testRetrieveClassShouldCheckClassLoadersCollectionThird() throws ClassNotFoundException {
        Class<?> testResult = Object.class;
        ClassLoader mockedClassLoader = mock(ClassLoader.class);
        Mockito.<Class<?>>when(mockedClassLoader.loadClass(TEST_CLASS_LOOKUP_NAME)).thenReturn(testResult);
        when(classLoadersCollection.getClassLoadersCollection()).thenReturn(Collections.singleton(mockedClassLoader));

        Class<?> clazz = classStorageService.retrieveClass(TEST_CLASS_LOOKUP_NAME);

        assertSame("Unexpected class returned", testResult, clazz);
        verify(mockedClassLoader).loadClass(TEST_CLASS_LOOKUP_NAME);
    }

    @Test
    public void testRetrieveClassShouldCheckClassLoadersCollectionAndContinueToNextClassLoaderWithoutCrash() throws ClassNotFoundException {
        Class<?> testResult = Object.class;
        ClassLoader mockedClassLoaderWithoutClass = mock(ClassLoader.class);
        ClassLoader mockedClassLoaderWithClass = mock(ClassLoader.class);

        Mockito.<Class<?>>when(mockedClassLoaderWithClass.loadClass(TEST_CLASS_LOOKUP_NAME)).thenReturn(testResult);
        when(classLoadersCollection.getClassLoadersCollection()).thenReturn(Arrays.asList(mockedClassLoaderWithoutClass, mockedClassLoaderWithClass));


        Class<?> clazz = classStorageService.retrieveClass(TEST_CLASS_LOOKUP_NAME);

        assertSame("Unexpected class returned", testResult, clazz);
        verify(mockedClassLoaderWithoutClass).loadClass(TEST_CLASS_LOOKUP_NAME);
        verify(mockedClassLoaderWithClass).loadClass(TEST_CLASS_LOOKUP_NAME);
    }

    @Test
    public void testRetrieveClassShouldThrowExceptionIfClassIsNotFound() {
        try {
            classStorageService.retrieveClass(TEST_CLASS_LOOKUP_NAME);
            fail();
        } catch (LookedUpClassNotFound e) {
            assertEquals("Unexpected exception message", "Class \"" + TEST_CLASS_LOOKUP_NAME + "\" not found.", e.getMessage());
        }
    }

    @Test
    public void storeClass() {
        Class<?> storedClass = Object.class;

        classStorageService.storeClass(TEST_CLASS_LOOKUP_NAME, storedClass);

        verify(classCache).addClass(TEST_CLASS_LOOKUP_NAME, storedClass);
        verify(classLoadersCollection).addClassLoader(storedClass.getClassLoader());
    }
}