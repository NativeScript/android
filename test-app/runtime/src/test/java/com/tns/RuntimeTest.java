package com.tns;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.Mockito.mock;

import org.junit.Before;
import org.junit.Test;

public class RuntimeTest {

    private static final int TESTING_NATIVE_OBJECT_ID = 666;
    private static final Object TESTING_NATIVE_OBJECT = new Object();

    private Runtime runtime;
    private ObjectTable objectTable;

    @Before
    public void setUp() {
        objectTable = new ObjectTable();
        runtime = new Runtime(mock(ClassResolver.class),
                mock(GcListener.class),
                mock(StaticConfiguration.class),
                mock(DynamicConfiguration.class),
                0,
                0,
                objectTable);
    }

    @Test
    public void testStrongInstanceIsSuccessfullyRemoved() {
        objectTable.putStrong(TESTING_NATIVE_OBJECT_ID, TESTING_NATIVE_OBJECT);

        runtime.releaseNativeCounterpart(TESTING_NATIVE_OBJECT_ID);

        assertNull(objectTable.get(TESTING_NATIVE_OBJECT_ID));
        assertEquals(ObjectTable.NO_ID, objectTable.findId(TESTING_NATIVE_OBJECT));
    }

    @Test
    public void testWeakInstanceIsSuccessfullyRemoved() {
        objectTable.putStrong(TESTING_NATIVE_OBJECT_ID, TESTING_NATIVE_OBJECT);
        objectTable.weakenAndCheckAlive(TESTING_NATIVE_OBJECT_ID);

        runtime.releaseNativeCounterpart(TESTING_NATIVE_OBJECT_ID);

        assertNull(objectTable.get(TESTING_NATIVE_OBJECT_ID));
        assertEquals(ObjectTable.NO_ID, objectTable.findId(TESTING_NATIVE_OBJECT));
        assertEquals(0, objectTable.size());
    }
}
