package com.tns;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Map;

import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.mockingDetails;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

public class RuntimeTest {

    private static final int TESTING_NATIVE_OBJECT_ID = 666;
    private static final Object TESTING_NATIVE_OBJECT = new Object();

    private Runtime runtime;

    @Mock
    private HashMap<Integer, Object> strongInstances;

    @Mock
    private HashMap<Integer, WeakReference<Object>> weakInstances;

    @Mock
    private NativeScriptHashMap<Object, Integer> strongJavaObjectToId;

    @Mock
    private NativeScriptWeakHashMap<Object, Integer> weakJavaObjectToId;

    @Mock
    private WeakReference<Object> testingObjectWeakRefMock;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        runtime = new Runtime(mock(ClassResolver.class),
                mock(GcListener.class),
                mock(StaticConfiguration.class),
                mock(DynamicConfiguration.class),
                0,
                0,
                strongInstances,
                weakInstances,
                strongJavaObjectToId,
                weakJavaObjectToId);
    }

    @Test
    public void testStrongInstanceIsSuccessfullyRemoved() {
        when(strongInstances.get(TESTING_NATIVE_OBJECT_ID)).thenReturn(TESTING_NATIVE_OBJECT);

        runtime.releaseNativeCounterpart(TESTING_NATIVE_OBJECT_ID);

        verify(strongInstances).remove(TESTING_NATIVE_OBJECT_ID);
        verify(strongJavaObjectToId).remove(TESTING_NATIVE_OBJECT);
    }

    @Test
    public void testWeakInstanceIsSuccessfullyRemoved(){
        when(weakInstances.get(TESTING_NATIVE_OBJECT_ID)).thenReturn(testingObjectWeakRefMock);

        runtime.releaseNativeCounterpart(TESTING_NATIVE_OBJECT_ID);

        verify(weakInstances).remove(TESTING_NATIVE_OBJECT_ID);
        verify(weakJavaObjectToId).remove(testingObjectWeakRefMock);
    }

    @Test
    public void testStrongAndWeakInstancesAreSuccessfullyRemoved(){
        when(strongInstances.get(TESTING_NATIVE_OBJECT_ID)).thenReturn(TESTING_NATIVE_OBJECT);
        when(weakInstances.get(TESTING_NATIVE_OBJECT_ID)).thenReturn(testingObjectWeakRefMock);

        runtime.releaseNativeCounterpart(TESTING_NATIVE_OBJECT_ID);

        verify(strongInstances).remove(TESTING_NATIVE_OBJECT_ID);
        verify(strongJavaObjectToId).remove(TESTING_NATIVE_OBJECT);
        verify(weakInstances).remove(TESTING_NATIVE_OBJECT_ID);
        verify(weakJavaObjectToId).remove(testingObjectWeakRefMock);
    }
}
